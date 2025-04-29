#include "Invoker.h"

void Invoker::run() {
	for (auto& func : commandEntries) {
		func();
	}
}

void Invoker::register_entry(Reference<BaseEntity> receiver, u64 commandIndex) {
	if (receiver && commandIndex < commands.size()) {
		commandEntries.emplace_back(
			[&command = commands[commandIndex], receiver]() { command->execute(receiver); }
		);
	}
}
