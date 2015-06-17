#ifndef KEYENCE_UTILS_H_
#define KEYENCE_UTILS_H_

#include <cstring>

namespace keyence
{

template <typename T>
void extract(const void* ptr, T& target, size_t offset = 0)
{
  std::memcpy(&target, static_cast<const char*>(ptr) + offset, sizeof(T));
}

template<typename T>
void insert(void* ptr, const T& data, size_t offset = 0)
{
  std::memcpy(static_cast<char*>(ptr) + offset, &data, sizeof(T));
}

}

#endif