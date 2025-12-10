#include "NetworkCluster.h"

#include "Scripts/Instance/IEntity/IEntity.h"

void NetworkCluster::prev_update() {
	receive();
}

void NetworkCluster::post_update() {
	send();
}

void NetworkCluster::initialize() {
	connectionManager.initialize();
	packetReceiver.initialize();
	packetSender.initialize();
}

void NetworkCluster::setup(const std::string& userName) {
	packetReceiver.setup(connectionManager);
	packetSender.setup(connectionManager);

	connectionManager.connect();

	Proto::Packet packet;
	packet.set_category_login_message(Proto::CategoryLoginMessage::LOGIN_REQUEST);
	Proto::PayloadLoginRequest body;
	body.set_username(userName);

	packet.set_payload(body.SerializeAsString());
	packetSender.stack_packet(packet);
}

void NetworkCluster::finalize() {
	if (player) {
		const auto& id = player->server_id();
		if (id.has_value()) {
			Proto::Packet packet;
			packet.set_category_logout_message(Proto::CategoryLogoutMessage::LOGOUT_REQUEST);
			Proto::PayloadLogoutRequest body;
			body.set_id(id.value());
			packet.set_payload(body.SerializeAsString());
			packetSender.stack_packet(packet);
			packetSender.send_all_packets();
		}
	}

	connectionManager.disconnect();

	packetSender.finalize();
	packetReceiver.finalize();
	connectionManager.finalize();
}

void NetworkCluster::receive() {
	packetReceiver.read_packets();
}

void NetworkCluster::send() {
	packetSender.send_all_packets();
}

void NetworkCluster::set_player(Reference<IEntity> player_) {
	player = player_;
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

	ImGui::Text(player->name_imm().c_str());

	bool send = false;
	send |= ImGui::InputText("Chat", &msg, ImGuiInputTextFlags_EnterReturnsTrue);
	send |= ImGui::Button("SendMessage");
	if (send) {
		Proto::Packet packet;
		packet.set_category_text_message(Proto::CategoryTextMessage::CHAT_SEND);
		Proto::PayloadTextMessage body;
		body.set_message(msg);
		body.set_id(id.value());
		packet.set_payload(body.SerializeAsString());

		packetSender.stack_packet(packet);
		msg.clear();
	}
}
#endif // DEBUG_FEATURES_ENABLE