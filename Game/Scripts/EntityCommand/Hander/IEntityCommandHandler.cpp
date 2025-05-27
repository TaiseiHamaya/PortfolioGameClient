#include "IEntityCommandHandler.h"

void IEntityCommandHandler::initialize(Reference<IEntity> receiver_) {
	receiver = receiver_;
}

void IEntityCommandHandler::run() {
	for (auto& func : commandEntries) {
		func();
	}

	commandEntries.clear();
}

void IEntityCommandHandler::entry_command(u64 commandIndex) {
	if (receiver && commandIndex < commands.size()) {
		commandEntries.emplace_back(
			[&, &command = commands[commandIndex]]() { command->execute(receiver); }
		);
	}
}
