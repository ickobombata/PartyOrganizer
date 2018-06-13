#pragma once

#include <spdlog/spdlog.h>

class LoggingService
{
public:
	void RegisterLogFile(const char *, const char *);
	void RegisterConsole(const char *);

public:
	void Info(const char *);
	void Error(const char *);

	template<typename Arg, typename... Args>
	void Info(const char *, const Arg &, const Args & ...);

	template<typename Arg, typename... Args>
	void Error(const char *, const Arg &, const Args & ...);

private:
	std::vector<std::shared_ptr<spdlog::logger>> loggers;
};

void LoggingService::RegisterLogFile(const char* loggerName, const char* logFileName)
{
	auto file = spdlog::daily_logger_mt(loggerName, logFileName);
	file->flush_on(spdlog::level::info);

	loggers.push_back(file);
}

void LoggingService::RegisterConsole(const char* loggerName)
{
	auto console = spdlog::stdout_color_mt(loggerName);
	console->flush_on(spdlog::level::info);

	loggers.push_back(console);
}

void LoggingService::Info(const char* message)
{
	for (const auto& logger : loggers)
		logger->info(message);
}

void LoggingService::Error(const char* message)
{
	for (const auto& logger : loggers)
		logger->info(message);
}

template<typename Arg, typename... Args>
void LoggingService::Error(const char* message, const Arg& arg, const Args&... args)
{
	for (const auto& logger : loggers)
		logger->info(message, arg, args...);
}

template<typename Arg, typename... Args>
void LoggingService::Info(const char* message, const Arg& arg, const Args&... args)
{
	for (const auto& logger : loggers)
		logger->info(message, arg, args...);
}
