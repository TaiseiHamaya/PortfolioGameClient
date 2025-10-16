#include "EnvironmentMeshExecutor.h"

#include <Engine/Assets/PolygonMesh/PolygonMeshLibrary.h>
#include <Engine/Assets/Texture/TextureAsset.h>
#include <Engine/Assets/Texture/TextureLibrary.h>
#include <Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h>
#include <Engine/Module/DrawExecutor/LightingExecutor/DirectionalLightingExecutor.h>
#include <Engine/Module/World/Camera/Camera3D.h>

EnvironmentMeshExecutor::EnvironmentMeshExecutor(std::string meshName, u32 maxInstance, std::string environmentTextureName) {
	reinitialize(PolygonMeshLibrary::GetPolygonMesh(meshName), maxInstance, TextureLibrary::GetTexture(environmentTextureName));
}

void EnvironmentMeshExecutor::reinitialize(std::shared_ptr<const PolygonMesh> mesh_, u32 maxInstance_, std::shared_ptr<const TextureAsset> environmentTexture_) {
	asset = mesh_;
	maxInstance = maxInstance_;
	environmentTexture = environmentTexture_;
	matrices.initialize(maxInstance);
	materials.resize(asset->material_count());
	for (StructuredBuffer<MaterialDataBuffer3>& material : materials) {
		material.initialize(maxInstance);
	}
}

void EnvironmentMeshExecutor::setup(Reference<const DirectionalLightingExecutor> directionalLightingExecutor_, Reference<const Camera3D> camera_) {
	directionalLightingExecutor = directionalLightingExecutor_;
	camera = camera_;
}

void EnvironmentMeshExecutor::draw_command() const {
	if (!asset || instanceCounter == 0) {
		return;
	}
	if (!directionalLightingExecutor) {
		return;
	}
	auto& commandList = DxCommand::GetCommandList();
	for (u32 i = 0; i < asset->material_count(); ++i) {
		commandList->IASetVertexBuffers(0, 1, &asset->get_vbv(i));
		commandList->IASetIndexBuffer(asset->get_p_ibv(i));
		commandList->SetGraphicsRootDescriptorTable(0, matrices.get_handle_gpu());
		commandList->SetGraphicsRootDescriptorTable(1, materials[i].get_handle_gpu());
		camera->register_world_projection(2);
		camera->register_world_lighting(3);
		directionalLightingExecutor->set_command(4);
		commandList->SetGraphicsRootDescriptorTable(5, environmentTexture->handle());

		commandList->DrawIndexedInstanced(asset->index_size(i), instanceCounter, 0, 0, 0);
	}
}

void EnvironmentMeshExecutor::write_to_buffer(Reference<const StaticMeshInstance> instance) {
	if (!instance || !instance->is_draw()) {
		return;
	}
	u32 next = instanceCounter;
	++instanceCounter;
	if (next >= maxInstance) {
		return;
	}
	const Affine& local = instance->local_affine();
	const Affine& world = instance->world_affine();
	Affine transformAffine = local * world;
	matrices[next] = {
		.world = transformAffine,
		.itWorld = transformAffine.inverse().get_basis().transposed()
	};
	const std::vector<IMultiMeshInstance::Material>& instanceMaterials = instance->get_materials();
	const size_t numMaterial = asset->material_count();
	for (u32 i = 0; i < numMaterial; ++i) {
		const IMultiMeshInstance::Material& source = instanceMaterials[i];
		materials[i][next] = {
			source.color,
			source.lightingType,
			source.shininess,
			source.texture->index(),
			source.uvTransform.get_matrix(),
		};
	}
}
