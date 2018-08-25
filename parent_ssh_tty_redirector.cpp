#include "parent_ssh_tty_redirector.hpp"
#include "utils.hpp"

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <iostream>
#include <memory>
#include <sys/types.h>
#include <sys/wait.h>
#include <array>

namespace sshpassten {
namespace detail {
inline namespace v1 {

const std::string ParentSSHTTYRedirector::PASSWORD_PROMPT = "password:";
const std::string ParentSSHTTYRedirector::SSH_PROMPT = "ssh>";

int ParentSSHTTYRedirector::redirect(const std::string& password, int fileDescriptor) const {
	int status = 0;
	timespec timeout {1L, 0L};
	
	// Stop redirecting after child process has been killed
	while (!waitpid(childPID_, &status, WNOHANG)) {
		FileDescriptorSet readFileDescriptor { fileDescriptor };
		int selret = pselect(fileDescriptor + 1, &readFileDescriptor, nullptr, nullptr, &timeout, nullptr);

		if (selret > 0) {
			std::array<char, 8192> buff = { '\0' };
			auto bytesRead = read(fileDescriptor, &buff, buff.size() - 1);
			
			if (bytesRead != -1 && bytesRead > 0) {
				buff[bytesRead] = '\0';
				std::string buffStr { std::begin(buff), std::end(buff) };

				if (buffStr.find(PASSWORD_PROMPT) != std::string::npos) {
					sendBuffer(fileDescriptor, password);
				} else if (buffStr.find(SSH_PROMPT) != std::string::npos) {
					handleSSHPromprt(fileDescriptor);
				}	
			}
		}
	}

	return 0;
}

void ParentSSHTTYRedirector::sendBuffer(const int fileDescriptor, const std::string& buffer) const {
	std::stringstream ss;
    ss << std::endl;
    std::string newLine = ss.str();

	write(fileDescriptor, buffer.c_str(), buffer.size());
	write(fileDescriptor, newLine.c_str(), newLine.size());
}

void ParentSSHTTYRedirector::handleSSHPromprt(const int fileDescriptor) const {
	std::cout << std::endl << SSH_PROMPT << " ";
					
	char* line = nullptr;
	size_t linesize = 0;

	auto lineSize = getline(&line, &linesize, stdin);
	std::unique_ptr<char, decltype(std::free)*> lineOwner{ line, &std::free };

	if (lineSize != -1) {
		sendBuffer(fileDescriptor, line);
	}
}

}
}
}