#include <ros/ros.h>


#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>

#include <limits>

#include "keyence/impl/keyence_tcp_client.h"
#include "keyence/impl/keyence_exception.h"

#include "keyence/impl/messages/high_speed_single_profile.h"
#include "keyence/impl/messages/change_program.h"

// keyence protocol / profile related defines
static const std::string KEYENCE_DEFAULT_TCP_PORT = "24691";
static const std::string KEYENCE_DEFAULT_TCP_PORT_HS = "24692";

#define KEYENCE_RET_CODE_NO_DATA 0xA0

const static double KEYENCE_PDEPTH_UNIT = 0.01d; // in micro meters

// TODO: only a difference of 2; most likely some (undocumented) flag?
const static int32_t KEYENCE_INFINITE_DISTANCE_VALUE = -524288;
const static int32_t KEYENCE_INFINITE_DISTANCE_VALUE2 = -524286;

// values LJ Navigator uses for out-of-range points (in meters)
const static double KEYENCE_INFINITE_DISTANCE_VALUE_SI = -999.9990d / 1e3;
const static double KEYENCE_INFINITE_DISTANCE_VALUE_SI2 = -999.9970d / 1e3;

// default values for parameters
const static double DEFAULT_SAMPLE_RATE = 10.0;
const static std::string DEFAULT_FRAME_ID = "sensor_optical_frame";

// error codes for receive_get_profile_response(..)
#define RESP_ERR_OK 0
#define RESP_ERR_PFX_LEN 1
#define RESP_ERR_UNL_PFX_LEN 2
#define RESP_ERR_PKT_BUFF_OVERFLOW 3
#define RESP_ERR_RECV_PAYLOAD 4
#define RESP_ERR_EAGAIN 5
#define RESP_ERR_BRC 6
#define RESP_ERR_ABN_BODY_LEN 7
#define RESP_ERR_HDR_RET_CODE 8

// local types
typedef pcl::PointCloud<pcl::PointXYZ> point_cloud_t;

// Prototype for function that converts a given profile to
// a PCL point cloud
int keyence_profile_to_pc(const keyence::ProfileInformation& info,
                          const std::vector<int32_t>& points,
                          point_cloud_t::Ptr msg,
                          bool cnv_inf_pts,
                          double scale_factor);


