#include "HTTPServer.h"
#include "Block.h"
#include "nlohmann/json.hpp"
#include <exception>



template<typename T>
std::ostream& operator<<(std::ostream& to, const optional_t<T>& v) {
	if (v) to << *v;
	return to;
}

template < typename RESP >
RESP
init_resp(RESP resp)
{
	resp.append_header("Server", "RESTinio sample server /v.0.4");
	resp.append_header_date_field()
		.append_header("Content-Type", "text/plain; charset=utf-8");

	return resp;
}

static const request_handling_status_t handleBadRequest(const restinio::request_handle_t& req)
{
	init_resp(req->create_response())
		.append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
		.set_body("{data: \"data error\"}")
		.done();
	return restinio::request_rejected();
}

HTTPServer::HTTPServer(const unsigned int port)
{
	using traits_t =
		restinio::traits_t<
		restinio::asio_timer_manager_t,
		restinio::single_threaded_ostream_logger_t,
		router_t >;

	restinio::run(
		restinio::on_thread_pool<traits_t>(2)
		.port(port)
		.address("localhost")
		.request_handler(this->create_server_handler()));
}

std::unique_ptr<router_t> HTTPServer::create_server_handler()
{
	auto router = std::make_unique< router_t >();

	router->http_get(
		"/blocks",
		[](auto req, auto) {

			std::string str = "[";
			const auto& blockchain = getBlockchain();
			const auto size = blockchain.size();

			for (size_t i = 0; i < size; ++i) {
				str += blockchain.at(i).Serialize();
				if (i < size - 1) str += ',';
			}
			str += "]";

			init_resp(req->create_response())
				.append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
				.set_body(fmt::format("{}", str))
				.done();

			return restinio::request_accepted();
		});

	router->http_post(
		"/mineBlock",
		[](const restinio::request_handle_t& req, auto) {

			const auto& body = req.get()->body();
			try {
				nlohmann::json j = nlohmann::json::parse(body);
				if (j.contains("data")) {
					const std::string& data = j["data"].get<std::string>();
					const auto& newBlock = generateNewBlock(data);

					init_resp(req->create_response())
						.append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
						.set_body(fmt::format("{}", newBlock.Serialize()))
						.done();
					return restinio::request_accepted();
				}
				else {
					return handleBadRequest(req);
				}
			}

		catch (std::exception& e) {
			return handleBadRequest(req);
			}
		}
	);

	router->http_get(
		"/html",
		[](auto req, auto) {
			init_resp(req->create_response())
				.append_header(restinio::http_field::content_type, "text/html; charset=utf-8")
				.set_body(
					"<html>\r\n"
					"  <head><title>Hello from RESTinio!</title></head>\r\n"
					"  <body>\r\n"
					"    <center><h1>Hello world</h1></center>\r\n"
					"  </body>\r\n"
					"</html>\r\n")
				.done();

			return restinio::request_accepted();
		});


	return router;
}


