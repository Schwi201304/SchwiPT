#pragma once

#include<random>
#include"vector2.h"
#include"vector3.h"

namespace schwi {
	class RNG {
	public:
		RNG(int seed=std::chrono::system_clock::now().time_since_epoch().count())
			:rngEngine(seed){}

		double UniformDouble() {
			return doubleDist(rngEngine);
		}

		int UniformInt() {
			return intDist(rngEngine);
		}

		uint32_t UniformUInt() {
			return uintDist(rngEngine);
		}

		Vector2d UniformVector2d() {
			return Vector2d(UniformDouble(), UniformDouble());
		}

		Vector3d UniformVector3d() {
			return Vector3d(UniformDouble(), UniformDouble(), UniformDouble());
		}

	private:
		std::mt19937_64 rngEngine;
		std::uniform_real_distribution<double> doubleDist{ 0., 1. };
		std::uniform_int_distribution<int> intDist;
		std::uniform_int_distribution<uint32_t> uintDist;
	};
}