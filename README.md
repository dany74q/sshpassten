# SSHPassten
A C++ (mini) port of SSHPass utility - which lets you automate password-based SSH authentication

# Credit - SSHPass
SSHPass is a utility that automates SSH password authentication via the creation of a PTY (Pseudo-Terminal)
pair - tricking the SSH password prompt to accept programmatic input,
bypassing its reliance on keyboard-interactiveness.

SSHPass was written by Shachar Shemesh for Lingnu Open Source Consulting LTD, and full credit
goes to him for his great work.

# About this port
This (mini) C++ port was a fun side project that served two causes:
1. Reinvigorate my rusky C++14 skills, and
2. Helping me better understand the relationship of TTY-s, PTY-s and stdin/out/err in *nix environments

This mini-port sports both a subset and superset of SSHPass features - mainly, accepting the password
as a parameter, and adding support for SSH Prompt/CLI handling.

# Build
Build was tested with the latest Clang-6, but any C++14 compatible compiler should make-due.

$> clang++-6.0 -Wall -std=c++1z sshpassten/*.cpp -o sshpassten

# Usage
$> ./sshpassten -p mypass -- ssh -o StrictHostKeyChecking=no -o PubkeyAuthentication=no myuser@myhost
