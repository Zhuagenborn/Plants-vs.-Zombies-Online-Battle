#include "windows_error.h"

#pragma comment(lib, "ws2_32.lib")


namespace sys {

/**
 * Get the explanatory message from an error code.
 *
 * @param err_code  An error code.
 */
std::string FormatErrMsg(const int err_code);


const WindowsCategory WindowsCategory::inst_{};

const std::error_category& WindowsCategory::Instance() noexcept {
    return inst_;
}

const char* WindowsCategory::name() const noexcept {
    return "Microsoft Windows";
}

std::string WindowsCategory::message(const int err_code) const {
    return FormatErrMsg(err_code);
}


WindowsError::WindowsError(const int err_code) noexcept :
    std::system_error{ err_code, WindowsCategory::Instance() },
    msg_{ FormatErrMsg(err_code) } {}

const char* WindowsError::what() const noexcept {
    return msg_.c_str();
}

[[noreturn]] void ThrowWsaLastError() {
    throw WindowsError{ WSAGetLastError() };
}

[[noreturn]] void ThrowLastError() {
    throw WindowsError{ static_cast<int>(GetLastError()) };
}

std::string FormatErrMsg(const int err_code) {
    char* raw_msg{ nullptr };
    if (const auto length{ FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, err_code, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
            reinterpret_cast<char*>(&raw_msg), 0, nullptr) };
        length == 0) {
        ThrowLastError();
    }

    std::string msg{ raw_msg };
    LocalFree(raw_msg);
    return msg;
}

}  // namespace sys