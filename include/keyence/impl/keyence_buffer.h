#ifndef KEYENCE_BUFFER_H
#define KEYENCE_BUFFER_H

#include <vector>
#include <cstring>

namespace keyence
{

class StreamBuffer
{
public:

  const void* data() const { return buff_.data(); }
  void* data() { return buff_.data(); }

  void ensure(std::size_t sz)
  {
    if (buff_.size() < sz) resize(sz);
  }

  void resize(std::size_t sz) { buff_.resize(sz); }

  std::size_t size() const { return buff_.size(); }

private:
  std::vector<char> buff_;
  
};

struct MutableBuffer
{
  MutableBuffer(void* data, std::size_t size)
    : data(data), size(size)
  {}

  void* const data;
  std::size_t size;
};

inline MutableBuffer buffer(StreamBuffer& buffer)
{
  return MutableBuffer(buffer.data(), buffer.size()); 
}

inline MutableBuffer operator+(const MutableBuffer buffer, std::size_t inc)
{
  return MutableBuffer(static_cast<char*>(buffer.data) + inc, buffer.size - inc);
}

}

#endif