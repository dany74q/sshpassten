#include "pseudo_terminal_manager.hpp"
#include "parent_ssh_tty_redirector.hpp"
#include "tty_attacher.hpp"
#include "window_resize_handler.hpp"
#include "args.hxx"
#include <unistd.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace sshpassten::detail;

int main(int argc, char** argv) {
	args::ArgumentParser argParser("parser");
    args::HelpFlag help(argParser, "help", "Display this help menu", {'h', "help"});
    args::ValueFlag<std::string> password(argParser, "password", "Pass the SSH password here", {'p'});
    args::PositionalList<std::string> command(argParser, "command", "The command to be executed");

	argParser.ParseCLI(argc, argv);

	if (!password || !command) {
		std::cout << argParser << std::endl;
		return -1;
	}

	PseudoTerminalManager manager;
	WindowResizeHandler::setSignal(manager.getMasterFileDescriptor());

	auto childPID = fork();
	if (!childPID) {
		// Child
		TTYAttacher::attachTo(manager.getSlavePTYPath());

		std::vector<const char*> commandVector {};
		std::transform(std::begin(command), std::end(command), std::back_inserter(commandVector), 
			[] (const std::string& str) -> const char* { return str.c_str(); }
		);
		commandVector.emplace_back(nullptr);
		return checkErrno(execvp, commandVector.front(), const_cast<char**>(commandVector.data()));
	}

	// Parent
	ParentSSHTTYRedirector parentRedirector { childPID };
	return parentRedirector.redirect(args::get(password), manager.getMasterFileDescriptor());
}