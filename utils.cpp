#include "utils.hpp"
#include <unistd.h>
#include <fcntl.h>


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