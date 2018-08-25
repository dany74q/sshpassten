#include "sshpassten.hpp"
#include "pseudo_terminal_manager.hpp"
#include "parent_ssh_tty_redirector.hpp"
#include "tty_resize_signal_binder.hpp"
#include "tty_attacher.hpp"
#include "args.hpp"
#include <vector>
#include <iostream>
#include <unistd.h>


namespace sshpassten {
namespace detail {
inline namespace v1 {


int SSHPassten::run(int argc, char** argv) {
	parseArgs(argc, argv);

	manager_ = std::make_unique<PseudoTerminalManager>();
	TTYResizeSignalBinder::bindSignal(manager_->getMasterFileDescriptor());

	childPID_ = fork();
	if (!childPID_) {
		return childHandler();
	}

	return parentHandler();
}

void SSHPassten::parseArgs(int argc, char** argv) {
	args::ArgumentParser argParser("parser");
	args::HelpFlag help(argParser, "help", "Display this help menu", {'h', "help"});

	args::ValueFlag<std::string> password(argParser, "password", "Pass the SSH password here", {'p'});
	args::PositionalList<std::string> command(argParser, "command", "The command to be executed");

	argParser.ParseCLI(argc, argv);

	if (!password || !command) {
		std::cout << argParser << std::endl;
		throw std::runtime_error("Failed parsing args");
	}

	password_ = args::get(password);
	auto commandContainer = args::get(command);
	std::copy(std::begin(commandContainer), std::end(commandContainer), std::back_inserter(command_));
}

int SSHPassten::childHandler() const {
	TTYAttacher::attachTo(manager_->getSlavePTYPath());

	std::vector<const char*> commandVector {};
	std::transform(std::begin(command_), std::end(command_), std::back_inserter(commandVector), 
		[] (const std::string& str) -> const char* { return str.c_str(); }
	);
	commandVector.emplace_back(nullptr);
	
	return checkErrno(execvp, commandVector.front(), const_cast<char**>(commandVector.data()));
}

int SSHPassten::parentHandler() const {
	ParentSSHTTYRedirector parentRedirector { childPID_ };
	return parentRedirector.redirect(password_, manager_->getMasterFileDescriptor());
}

}
}
}