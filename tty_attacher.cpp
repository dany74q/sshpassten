#include "tty_attacher.hpp"
#include "utils.hpp"
#include <unistd.h>
#include <fcntl.h>

namespace sshpassten {
namespace detail {
inline namespace v1 {



void TTYAttacher::attachTo(const std::string& ttyPath) {
	// This creates a new session, making the current process the session leader, process-group leader,
	// A by-product of this is detaching the current process from its previous TTY.
	checkErrno(::setsid);

	// Opening a tty file here will attach the current process to it;
	// However, we don't need the FD itself open.
	{
		File(ttyPath, O_RDWR);
	}
}

}
}
}