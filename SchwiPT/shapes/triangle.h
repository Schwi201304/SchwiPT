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
			Shape(frame),mesh(mesh){
			v = &mesh->vertexIndices[3 * triNum];
			faceIndex = mesh->faceIndices.size() ? mesh->faceIndices[triNum] : 0;
		}

		bool Intersect(const Ray& r, Intersection* out_isect)const {
			Ray ray = frame->ToLocal(ray);

			const Point3d& A = mesh->p[v[0]];
			const Point3d& B = mesh->p[v[1]];
			const Point3d& C = mesh->p[v[2]];

			Vector3d E1 = B - A;
			Vector3d E2 = C - A;
			Vector3d P = Cross(ray.direction(), E2);
			double det = Dot(E1, P);

			//back facing
			//if(det<0)
			if (det < epsilon)return false;
			if (std::abs(det) < epsilon)return false;

			double invDet = 1 / det;
			Vector3d T = ray.origin() - A;
			double beta = Dot(T, P) * invDet;
			if (beta < 0 || beta>1)return false;

			Vector3d Q = Cross(T, E1);
			double gamma = Dot(ray.direction(), Q) * invDet;
			if (gamma < 0 || gamma>1)return false;

			double t = Dot(E2, Q) * invDet;
			r.set_distance(t);
			Point3d hit_point = ray(t);
			Normal3d normal = Normal3d(Cross(E1, E2));

			double alpha = 1 - beta - gamma;
			Point2d uv= mesh->uv[v[0]] * alpha + mesh->uv[v[1]] * beta + mesh->uv[v[2]] * gamma;
			*out_isect = Intersection(
				frame->ToWorld(hit_point),
				frame->ToWorld(normal),
				-r.direction(),
				uv);
			return true;
		}

	private:
		void GetUVs(Point2d uv[3])const {
			if (mesh->uv) {
				uv[0] = mesh->uv[v[0]];
				uv[1] = mesh->uv[v[1]];
				uv[2] = mesh->uv[v[2]];
			}
			else {
				uv[0] = Point2d(0, 0);
				uv[1] = Point2d(1, 0);
				uv[2] = Point2d(1, 1);
			}
		}

		Point2d Barycentric(double alpha,double beta,double gamma) {
			
		}
	};
}