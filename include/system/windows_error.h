/**
 * @file windows_error.h
 * @brief Error handling.
 *
 * @author Chen Zhenshuo (chenzs108@outlook.com)
 * @author Liu Guowen (liu.guowen@outlook.com)
 * @version 1.0
 * @date 2021-07-10
 * @par GitHub
 * https://github.com/czs108
 * @par
 * https://github.com/lgw1995
 *
 * @see https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes
 */

#pragma once

#include <Windows.h>

#include <string>
#include <system_error>


namespace sys {

/**
 * Throw a `WindowsError` exception containing the Windows Sockets last-error.
 *
 * @warning This function does not return.
 */
[[noreturn]] void ThrowWsaLastError();

/**
 * Throw a `WindowsError` exception containing the last-error.
 *
 * @warning This function does not return.
 */
[[noreturn]] void ThrowLastError();

/**
 * The Windows-related exception containing error information.
 */
class WindowsError : public std::system_error {
public:
    /**
     * Create a Windows exception.
     *
     * @param err_code  An error code.
     */
    WindowsError(const int err_code) noexcept;

    /**
     * Get the explanatory message.
     */
    const char* what() const noexcept override;

private:
    //! The explanatory message.
    const std::string msg_;
};

/**
 * The map between error codes and their explanatory messages.
 */
class WindowsCategory : public std::error_category {
public:
    /**
     * Get an instance of `WindowsCategory`.
     */
    static const std::error_category& Instance() noexcept;

    /**
     * Get the name of the category.
     */
    const char* name() const noexcept override;

    /**
     * Get the explanatory message from an error code.
     */
    std::string message(const int err_code) const override;

private:
    //! The global instance of `WindowsCategory`.
    static const WindowsCategory inst_;
};

}  // namespace sys