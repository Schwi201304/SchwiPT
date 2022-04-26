#pragma once

#include<shapes/shape.h>
#include<textures/texture.h>

namespace schwi {
	struct TriangleMesh {
		TriangleMesh(const Frame& frame, int nTriangles,
			const int* vertexIndices, int nVertices, const Point3d* P,
			const Vector3d* S, const Normal3d* N, const Point2d* UV,
			const std::shared_ptr<Texture<double>>& alphaMask)
			: nTriangles(nTriangles),
			nVertices(nVertices),
			vertexIndices(vertexIndices, vertexIndices + 3 * nTriangles),
			alphaMask(alphaMask) {
			// Transform mesh vertices to world space
			p.reset(new Point3d[nVertices]);
			for (int i = 0; i < nVertices; ++i)
				p[i] = frame.ToWorld(P[i]);

			// Copy _UV_, _N_, and _S_ vertex data, if present
			if (UV) {
				uv.reset(new Point2d[nVertices]);
				memcpy(uv.get(), UV, nVertices * sizeof(Point2d));
			}
			if (N) {
				n.reset(new Normal3d[nVertices]);
				for (int i = 0; i < nVertices; ++i) n[i] = frame.ToWorld(N[i]);
			}
			if (S) {
				s.reset(new Vector3d[nVertices]);
				for (int i = 0; i < nVertices; ++i) s[i] = frame.ToWorld(S[i]);
			}
		}

		const int nTriangles, nVertices;
		std::vector<int> vertexIndices;
		std::unique_ptr<Point3d[]> p;
		std::unique_ptr<Normal3d[]> n;
		std::unique_ptr<Vector3d[]> s;
		std::unique_ptr<Point2d[]> uv;
		std::shared_ptr<Texture<double>> alphaMask, shadowAlphaMask;
		std::vector<int> faceIndices;
	};

	class Triangle :public Shape {
	private:
		std::shared_ptr<TriangleMesh> mesh;
		const int* v;
		int faceIndex;

	public:
		Triangle(const Frame* frame,const std::shared_ptr<TriangleMesh>&mesh,int triNum):
			Shape(){}
	};
}