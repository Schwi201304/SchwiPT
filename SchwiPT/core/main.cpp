#include<core/schwi.h>
#include<math/matrix.h>
#include<samplers/sampler.h>
#include<samplers/random.h>
#include<cameras/camera.h>
#include<cameras/perspective.h>
#include<core/scene.h>
#include<integrators/integrator.h>
#include<integrators/path.h>
#include<integrators/debug.h>

using namespace schwi;
using namespace std;

constexpr int width = 500;
constexpr int height = 500;

constexpr int spp = 100;
constexpr int maxDepth = 10;

int main() {


	SchwiImage img(width, height);

	unique_ptr<Sampler> originalSampler = make_unique<RandomSampler>(spp);

	unique_ptr<Camera> camera = make_unique<PerspectiveCamera>(
		Point3d{ 0, 0, 200 },
		Vector3d{ 0, 0, -1 },
		Vector3d{ 0, 1, 0 },
		53,
		Vector2i(width, height));

	auto scene = Scene::CreatCornellBox();

	unique_ptr<Integrator> integrator = make_unique<PathIntegrator>(maxDepth);
	//unique_ptr<Integrator> integrator = make_unique<DebugIntegrator>(1);
	integrator->Render(scene, *camera, *originalSampler, img);

	img.write_file("out.png", false);

#if defined(_WIN32) || defined(_WIN64)
	system("mspaint out.png");
#endif
	return 0;
}