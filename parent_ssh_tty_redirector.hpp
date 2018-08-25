#pragma once

#include <string>

namespace sshpassten {
namespace detail {
inline namespace v1 {

struct ParentSSHTTYRedirector {
	const static std::string PASSWORD_PROMPT;
	const static std::string SSH_PROMPT;

	ParentSSHTTYRedirector(pid_t childPID) : childPID_ { childPID } {};
	int redirect(const std::string& password, int fileDescriptor) const;
private:
	void sendBuffer(const int fileDescriptor, const std::string& buffer) const;
	void handleSSHPromprt(const int fileDescriptor) const;
private:
	pid_t childPID_;
};

}
}
}