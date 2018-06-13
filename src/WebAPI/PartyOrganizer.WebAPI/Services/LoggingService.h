#pragma once

#include <spdlog/spdlog.h>

class LoggingService
{
public:
	LoggingService(const char *);

public:
	void Info(const char *);
	void Error(const char *);
	
	template<typename Arg, typename... Args>
	void Info(const char *, const Arg &, const Args & ...);
	
	template<typename Arg, typename... Args>
	void Error(const char *, const Arg &, const Args & ...);

private:
	std::shared_ptr<spdlog::logger> logger;
};

LoggingService::LoggingService(const char* logFileName) : logger(nullptr)
{
	logger = spdlog::daily_logger_mt("WebAPI", logFileName);
	logger->flush_on(spdlog::level::info);
}

void LoggingService::Info(const char* message)
{
	logger->info(message);
}

void LoggingService::Error(const char* message)
{
	logger->error(message);
}

template<typename Arg, typename... Args>
void LoggingService::Error(const char* message, const Arg& arg, const Args&... args)
{
	logger->error(message, arg, args...);
}

template<typename Arg, typename... Args>
void LoggingService::Info(const char* message, const Arg& arg, const Args&... args)
{
	logger->info(message, arg, args...);
}
