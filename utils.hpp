#pragma once

#include <string> 
#include <exception>
#include <sstream>

namespace sshpassten {
namespace detail {
inline namespace v1 {

/*
	RAII FD wrapper - given a path and flags, it will use ::open and ::close
	to manage the file's lifecycle.
*/
struct File {
	File(const std::string& path, size_t flags);
	virtual ~File() noexcept;

    int getFileDescriptor() const {
        return fileDescriptor_;
    }

private:
	int fileDescriptor_;
};

/*
	RAII FD-Set wrapper - given an FD, we'll initialize an empty fd_set using it,
	and give the ability to manage the underlying pointer via operator&.
*/
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

/*
	Errno checking helper - given a function and args, we'll run them
	and check the res for -1, if that's the case, we'll throw std::runtime_error with the given
	errno
*/
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