#include "Log.hpp"

#include <memory>
#include <spdlog/logger.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <vector>

namespace hub33k {

  // Core
  std::shared_ptr<spdlog::logger> GetCoreLogger() {
    std::string s_LoggerCoreName = "HK_CORE";
    std::shared_ptr<spdlog::logger> logger = spdlog::get(s_LoggerCoreName);

    if (not logger) {
      std::vector<spdlog::sink_ptr> sinks;
      sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
      sinks.push_back(
        std::make_shared<spdlog::sinks::daily_file_sink_st>("logs/core/" HK_LOG_FILENAME ".core.log", 23, 59)
      );

      if (!sinks.empty()) {
        logger = std::make_shared<spdlog::logger>(s_LoggerCoreName, std::begin(sinks), std::end(sinks));
        spdlog::register_logger(logger);
      } else {
        logger = spdlog::stdout_color_mt(s_LoggerCoreName);
      }
    }

    return logger;
  }

  // Client
  std::shared_ptr<spdlog::logger> GetClientLogger() {
    std::string s_LoggerClientName = "HK";
    std::shared_ptr<spdlog::logger> logger = spdlog::get(s_LoggerClientName);

    if (not logger) {
      std::vector<spdlog::sink_ptr> sinks;
      sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
      sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>("logs/client/" HK_LOG_FILENAME ".log", 23, 59)
      );

      if (!sinks.empty()) {
        logger = std::make_shared<spdlog::logger>(s_LoggerClientName, std::begin(sinks), std::end(sinks));
        spdlog::register_logger(logger);
      } else {
        logger = spdlog::stdout_color_mt(s_LoggerClientName);
      }
    }

    return logger;
  }

} // namespace hub33k
