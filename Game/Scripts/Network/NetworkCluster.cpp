#include "NetworkCluster.h"

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

#ifdef DEBUG_FEATURES_ENABLE
#include <imgui.h>
#include <imgui_stdlib.h>
void NetworkCluster::debug_gui() {
	connectionManager.debug_gui();

	bool send = false;
	send |= ImGui::InputText("Chat", &msg, ImGuiInputTextFlags_EnterReturnsTrue);
	send |= ImGui::Button("SendMessage");
	if (send) {
		Proto::Packet packet;
		packet.set_textmessagetype(Proto::TextMessageType::MessageChatSend);
		Proto::ChatMessageBody body;
		body.set_message(msg);
		body.set_userid(0);
		packet.set_payload(body.SerializeAsString());

		packetSender.stack_packet(packet);
		msg.clear();
	}
	ImGui::End();
}
#endif // DEBUG_FEATURES_ENABLE