#pragma once
#include "utils.hpp"

#include <memory>
#include <functional>
#include <sys/ioctl.h>
#include <signal.h>
#include <mutex>
#include <thread>

namespace sshpassten {
namespace detail {
inline namespace v1 {

/* 
	A Header-only resize TTY resize (SIGWINCH) binder, which binds signals from 
	a one TTY (or PTY) to a second TTY (or PTY).

	What this means is that for every window resize signal the first TTY gets,
	we'll fire a callback which will signal the second TTY with to resize to the same dimentions.
*/
struct TTYResizeSignalBinder {
private:
    static const std::string CURRENT_TTY_PATH;
    static std::unique_ptr<File> currentTTY_;
    static int bindResizeToFD_;
    static bool signalSet_;
	static std::mutex signalLock_;

public:
    TTYResizeSignalBinder() = delete;
    ~TTYResizeSignalBinder() = default;

	static void bindSignal(int bindResizeToFD) {
		currentTTY_ = std::make_unique<File>(CURRENT_TTY_PATH, 0);
		bindSignal(currentTTY_->getFileDescriptor(), bindResizeToFD);
	}

    static void bindSignal(int bindResizeFromFD, int bindResizeToFD) {
		std::lock_guard<std::mutex> lock(signalLock_);

        if (!signalSet_) {
            signalSet_ = true;

            bindResizeToFD_ = bindResizeToFD;

            winsize ttysize {};
            if(!ioctl(bindResizeFromFD, TIOCGWINSZ, &ttysize)) {
                signal(SIGWINCH, &resize_handler);
                ioctl(bindResizeToFD_, TIOCSWINSZ, &ttysize);
            }
        }
    }

    static void resize_handler(int sigNum) {
        winsize ttysize {};

        if(!ioctl(currentTTY_->getFileDescriptor(), TIOCGWINSZ, &ttysize)) {
            ioctl(bindResizeToFD_, TIOCSWINSZ, &ttysize);
        }
    }
};

const std::string TTYResizeSignalBinder::CURRENT_TTY_PATH = "/dev/tty";

bool TTYResizeSignalBinder::signalSet_ = false;

std::unique_ptr<File> TTYResizeSignalBinder::currentTTY_ = nullptr;

int TTYResizeSignalBinder::bindResizeToFD_ = 0;

std::mutex TTYResizeSignalBinder::signalLock_ = {};

}
}
}