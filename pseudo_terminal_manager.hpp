#pragma once

#include <string>
#include <memory>
#include "utils.hpp"

namespace sshpassten {
namespace detail {
inline namespace v1 {

/*
	This object will create and manage the lifecycle of a master-slave pair of PTY-s.
*/
struct PseudoTerminalManager {
	static const std::string PSEUDO_TERMINAL_CREATOR_DEVICE;

	PseudoTerminalManager();
	virtual ~PseudoTerminalManager() noexcept = default;

	std::string getSlavePTYPath() const;
    
    int getMasterFileDescriptor() const noexcept {
        return masterFileDescriptor_->getFileDescriptor();
    }

    int getSlaveFileDescriptor() const noexcept {
        return slaveFileDescriptor_->getFileDescriptor();
    }

private:
	std::unique_ptr<File> masterFileDescriptor_;
	std::unique_ptr<File> slaveFileDescriptor_;
	std::string slavePTYPath_;
};

}
}
}