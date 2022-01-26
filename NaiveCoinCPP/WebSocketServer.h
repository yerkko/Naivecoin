#pragma once
#include <restinio/all.hpp>
#include <restinio/websocket/websocket.hpp>
#include <fmt/format.h>

#include <memory>
using router_t = restinio::router::express_router_t<>;
namespace rws = restinio::websocket::basic;
using router_t = restinio::router::express_router_t<>;

using traits_t =
restinio::traits_t<
	restinio::asio_timer_manager_t,
	restinio::single_threaded_ostream_logger_t,
	router_t >;

using ws_registry_t = std::map< std::uint64_t, rws::ws_handle_t >;

class WebSocketServer
{

public:
    WebSocketServer(const unsigned int port);

    std::unique_ptr<router_t> create_server_handler(ws_registry_t& registry);

};
