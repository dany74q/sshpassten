#pragma once

#include "pseudo_terminal_manager.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unistd.h>

namespace sshpassten {
namespace detail {
inline namespace v1 {

/* 
    SSHPassten is a C++14 mini-port of SSHPass (Not every feature was implemented).
    This utility provides the ability to pass SSH passwords via an argument, so you could automate
    password-based SSH session creation.

    How it works:

    1. We create a valid PTY (Pseudo-Terminal) pair (Master & Slave)
    2. We fork the process
    3. The child is going to create a new Session (via setsid) and attach itself to the afforementioned PTY-slave
    4. The parent is going to handle slave input and output via the master PTY

    A simple diagram showing the I/O flow:

	    stdout									 stdin						            stdin
	      ▲										   |								      |
	      |										   |								      |
	      |                                     |                                    ◀-
	 ------------              -----------      |       -----------              -----------
	|            |            |           |     |	   |           |            |           |
	|   Parent   |<---------->|   Master  |<---------->|   Slave   |<---------->|   Child   |
	|            |            |    PTY    |	    |	   |    PTY    |            |   (SSH)   |
	 ------------              -----------      |       -----------              -----------
	        -▶                                  |                                     |
	       |                                                                          |
	       |                                                                          ▼
             stdin                                                                      stdout
	
	Support for handling the SSH prompt was added as well.
*/
struct SSHPassten {
    int run(int argc, char** argv);
private:
    /*
        Usage: 

        sshpassten -p password -- ssh -o StrictHostKeyChecking=no -o PubkeyAuthentication=no user@host
    */
    void parseArgs(int argc, char** argv);
    
    /*
        We're going to fork, and the child handler will be execvp-ing itself to the passed SSH command.
    */
    int childHandler() const;

    /*
        The parent handler will handle input from the created pseudo-terminal.
    */
    int parentHandler() const;
private:
    std::string password_;
    std::vector<std::string> command_;
    std::unique_ptr<PseudoTerminalManager> manager_;
    pid_t childPID_;
};

}
}
}
