#include <silicon/api.hh>
#include <silicon/backends/mhd.hh>
#include <silicon/middlewares/mysql_connection.hh>
#include <silicon/middlewares/mysql_orm.hh>
#include "symbols.hh"
#include <cstdio>

#include "Services/LoggingService.hpp"
#include "Services/ServiceProvider.hpp"
#include "ServiceInitializer.h"

using namespace sl; // Silicon namespace
using namespace s; // Symbols namespace

#ifdef USE_DB
typedef mysql_orm_factory<User> user_orm_factory;
typedef mysql_orm_factory<Multimedia> multimedia_orm_factory;
typedef mysql_orm_factory<Task> task_orm_factory;
typedef mysql_orm_factory<Message> message_orm_factory;
typedef mysql_orm_factory<Chat> chat_orm_factory;
typedef mysql_orm_factory<Status> status_orm_factory;
typedef mysql_orm_factory<Event> event_orm_factory;
#endif

int main()
{
	setbuf(stdout, NULL);

	ServiceInitializer::InitializeServices();

	// Define the API:
	auto apis = ServiceInitializer::GetApis();

	auto logger = ServiceProvider::Instance().Resolve<LoggingService>();

	logger->Info("Loading middlewares...");

	auto middlewares = std::make_tuple(
		mysql_connection_factory("localhost", "kurendo", "kurendo", "party_organizer")
#ifdef USE_DB
		,user_orm_factory("Users"),
		multimedia_orm_factory("Multimedias"),
		task_orm_factory("Tasks"),
		message_orm_factory("Messages"),
		chat_orm_factory("Chats"),
		status_orm_factory("Statuses"),
		event_orm_factory("Events")
#endif
	);

	logger->Info("Server started.");
	// Serve hello_api via microhttpd using the json format:
	sl::mhd_json_serve(apis, middlewares, 12345);
}
