#include "NetworkCluster.h"

void NetworkCluster::Initialize() {
	auto& instance = GetInstance();
	instance.connectionManager.initialize();
	instance.packetReceiver.initialize();
	instance.packetSender.initialize();
}

void NetworkCluster::Setup(const std::string& addr, u16 port) {
	auto& instance = GetInstance();
	instance.connectionManager.setup(addr, port);

	instance.packetReceiver.setup(instance.connectionManager);
	instance.packetSender.setup(instance.connectionManager);
}

void NetworkCluster::Connect() {
	GetInstance().connectionManager.connect();
}

void NetworkCluster::Finalize() {
	auto& instance = GetInstance();

	instance.connectionManager.disconnect();

	instance.packetSender.finalize();
	instance.packetReceiver.finalize();
	instance.connectionManager.finalize();
}

void NetworkCluster::Receive() {
	GetInstance().packetReceiver.read_packets();
}

void NetworkCluster::Send() {
	GetInstance().packetSender.send_all_packets();
}

Reference<GameServerPacketReceiver> NetworkCluster::ReceiverMut() {
	return GetInstance().packetReceiver;
}

Reference<GameServerConnectionManager> NetworkCluster::ConnectionManagerMut() {
	return GetInstance().connectionManager;
}

Reference<GameServerPacketSender> NetworkCluster::SenderMut() {
	return GetInstance().packetSender;
}
