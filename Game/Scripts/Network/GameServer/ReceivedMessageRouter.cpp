#include "ReceivedMessageRouter.h"

#include <Engine/Application/Logger.h>

void ReceivedMessageRouter::initialize() {
}

void ReceivedMessageRouter::register_handler(Proto::ToClientMessage::MessageCase messageCase, MessageHandler handler) {
	handlers.emplace(messageCase, std::move(handler));
}

void ReceivedMessageRouter::dispatch(const std::vector<Proto::ToClientMessage>& packets) {
	for (const auto& packet : packets) {
		szgInformation("Recv packet byte size long {}", packet.ByteSizeLong());

		if (handlers.contains(packet.message_case())) {
			handlers[packet.message_case()](packet);
		}
		else {
			szgError("Unknown packet category.");
		}
	}
}
