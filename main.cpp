#include "sshpassten.hpp"
#include <iostream>

using namespace sshpassten::detail;

int main(int argc, char** argv) {
	try {
		SSHPassten sshPasten;
		return sshPasten.run(argc, argv);
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}