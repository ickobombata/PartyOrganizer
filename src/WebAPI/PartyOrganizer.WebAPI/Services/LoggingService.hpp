#pragma once

#include <spdlog/spdlog.h>

class LoggingService
{
public:
	void RegisterLogFile(const char* loggerName, const char* logFileName)
	{
		auto file = spdlog::daily_logger_mt(loggerName, logFileName);
		file->flush_on(spdlog::level::info);

		loggers.push_back(file);
	}
	void RegisterConsole(const char* loggerName)
	{
		auto console = spdlog::stdout_color_mt(loggerName);
		console->flush_on(spdlog::level::info);

		loggers.push_back(console);
	}

public:
	void Info(const char* message)
	{
		for (const auto& logger : loggers)
			logger->info(message);
	}

	void Error(const char* message)
	{
		for (const auto& logger : loggers)
			logger->info(message);
	}

	template<typename Arg, typename... Args>
	void Error(const char* message, const Arg& arg, const Args&... args)
	{
		for (const auto& logger : loggers)
			logger->info(message, arg, args...);
	}

	template<typename Arg, typename... Args>
	void Info(const char* message, const Arg& arg, const Args&... args)
	{
		for (const auto& logger : loggers)
			logger->info(message, arg, args...);
	}

private:
	std::vector<std::shared_ptr<spdlog::logger>> loggers;
};
