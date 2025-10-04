#include "NetworkCluster.h"

#include "Scripts/Instance/IEntity/IEntity.h"

void NetworkCluster::initialize() {
	connectionManager.initialize();
	packetReceiver.initialize();
	packetSender.initialize();

}

void NetworkCluster::setup() {
	packetReceiver.setup(connectionManager);
	packetSender.setup(connectionManager);

	connectionManager.connect();
}

void NetworkCluster::finalize() {
	const auto& id = player->server_id();
	if (id.has_value()) {
		Proto::Packet packet;
		packet.set_logoutpackettype(Proto::LogoutPacketType::LogoutRequest);
		Proto::LogoutRequestBody body;
		body.set_id(id.value());
		packet.set_payload(body.SerializeAsString());
		packetSender.stack_packet(packet);
		packetSender.send_all_packets();
	}

	connectionManager.disconnect();

	packetSender.finalize();
	packetReceiver.finalize();
	connectionManager.finalize();
}

void NetworkCluster::receive() {
	packetReceiver.read_packets();

	connectionManager.update();
}

void NetworkCluster::send() {
	packetSender.send_all_packets();
}

Reference<GameServerPacketReceiver> NetworkCluster::get_receiver() {
	return packetReceiver;
}

Reference<GameServerConnectionManager> NetworkCluster::connection_manager() {
	return connectionManager;
}

Reference<GameServerPacketSender> NetworkCluster::get_sender() {
	return packetSender;
}

#ifdef DEBUG_FEATURES_ENABLE
#include <imgui.h>
#include <imgui_stdlib.h>
#include "Scripts/Instance/IEntity/IEntity.h"
void NetworkCluster::debug_gui() {
	connectionManager.debug_gui();

	if (!player) {
		return;
	}

	const auto& id = player->server_id();
	if (!id.has_value()) {
		return;
	}
	ImGui::Separator();

	ImGui::Text(player->name_imu().c_str());

	bool send = false;
	send |= ImGui::InputText("Chat", &msg, ImGuiInputTextFlags_EnterReturnsTrue);
	send |= ImGui::Button("SendMessage");
	if (send) {
		Proto::Packet packet;
		packet.set_textmessagetype(Proto::TextMessageType::MessageChatSend);
		Proto::ChatMessageBody body;
		body.set_message(msg);
		body.set_id(id.value());
		packet.set_payload(body.SerializeAsString());

		packetSender.stack_packet(packet);
		msg.clear();
	}
}
#endif // DEBUG_FEATURES_ENABLE