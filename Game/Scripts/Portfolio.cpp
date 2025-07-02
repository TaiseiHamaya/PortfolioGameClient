#include "Portfolio.h"

#include <Engine/Runtime/Scene/SceneManager.h>

#include "./Scene/FactoryPortfolio.h"

#include <Engine/Application/Output.h>

#include <asio/read.hpp>

Portfolio::Portfolio() : socket_v4(context, asio::ip::tcp::v4()){
}

void Portfolio::initialize() {
	SceneManager::Setup(std::make_unique<FactoryPortfolio>());

	endpoint = { asio::ip::make_address_v4("127.0.0.1"), 3215 };
	socket_v4.connect(endpoint);

	std::string recv;
	recv.resize(1024);
	asio::read(socket_v4, asio::buffer(recv.data(), recv.size()));

	Information("Portfolio initialized. Received: {}", recv);
}

void Portfolio::finalize() {
	socket_v4.close();
	context.stop();
	Framework::finalize();
}
