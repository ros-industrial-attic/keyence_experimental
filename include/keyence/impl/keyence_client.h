#ifndef KEYENCE_CLIENT_H
#define KEYENCE_CLIENT_H

#include "keyence_message.h"

#include <cassert>

namespace keyence
{

// Template Alias for determining if the class is a base of the
// keyence::Message type
// template<typename T>
// using enable_if_msg = typename std::enable_if<std::is_base_of<Message, T>::value>::type;

// Base class that represents a client to a keyence device
// This class handles keyence protocol concerns
// Sub implementations handle specific communications via send/recv overloads
class Client
{
public:
  virtual int send(const void* data, std::size_t size) = 0;
  virtual int recv(void* data, std::size_t size) = 0;

  template <typename ReqT>
  struct Response
  {
    Message::ResponseHeader header;
    typename ReqT::response_type body;
  };

  /**
   * MsgType is a class that inherits from keyence::Message
   * It must support the message concept that requires:
   *   1. A packRequestBody(MutableBuffer& buffer) function
   *   2. A decodeResponseBody(MutableBuffer& buffer) function
   *   3. A const static field indicating the command_code
   *   4. A const static field indicating the size of the body
   */
  template <typename ReqT>
  Response<ReqT> sendRecieve(ReqT& msg)
  {
    // Pack Request
    const std::size_t total_size = ReqT::size + 
                                   Message::request_header_size + 
                                   Message::prefix_size;

    buffer_.ensure(total_size);
    Message::encodeRequestHeader(ReqT::command_code, ReqT::size, buffer(buffer_));
    msg.encode(buffer(buffer_) + (Message::prefix_size + Message::request_header_size));
    
    // Send Request
    int bytes_sent = this->send(buffer_.data(), total_size);
    assert(bytes_sent == total_size);

    // Recieve response size
    uint32_t response_size;
    int bytes_recieved = this->recv(&response_size, sizeof(response_size));
    
    // Recieve response
    buffer_.ensure(response_size);
    bytes_recieved = this->recv(buffer_.data(), response_size);

    // Decode response
    Response<ReqT> response;
    Message::decodeResponseHeader(buffer(buffer_), response.header);
    response.body.decode(buffer(buffer_) + Message::response_header_size);

    return response;
  }

private:
  StreamBuffer buffer_;
};

}

#endif