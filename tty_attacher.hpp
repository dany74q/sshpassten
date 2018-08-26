#pragma once

#include <string>

namespace sshpassten {
namespace detail {
inline namespace v1 {

/*
    Given a valid TTY path, the attacher will:
    
    1. Create a new session (via setsid) - this detaches the process from its tty
    2. Attach the process to the given TTY (or PTY)
*/
struct TTYAttacher {
    static void attachTo(const std::string& ttyPath);
};

}
}
}