#ifndef KEYENCE_EXCEPTION_H
#define KEYENCE_EXCEPTION_H

#include <stdexcept>

namespace keyence
{

class KeyenceException : public std::runtime_error 
{
public:
  KeyenceException(const std::string& msg) : std::runtime_error(msg) {}
};

}

#endif