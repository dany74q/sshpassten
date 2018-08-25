#pragma once
#include "utils.hpp"

#include <memory>
#include <functional>
#include <sys/ioctl.h>
#include <signal.h>

namespace sshpassten {
namespace detail {
inline namespace v1 {

struct WindowResizeHandler {
private:
    const static std::string CURRENT_TTY_PATH;
    static std::unique_ptr<File> currentTTY_;
    static int masterFileDescriptor_;
    static bool signalSet;

public:
    WindowResizeHandler() = delete;
    ~WindowResizeHandler() = default;

    static void setSignal(int masterFileDescriptor) {
        if (!signalSet) {
            signalSet = true;

            masterFileDescriptor_ = masterFileDescriptor;

            winsize ttysize {};
            currentTTY_ = std::make_unique<File>(CURRENT_TTY_PATH, 0);
            if(!ioctl(currentTTY_->getFileDescriptor(), TIOCGWINSZ, &ttysize)) {
                signal(SIGWINCH, &resize_handler);
                ioctl(masterFileDescriptor_, TIOCSWINSZ, &ttysize);
            }
        }
    }

    static void resize_handler(int sigNum) {
        winsize ttysize {};

        if(!ioctl(currentTTY_->getFileDescriptor(), TIOCGWINSZ, &ttysize)) {
            ioctl(masterFileDescriptor_, TIOCSWINSZ, &ttysize);
        }
    }
};

const std::string WindowResizeHandler::CURRENT_TTY_PATH = "/dev/tty";
bool WindowResizeHandler::signalSet = false;
std::unique_ptr<File> WindowResizeHandler::currentTTY_ = nullptr;
int WindowResizeHandler::masterFileDescriptor_ = 0;

}
}
}