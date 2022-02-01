#include "windows_error.h"

#include <Windows.h>

#include <system_error>

#pragma comment(lib, "ws2_32.lib")


namespace sys {

[[noreturn]] void ThrowWsaLastError() {
    throw std::system_error{ WSAGetLastError(), std::system_category() };
}

[[noreturn]] void ThrowLastError() {
    throw std::system_error{ static_cast<int>(GetLastError()), std::system_category() };
}

}  // namespace sys