#include "keyence/impl/keyence_tcp_client.h"
#include "keyence/impl/keyence_exception.h"
// For given message
#include "keyence/impl/messages/high_speed_single_profile.h"

void displayProfile(const keyence::command::SingleProfile::Response& resp)
{
  // Display General Profile Info
  std::cout << "Encoder count: " << resp.encoder_count << '\n';
  std::cout << "Trigger count: " << resp.trigger_count<< '\n';
  // Display Interpretation Information about Profiles
  const double depth_unit = KEYENCE_FUNDAMENTAL_LENGTH_UNIT * resp.profile_info.data_unit;
  const double x_unit = KEYENCE_FUNDAMENTAL_LENGTH_UNIT * resp.profile_info.x_increment;
  const double x_start = KEYENCE_FUNDAMENTAL_LENGTH_UNIT * resp.profile_info.x_start;

  std::cout << "Profile width: " << resp.profile_info.num_profiles << '\n';
  std::cout << "Depth Unit: " << (depth_unit * 1e6) << " (um)\n";
  std::cout << "Step Unit: " << (x_unit * 1e6) << " (um)\n";
  std::cout << "Start Offset: " << (x_start * 1e6) << " (um)\n";

  // Display every 100th point
  for (std::size_t i = 0; i < resp.profile_points.size(); i+=100)
  {
    std::cout << "Point" << i << " (" << ((x_start + i * x_unit) * 1e6) << ", ";
    // Check for 'inf' indicating the point was out of range of the sensor's
    // measuring capability
    if (!keyence::isPointValid(resp.profile_points[i]))
    {
      std::cout << "INVALID)\n";
    }
    else
    {
      std::cout << resp.profile_points[i] * depth_unit << ")\n";
    }
  }
}

int main(int argc, char** argv)
{
  using keyence::command::SingleProfile;
  using keyence::Client;

  if (argc != 3)
  {
    std::cerr << "Usage: ./keyence_change_program <host> <port>\n";
    return 1;
  }

  try
  {
    keyence::TcpClient keyence (argv[1], argv[2]);

    SingleProfile::Request req;

    Client::Response<SingleProfile::Request> resp = keyence.sendReceive(req);

    std::cout << resp.header << '\n';

    displayProfile(resp.body);
  }
  catch (const keyence::KeyenceException& exc)
  {
    std::cerr << exc.what();
    return -1;
  }

  return 0;
}

