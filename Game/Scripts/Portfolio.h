#pragma once

#include <Engine/Application/Framework.h>

#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>

class Portfolio final : public Framework {
public:
	Portfolio();

public:
	void initialize() override;

	void finalize() override;

private:
	asio::io_context context;
	asio::ip::tcp::socket socket_v4;
	asio::ip::tcp::endpoint endpoint;
};