int main(int argc, char** argv)
{
  ros::init (argc, argv, "keyence_lj_driver");
  ros::NodeHandle nh, pnh("~");

  // ros parameters
  std::string sensor_host;
  std::string head_a_model;
  std::string frame_id;
  std::string sensor_port;
  double sample_rate;

  // param init
  // check required parameters
  if (!pnh.hasParam("controller_ip"))
  {
   ROS_FATAL("Parameter 'controller_ip' missing. Cannot continue.");
   return -1;
  }
  
  if (!pnh.hasParam("head_a_model"))
  {
   ROS_FATAL("Parameter 'head_a_model' missing. Cannot continue.");
   return -1;
  }

  pnh.getParam("controller_ip", sensor_host);
  pnh.getParam("head_a_model", head_a_model);
  pnh.param<std::string>("controller_port", sensor_port, KEYENCE_DEFAULT_TCP_PORT);
  pnh.param("sample_rate", sample_rate, DEFAULT_SAMPLE_RATE);
  pnh.param<std::string>("frame_id", frame_id, DEFAULT_FRAME_ID);

  ROS_INFO("Connecting to %s (TCP %s), expecting a single %s head",
   sensor_host.c_str(), sensor_port.c_str(), head_a_model.c_str());

  ROS_INFO("Attempting to publish at %.2f Hz", sample_rate);

  // setup point cloud message (we reuse single one)
  // TODO: this won't work with nodelets
  point_cloud_t::Ptr pc_msg(new point_cloud_t);
  pc_msg->header.frame_id = frame_id;
  // message is essentially a line-strip of points
  pc_msg->height = 1;

  // set up profile cloud publisher
  ros::Publisher pub = nh.advertise<point_cloud_t>("keyence_profiles", 10);

  while (ros::ok())
  {
    try
    {
      // Establish communications
      keyence::TcpClient keyence (sensor_host, sensor_port);
      ROS_INFO_STREAM("Keyence connection established");

      // Set the Keyence to safe default program
      keyence::command::ChangeProgram::Request cmd (0);
      keyence.sendReceive(cmd);

      // Main loop
      ros::Rate sleeper(sample_rate);
      while (ros::ok())
      {
        sleeper.sleep();
        ros::spinOnce();

        // avoid interacting with sensor if there are no publishers
        // TODO: maybe we should actually always poll sensor, but just not
        //       publish anything (although unpacking + publishing is cheap)
        if (pub.getNumSubscribers() == 0)
        {
          ROS_INFO_THROTTLE(60, "No (more) subscribers. Not polling sensor.");
          continue;
        }

        // unpack profile data
        keyence::command::SingleProfile::Request req;
        keyence::Client::Response<keyence::command::SingleProfile::Request> resp = keyence.sendReceive(req);
        // Check for success
        if (resp.header.return_code != 0)
        {
          ROS_WARN_STREAM("Error code in header:\n" << resp.header);
        }
        else
        {
          // convert to pointcloud
          pc_msg->points.clear();
          keyence_profile_to_pc(resp.body.profile_info, resp.body.profile_points, pc_msg, true, 1);

          // publish pointcloud
          pub.publish(pc_msg);
        }
      } // end main loop
    }
    catch (const keyence::KeyenceException& ex)
    {
      ROS_ERROR_STREAM("Keyence Exception Caught:\n" << ex.what());
      ROS_ERROR_STREAM("Attempting reconnection after pause");
      ros::Duration(1.0).sleep();
    }
  }

  return 0;
}

// assumption: 'msg' is a properly setup (and empty) PointCloud msg

int keyence_profile_to_pc(const keyence::ProfileInformation& info, const std::vector<int32_t>& points, 
                          point_cloud_t::Ptr msg, bool cnv_inf_pts, double scale_factor)
{
  // TODO: get proper timestamp from somewhere
  // pcl header stamps are in microseconds
  msg->header.stamp = ros::Time::now().toNSec()/1e3;
  msg->width = info.num_profiles;
  cnv_inf_pts=true;
  scale_factor=1;

  const double x_start = (info.x_start * KEYENCE_PDEPTH_UNIT);
  double x = 0., y = 0., z = 0.;

  msg->points.reserve(info.num_profiles);

  // add points
  for (unsigned int i = 0; i < points.size(); ++i)
  {
    // convert profile points to SI units (meters)
    x = (x_start + (i * (info.x_increment * KEYENCE_PDEPTH_UNIT))) / 1e6;
    y = 0.;
    z = ((info.data_unit * KEYENCE_PDEPTH_UNIT) * points[i]) / 1e6;

    // filter out 'infinite distance' points
    // REP-117: http://www.ros.org/reps/rep-0117.html
    //  "out of range detections will be represented by +Inf."
    if(points[i] == KEYENCE_INFINITE_DISTANCE_VALUE)
    {
      if (cnv_inf_pts)
        z = std::numeric_limits<double>::infinity();
      else
        z = KEYENCE_INFINITE_DISTANCE_VALUE_SI;
    }

    // device returns two different values that are supposed to be interpreted
    // as out-of-range or 'infinite'. This is the second
    if(points[i] == KEYENCE_INFINITE_DISTANCE_VALUE2)
    {
      if (cnv_inf_pts)
        z = std::numeric_limits<double>::infinity();
      else
        z = KEYENCE_INFINITE_DISTANCE_VALUE_SI2;
    }

    x *= scale_factor;
    y *= scale_factor;
    z *= scale_factor; // 'inf * something' still inf

    msg->points.push_back(pcl::PointXYZ(x, y, z));
  }

  return 0;
}
