#include "utils.hpp"
#include <memory>
#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include <stdexcept>

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>


namespace sshpassten {
namespace detail {
inline namespace v1 {

File::File(const std::string& path, size_t flags) : fileDescriptor_{ checkErrno(::open, path.c_str(), flags) } {
}

File::~File() noexcept {
	if (fileDescriptor_ != -1) {
		::close(fileDescriptor_);
		fileDescriptor_ = -1;
	}
}

}
}
}