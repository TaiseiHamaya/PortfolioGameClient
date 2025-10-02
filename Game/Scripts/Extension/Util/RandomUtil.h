#pragma once

#include <Library/Utility/Tools/RandomEngine.h>

#include <Library/Math/Vector3.h>

inline Vector3 RandomOnSphere() {
	float cosTheta = -2.0f * RandomEngine::Random01Closed() + 1.0f;
	float sinTheta = std::sqrt(1 - cosTheta * cosTheta);
	float phi = PI2 * RandomEngine::Random01Closed();
	return { sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta };
}