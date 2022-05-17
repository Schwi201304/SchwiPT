#pragma once

#include<shapes/shape.h>
#include<textures/texture.h>

namespace schwi {
	struct TriangleMesh {
		TriangleMesh(const Frame& frame, int nTriangles,
			const int* vertexIndices, int nVertices, const Point3d* P,
			const Vector3d* S, const Normal3d* N,
			const int* UVIndices,const Point2d* UV,
			const std::shared_ptr<Texture<double>>& alphaMask)
			: nTriangles(nTriangles),
			nVertices(nVertices),
			vertexIndices(vertexIndices, vertexIndices + 3 * nTriangles),
			UVIndices(UVIndices,UVIndices+3*nTriangles),
			alphaMask(alphaMask) {
			// Transform mesh vertices to world space
			p.reset(new Point3d[nVertices]);
			for (int i = 0; i < nVertices; ++i)
				p[i] = frame.ToWorld(P[i])*50.;

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
		std::vector<int> UVIndices;
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
		const int* UV;
		int faceIndex;

	public:
		Triangle(const Frame* frame,const std::shared_ptr<TriangleMesh>&mesh,int triNum):
			Shape(frame),mesh(mesh){
			v = &mesh->vertexIndices[3 * triNum];
			UV = &mesh->UVIndices[3 * triNum];
			faceIndex = mesh->faceIndices.size() ? mesh->faceIndices[triNum] : 0;
		}

		virtual bool Intersect(const Ray& r, SurfaceIntersection* out_isect)const override{
			Ray ray = frame->ToLocal(r);
			
			const Point3d& A = mesh->p[v[0]];
			const Point3d& B = mesh->p[v[1]];
			const Point3d& C = mesh->p[v[2]];
			//std::cout << A<<B<<C << std::endl;

			Vector3d E1 = B - A;
			Vector3d E2 = C - A;
			Vector3d S1 = Cross(ray.direction(), E2);
			double det = Dot(E1, S1);

			if (det < epsilon)return false;
			if (std::abs(det) < epsilon)return false;

			double invDet = 1 / det;
			Vector3d S = ray.origin() - A;
			double beta = Dot(S, S1) * invDet;
			if (beta < 0 || beta>1)return false;

			Vector3d S2 = Cross(S, E1);
			double gamma = Dot(ray.direction(), S2) * invDet;
			if (gamma < 0 || gamma>1)return false;

			double t = Dot(E2, S2) * invDet;
			if (t >= r.distance())return false;
			r.set_distance(t);
			Point3d hit_point = r(t);
			Normal3d normal = Normal3d(Cross(E1, E2));

			double alpha = 1 - beta - gamma;
			if (alpha < 0 || alpha>1)return false;
			//std::cout << alpha << std::endl;
			Point2d uv= mesh->uv[UV[0]] * alpha + mesh->uv[UV[1]] * beta + mesh->uv[UV[2]] * gamma;
			*out_isect = SurfaceIntersection(
				frame->ToWorld(hit_point),
				frame->ToWorld(normal),
				-r.direction(),
				uv,this);
			//std::cout << t << std::endl;
			return true;
		}

		virtual Bounds3d WorldBound()const override {
			Bounds3d bound(mesh->p[v[0]], mesh->p[v[1]]);
			return Union(bound,mesh->p[v[2]]);
		}

		virtual double Area()const override {
			return Cross(mesh->p[v[1]]-mesh->p[v[0]], mesh->p[v[2]] - mesh->p[v[0]]).Length()/2;
		}
		
		virtual SurfaceIntersection SamplePosition(
			const Vector2d& random, double* pdf
		)const override {
			Point2d b=SampleTriangleUniform(random);

			SurfaceIntersection isect;
			isect.position = b.x * mesh->p[v[0]] 
				+ b.y * mesh->p[v[1]] 
				+ (1 - b.x - b.y) * mesh->p[v[2]];
			isect.normal = Normal3d(frame->normal());

			*pdf = 1 / Area();
			return isect;
		}
	};

	std::vector<std::shared_ptr<Shape>> CreateTriangleMaeh(const ModelSPtr& model,const Frame& frame) {
		std::shared_ptr<TriangleMesh> mesh = std::make_shared<TriangleMesh>(frame,
			model->nfaces(), model->facet_vert.data(), model->nverts(),
			model->vert.data(), nullptr, model->norm.data(),model->facet_tex.data(), model->tex.data(), nullptr);

		std::vector<std::shared_ptr<Shape>> tris;
		int nTriangles = model->nfaces();
		tris.reserve(nTriangles);
		for (int i = 0; i < nTriangles; ++i)
			tris.push_back(std::make_shared<Triangle>(nullptr,mesh, i));
		return tris;
	}
}