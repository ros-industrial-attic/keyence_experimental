#ifndef KEYENCE_CLIENT_H
#define KEYENCE_CLIENT_H

#include "keyence_message.h"

namespace keyence
{


class Client
{
public:
  virtual int send(const void* data, std::size_t size) = 0;
  virtual int recv(void* data, std::size_t size) = 0;

  // Supporting type. For a given request message, this template
  // generates a response type which contains the appropriate body data
  // and a fixed header
  template <typename ReqT>
  struct Response
  {
    Message::ResponseHeader header;
    typename ReqT::response_type body;

    bool good() const { return header.return_code == 0; }
  };

  // Synchronous interface
  // May throw instance of 'KeyenceException' on error
  template <typename ReqT>
  Response<ReqT> sendReceive(ReqT& msg)
  {
    // Get total size of request
    const std::size_t total_size = ReqT::size + // size of data payload
                                   Message::request_header_size + // size of header + fixed body segment
                                   Message::prefix_size; // size of message prefix indicating size

    // Ensure space and then encode message into local buffer
    buffer_.ensure(total_size);
    encodeRequestHeader(ReqT::command_code, ReqT::size, buffer(buffer_));
    // call message's encode function
    msg.encodeInto(buffer(buffer_) + (Message::prefix_size + Message::request_header_size));

    // Send out the raw bytes and recieve back into same buffer.
    // Output is overwritten
    sendAndGetResponse(buffer_, total_size);

    // Decode response
    Response<ReqT> response;
    decodeResponseHeader(buffer(buffer_), response.header);
    response.body.decodeFrom(buffer(buffer_) + Message::response_header_size);

    return response;
  }

protected:
  // Helper function that sends the first 'size' bytes from the stream
  // buffer 'buf' and then receives the response back into that same
  // buffer.
  void sendAndGetResponse(StreamBuffer& buf, std::size_t size)
  {
    // Send Request
    this->send(buf.data(), size);

    // Receive response size (the message 'prefix')
    uint32_t response_size;
    this->recv(&response_size, sizeof(response_size));

    // Receive response (including header, fixed-body, and message payload)
    buffer_.ensure(response_size);
    this->recv(buf.data(), response_size);
  }

private:
  StreamBuffer buffer_;
};

}

#endif
