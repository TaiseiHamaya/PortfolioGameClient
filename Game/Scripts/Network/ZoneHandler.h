#pragma once

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include <Scripts/Proto/types.pb.h>

class GameServerPacketReceiver;
class GameServerConnectionManager;

class ZoneHandler final {
public:
	ZoneHandler() = default;
	~ZoneHandler() = default;

	__CLASS_NON_COPYABLE(ZoneHandler)

public:
	void setup(
		Reference<GameServerPacketReceiver> gameServerPacketReceiver_,
		Reference<GameServerConnectionManager> gameServerConnectionManager_
	);

	void handle_zone();

private:
	void process_text_message(Proto::TextMessageType type, const std::string& payload);
	void process_login_packet(Proto::LoginPacketType type, const std::string& payload);
	void process_logout_packet(Proto::LogoutPacketType type, const std::string& payload);
	void process_sync_packet(Proto::SyncPacketType type, const std::string& payload);

private:
	Reference<GameServerConnectionManager> gameServerConnectionManager;
	Reference<GameServerPacketReceiver> gameServerPacketReceiver;
};
