#include <ros/ros.h>

#include <pcl/point_types.h>
#include <pcl_ros/point_cloud.h>

#include <limits>

#include "keyence/impl/keyence_exception.h"
#include "keyence/impl/keyence_tcp_client.h"

#include "keyence/impl/messages/high_speed_single_profile.h"

#include "boost/bind.hpp"
#include "boost/ref.hpp"
#include "keyence/impl/messages/change_program.h"
#include "keyence_experimental/ChangeProgram.h"

// keyence protocol / profile related defines
static const std::string KEYENCE_DEFAULT_TCP_PORT = "24691";
static const std::string KEYENCE_DEFAULT_TCP_PORT_HS = "24692";

// values LJ Navigator uses for out-of-range points (in meters)
const static double KEYENCE_INFINITE_DISTANCE_VALUE_SI = -999.9990 / 1e3;
const static double KEYENCE_INFINITE_DISTANCE_VALUE_SI2 = -999.9970 / 1e3;

// default values for parameters
const static std::string DEFAULT_FRAME_ID = "sensor_optical_frame";

// local types
typedef pcl::PointCloud<pcl::PointXYZ> Cloud;

// Prototype for function that converts a given profile to
// a PCL point cloud
int unpackProfileToPointCloud(const keyence::ProfileInformation& info,
                              const std::vector<int32_t>& points, Cloud& msg, bool cnv_inf_pts,
                              double scale_factor);

bool changeProgramCallback(keyence_experimental::ChangeProgram::Request& req,
                           keyence_experimental::ChangeProgram::Response& res,
                           keyence::TcpClient& client, bool& active_flag)
{
  ROS_INFO("Attempting to change keyence program to %hhd with active flag: %hhd", req.program_no,
           req.active);
  keyence::command::ChangeProgram::Request cmd(req.program_no);
  keyence::Client::Response<keyence::command::ChangeProgram::Request> resp =
      client.sendReceive(cmd);
  if (resp.good())
  {
    active_flag = req.active;
    return true;
  }
  else
    return false;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "keyence_lj_driver");
  ros::NodeHandle nh, pnh("~");

  // ros parameters
  std::string sensor_host;
  std::string frame_id;
  std::string sensor_port;
  double sample_rate;

  // check required parameters
  if (!pnh.hasParam("controller_ip"))
  {
    ROS_FATAL("Parameter 'controller_ip' missing. Cannot continue.");
    return -1;
  }

  if (!pnh.hasParam("sample_rate"))
  {
    ROS_FATAL("Parameter 'sample_rate' missing. Cannot continue.");
    return -2;
  }

  pnh.getParam("controller_ip", sensor_host);
  pnh.getParam("sample_rate", sample_rate);
  pnh.param<std::string>("controller_port", sensor_port, KEYENCE_DEFAULT_TCP_PORT);
  pnh.param<std::string>("frame_id", frame_id, DEFAULT_FRAME_ID);

  ROS_INFO("Attempting to connect to %s (TCP %s); expecting a single head attached to port A.",
           sensor_host.c_str(), sensor_port.c_str());

  // setup point cloud message (we reuse single one)
  // TODO: this won't work with nodelets
  Cloud::Ptr pc_msg(new Cloud);
  pc_msg->header.frame_id = frame_id;
  pc_msg->is_dense = false; // cloud could have NaNs
  // message is essentially a line-strip of points
  pc_msg->height = 1;

  // set up profile cloud publisher
  ros::Publisher pub = nh.advertise<Cloud>("profiles", 100);

  bool active_flag = true;

  while (ros::ok())
  {
    try
    {
      // Establish communications
      keyence::TcpClient keyence(sensor_host, sensor_port);

      ros::ServiceServer program_server =
          nh.advertiseService<keyence_experimental::ChangeProgram::Request,
                              keyence_experimental::ChangeProgram::Response>(
              "change_program", boost::bind(changeProgramCallback, _1, _2, boost::ref(keyence),
                                            boost::ref(active_flag)));

      ROS_INFO("Keyence connection established");
      ROS_INFO("Attempting to publish at %.2f Hz.", sample_rate);

      // Main loop
      ros::Rate sleeper(sample_rate);
      sleeper.reset();
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

        if (!active_flag)
        {
          ROS_INFO_THROTTLE(60, "Sensor is disabled via the 'activity flag'.");
          continue;
        }

        // unpack profile data
        keyence::command::SingleProfile::Request req;
        auto resp = keyence.sendReceive(req);

        // Check for success
        if (!resp.good())
        {
          ROS_DEBUG_STREAM("Error code in header:\n" << resp.header);
        }
        else
        {
          // convert to pointcloud
          pc_msg->points.clear();
          unpackProfileToPointCloud(resp.body.profile_info, resp.body.profile_points, *pc_msg, true,
                                    1);
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

int unpackProfileToPointCloud(const keyence::ProfileInformation& info,
                              const std::vector<int32_t>& points, Cloud& msg, bool cnv_inf_pts,
                              double scale_factor)
{
  // TODO: get proper timestamp from somewhere
  // pcl header stamps are in microseconds
  msg.header.stamp = ros::Time::now().toNSec() / 1e3;
  msg.width = info.num_profiles;
  cnv_inf_pts = true;
  scale_factor = 1;

  const double x_start = (info.x_start * KEYENCE_FUNDAMENTAL_LENGTH_UNIT);
  double x = 0., y = 0., z = 0.;

  msg.points.reserve(info.num_profiles);

  // add points
  for (unsigned int i = 0; i < points.size(); ++i)
  {
    // convert profile points to SI units (meters)
    x = (x_start + (i * (info.x_increment * KEYENCE_FUNDAMENTAL_LENGTH_UNIT))) / 1e6;
    y = 0.;
    z = ((info.data_unit * KEYENCE_FUNDAMENTAL_LENGTH_UNIT) * points[i]) / 1e6;

    // filter out 'infinite distance' points
    // REP-117: http://www.ros.org/reps/rep-0117.html
    //  "out of range detections will be represented by +Inf."
    if (points[i] == KEYENCE_INVALID_DATA_VALUE)
    {
      if (cnv_inf_pts)
        z = std::numeric_limits<double>::infinity();
      else
        z = KEYENCE_INFINITE_DISTANCE_VALUE_SI;
    }

    // device returns two different values that are supposed to be interpreted
    // as out-of-range or 'infinite'. This is the second
    if (points[i] == KEYENCE_DEAD_ZONE_DATA_VALUE)
    {
      if (cnv_inf_pts)
        z = std::numeric_limits<double>::infinity();
      else
        z = KEYENCE_INFINITE_DISTANCE_VALUE_SI2;
    }

    x *= scale_factor;
    y *= scale_factor;
    z *= scale_factor; // 'inf * something' still inf

    msg.points.push_back(pcl::PointXYZ(x, y, z));
  }

  return 0;
}
