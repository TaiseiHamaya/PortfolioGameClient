#include "ProtoBufManager.h"

#include "./Scripts/Network/NetworkCluster.h"

void ProtoBufManager::Allocate() {
	GetInstance();
}

ProtoBufManager::OnDestroy::~OnDestroy() {
	NetworkCluster::Finalize();
	google::protobuf::ShutdownProtobufLibrary();
}

