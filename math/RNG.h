#pragma once
#include<random>
#include<vector3.h>
namespace schwi {
	class RNG {
	public:
		RNG(int seed=1234):rngEngine(seed){}

		double UniformDouble() {
			return double01Dist(rngEngine);
		}

		Vector3d UniformVector3d() {
			return Vector3d(UniformDouble(), UniformDouble(), UniformDouble());
		}

	private:
		std::mt19937_64 rngEngine;
		std::uniform_real_distribution<double> double01Dist{ 0., 1. };
	};
}