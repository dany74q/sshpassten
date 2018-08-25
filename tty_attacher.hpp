#pragma once
#include <string>

namespace sshpassten {
namespace detail {
inline namespace v1 {

struct TTYAttacher {
	static void attachTo(const std::string& ttyPath);
};

}
}
}