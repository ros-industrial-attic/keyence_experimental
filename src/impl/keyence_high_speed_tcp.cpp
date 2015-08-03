#include "keyence/impl/keyence_high_speed_tcp.h"
#include "boost/thread.hpp"
#include "boost/atomic.hpp"

#include "keyence/impl/messages/prepare_high_speed.h"
#include "keyence/impl/messages/start_high_speed.h"
#include "keyence/impl/messages/stop_high_speed.h"
#include "keyence/impl/keyence_exception.h"
#include "keyence/impl/ljv7_rawdata.h"

#include "libsocket/epoll.hpp"

#define KEYENCE_HIGHSPEED_NOTIFY_MESSAGE_SIZE 8

namespace keyence
{

static void worker_main(boost::atomic_flag& flag,
                        libsocket::inet_stream& sock,
                        keyence::HighSpeedTcpClient::callback_type cb,
                        uint16_t profile_size)
{
  using libsocket::inet_stream;
  // prepare buffers
  const size_t msg_length = 24 + (profile_size * 5 / 2) + 4;
  std::vector<unsigned char> byte_buffer (msg_length);
  std::vector<int32_t> points_buffer (profile_size);
  // keep track of the 'message state'
  size_t received = 0;

  libsocket::epollset<inet_stream> poll;
  poll.add_fd(sock, LIBSOCKET_READ);

  while (flag.test_and_set())
  {
    libsocket::epollset<inet_stream>::ready_socks ready = poll.wait(100);
    if (!ready.first.empty())
    {
      // read into buffer
      ssize_t n = sock.rcv(byte_buffer.data() + received, msg_length - received);
      if (n <= 0)
      {
        // Socket has been closed
        std::cerr << "High speed socket closed\n";
        break;
      }

      received += n;

      if (received == msg_length)
      {
        // if we have enough, process
        ljv7_unpack_profile_data(byte_buffer.data(), byte_buffer.size(),
                                 profile_size, points_buffer.data(),
                                 points_buffer.size() * 4);

        cb(points_buffer.data(), points_buffer.size());
        // reset message state
        received = 0;
      }
      else if (received == KEYENCE_HIGHSPEED_NOTIFY_MESSAGE_SIZE)
      {
        if (byte_buffer[0] == 0xFF && byte_buffer[1] == 0xFF &&
            byte_buffer[2] == 0xFF && byte_buffer[3] == 0xFF)
        {
          std::cerr << "High-Speed-Mode Stopped with notify code: " << int(byte_buffer[4]) << std::endl;
          break;
        }
      }
    }
  } // end main loop

  return;
}

// Impl details
struct HighSpeedTcpClient::Impl
{
  Impl(const std::string host_ip,
       const std::string host_port,
       const ProfileInformation& info,
       uint32_t code)
    : start_code(code)
    , info(info)
  {
    // todo: socket exception safety
    sock.connect(host_ip, host_port, LIBSOCKET_IPv4);
  }

  ~Impl()
  {
    stop();
  }

  void start(callback_type cb)
  {
    flag.test_and_set();
    worker = boost::thread(worker_main, boost::ref(flag), boost::ref(sock), cb, info.num_profiles);
  }

  void stop()
  {
    flag.clear();
    worker.join();
  }

  // Socket details
  libsocket::inet_stream sock;

  // Worker details
  boost::thread worker;
  boost::atomic_flag flag;

  // Data
  uint32_t start_code;
  ProfileInformation info;
};


HighSpeedTcpClient::HighSpeedTcpClient(TcpClient &client, const std::string &port)
  : client_(client)
  , impl_(NULL)
{
  using keyence::command::PrepareHighSpeed;

  PrepareHighSpeed::Request prepare (PrepareHighSpeed::Request::POSITION_NEXT);
  Client::Response<PrepareHighSpeed::Request> resp = client.sendReceive(prepare);

  if (resp.header.return_code)
  {
    // Something went wrong
    std::ostringstream ss;
    ss << "High-Speed-Mode prepare command failed with code: " << int(resp.header.return_code);
    throw keyence::KeyenceException(ss.str());
  }

  try
  {
    impl_ = new Impl(client.hostName(), port, resp.body.profile_info, resp.body.start_code);
  }
  catch (const libsocket::socket_exception& exception)
  {
    throw keyence::KeyenceException(exception.mesg);
  }
}

HighSpeedTcpClient::~HighSpeedTcpClient()
{
  stop();
  delete impl_;
}

const ProfileInformation &HighSpeedTcpClient::profileInfo() const
{
  return impl_->info;
}

void HighSpeedTcpClient::start(HighSpeedTcpClient::callback_type cb)
{
  using keyence::command::StartHighSpeed;

  StartHighSpeed::Request start (impl_->start_code);
  Client::Response<StartHighSpeed::Request> resp = client_.sendReceive(start);

  if (resp.header.return_code)
  {
    std::ostringstream ss;
    ss << "High-Speed-Mode start command failed with code: " << int(resp.header.return_code);
    throw keyence::KeyenceException(ss.str());
  }

  impl_->start(cb);
}

void HighSpeedTcpClient::stop()
{
  using keyence::command::StopHighSpeed;

  try
  {
    StopHighSpeed::Request stop;
    Client::Response<StopHighSpeed::Request> resp = client_.sendReceive(stop);

    if (resp.header.return_code)
    {
      std::cerr << "High-Speed-Mode stop command failed with code: " << int(resp.header.return_code);
    }
    impl_->stop();
  }
  catch (keyence::KeyenceException& exc)
  {
    std::cerr << "An exception occured while trying to stop the high speed connection: " << exc.what();
  }
}

} // namespace keyence

