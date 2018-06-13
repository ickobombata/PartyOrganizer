#pragma once

#include <memory>
#include <unordered_map>

class ServiceProvider
{
public:
	static ServiceProvider& Instance()
	{
		if (!instance)
			instance = new ServiceProvider();

		return *instance;
	}

public:
	template <typename T>
	void Register(std::shared_ptr<T> service)
	{
		const std::type_info* typeInfo = &typeid(T);
		if (services.find(typeInfo->name()) != services.end())
			throw std::runtime_error("Service was already added.");

		services[typeInfo->name()] = service;
	}

	template <typename T>
	std::shared_ptr<T> Resolve()
	{
		const std::type_info* typeInfo = &typeid(T);
		if (services.find(typeInfo->name()) == services.end())
			throw std::runtime_error("Cannot find type.");

		return std::static_pointer_cast<T>(services[typeInfo->name()]);
	}

private:
	ServiceProvider()
	{  }
	ServiceProvider(const ServiceProvider &) = delete;
	ServiceProvider& operator=(const ServiceProvider &) = delete;

private:
	static ServiceProvider* instance;
	std::unordered_map<const char *, std::shared_ptr<void>> services;
};
