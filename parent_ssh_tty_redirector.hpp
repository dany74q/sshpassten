#pragma once

#include <string>
#include <unistd.h>
#include <sys/types.h>

namespace sshpassten {
namespace detail {
inline namespace v1 {

/*
	This SSH TTY redirector handles the necessary redirection of the SSH prompt and password
	insertion (which is the stdin and stdout of the Slave-end of our PTY),
	to and from the stdin and stdout of our Master-end PTY.

	Redirection terminates after the SSH child process dies.
*/
struct ParentSSHTTYRedirector {
	const static std::string PASSWORD_PROMPT;
	const static std::string SSH_PROMPT;

	ParentSSHTTYRedirector(const pid_t& childPID) : childPID_ { childPID } {};
	int redirect(const std::string& password, const int fileDescriptor) const;
private:
	void sendBuffer(const int fileDescriptor, const std::string& buffer) const;
	void handleSSHPromprt(const int fileDescriptor) const;
private:
	pid_t childPID_;
};

}
}
}