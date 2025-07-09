#include "harakiri.h"

AwesomeCallback::AwesomeCallback(std::string&& str) : str_(str) {
}

std::string AwesomeCallback::operator()() const&& {
    std::string ret = str_ + "awesomeness";
    delete this;
    return ret;
}
