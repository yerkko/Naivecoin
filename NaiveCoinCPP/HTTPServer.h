#pragma once
#include <restinio/all.hpp>

#include <memory>
using namespace restinio;
using RestServer = restinio::running_server_handle_t<restinio::default_traits_t>;

using router_t = restinio::router::express_router_t<>;



class HTTPServer
{

public:
	HTTPServer(const unsigned int port);

	std::unique_ptr<router_t> create_server_handler();

};

