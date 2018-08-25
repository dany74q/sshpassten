#pragma once

#include <string> 
#include <exception>
#include <stdexcept>
#include <sstream>

namespace sshpassten {
namespace detail {
inline namespace v1 {

struct File {
	File(const std::string& path, size_t flags);
	virtual ~File() noexcept;

    int getFileDescriptor() {
        return fileDescriptor_;
    }

private:
	int fileDescriptor_;
};

struct FileDescriptorSet {
    FileDescriptorSet(int fileDescriptor) {
		FD_ZERO(&fdSet_);
		FD_SET(fileDescriptor, &fdSet_);
    }

    fd_set* operator&() {
        return &fdSet_;
    }

private:
    fd_set fdSet_;
};

auto checkErrno = [](auto&& func, auto&&... args) {
	auto res = func(args...);
	
	if (res == -1) {
		std::stringstream ss;
		ss << "Failed with errno=" << errno;
		throw std::runtime_error(ss.str());
	}

	return res;
};

}
}
}