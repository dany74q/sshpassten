#define _XOPEN_SOURCE
#include "pseudo_terminal_manager.hpp"
#include "utils.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <iostream>
#include <exception>
#include <stdexcept>

namespace sshpassten {
namespace detail {
inline namespace v1 {

const std::string PseudoTerminalManager::PSEUDO_TERMINAL_CREATOR_DEVICE = "/dev/ptmx";

PseudoTerminalManager::PseudoTerminalManager() {
	// Creates a PTY Master/Slave Pair
	masterFileDescriptor_ = std::make_unique<File>(PSEUDO_TERMINAL_CREATOR_DEVICE, O_RDWR);
	
	// Enable master-slave communication
	auto masterFD = masterFileDescriptor_->getFileDescriptor();
	checkErrno(fcntl, masterFD, F_SETFL, O_NONBLOCK);
	checkErrno(grantpt, masterFD);
	checkErrno(unlockpt, masterFD);

	// Get slave PTY path
	auto slavePTYPath = ::ptsname(masterFD);
	if (slavePTYPath == nullptr) {
		std::stringstream ss;
		ss << "ptsname failed with errno=" << errno;
		throw std::runtime_error(ss.str());
	}
	slavePTYPath_ = slavePTYPath;
	
	slaveFileDescriptor_ = std::make_unique<File>(slavePTYPath, O_RDWR);
}

std::string PseudoTerminalManager::getSlavePTYPath() const {
	return slavePTYPath_;
}

}
}
}