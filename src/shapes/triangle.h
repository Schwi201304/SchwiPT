#pragma once

#include<shapes/shape.h>
#include<textures/texture.h>

namespace schwi {
	struct TriangleMesh {
        TriangleMesh(const Frame& ObjectToWorld, int nTriangles,
            const int* vertexIndices, int nVertices, const Point3d* P,
            const Vector3d* S, const Normal3d* N, const Point2d* uv,
            const std::shared_ptr<Texture<double>>& alphaMask,
            const std::shared_ptr<Texture<double>>& shadowAlphaMask,
            const int* faceIndices);

	};
}