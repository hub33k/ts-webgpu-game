#pragma once

// https://github.com/gabime/spdlog/wiki/How-to-use-spdlog-in-DLLs

#include <spdlog/spdlog.h>

// TODO (hub33k): override log filename
#if !defined(HK_LOG_FILENAME)
  #define HK_LOG_FILENAME "hub33k"
#endif

namespace hub33k {

  std::shared_ptr<spdlog::logger> GetCoreLogger();
  std::shared_ptr<spdlog::logger> GetClientLogger();

} // namespace hub33k

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-macros"
// clang-format off
// Core log macros
#define HK_LOG_CORE_TRACE(...)     ::hub33k::GetCoreLogger()->trace(__VA_ARGS__)
#define HK_LOG_CORE_INFO(...)      ::hub33k::GetCoreLogger()->info(__VA_ARGS__)
#define HK_LOG_CORE_WARN(...)      ::hub33k::GetCoreLogger()->warn(__VA_ARGS__)
#define HK_LOG_CORE_ERROR(...)     ::hub33k::GetCoreLogger()->error(__VA_ARGS__)
#define HK_LOG_CORE_CRITICAL(...)  ::hub33k::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define HK_LOG_TRACE(...)          ::hub33k::GetClientLogger()->trace(__VA_ARGS__)
#define HK_LOG_INFO(...)           ::hub33k::GetClientLogger()->info(__VA_ARGS__)
#define HK_LOG_WARN(...)           ::hub33k::GetClientLogger()->warn(__VA_ARGS__)
#define HK_LOG_ERROR(...)          ::hub33k::GetClientLogger()->error(__VA_ARGS__)
#define HK_LOG_CRITICAL(...)       ::hub33k::GetClientLogger()->critical(__VA_ARGS__)
// clang-format on
#pragma clang diagnostic pop
