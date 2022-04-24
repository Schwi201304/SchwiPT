#include<core/schwi.h>
#include<image/image.h>
#include<math/matrix.h>
#include<samplers/sampler.h>
#include<samplers/random.h>
#include<cameras/camera.h>
#include<cameras/perspective.h>
#include<core/scene.h>
#include<integrators/integrator.h>
#include<integrators/path.h>
#include<ImageManager.h>

using namespace schwi;
using namespace std;

constexpr int width = 400;
constexpr int height = 400;

constexpr int spp = 100;
constexpr int maxDepth = 10;

int main() {


	SchwiImage img(width, height);

	unique_ptr<Sampler> originalSampler = make_unique<RandomSampler>(spp);

	unique_ptr<Camera> camera=make_unique<PerspectiveCamera>(
		Point3d{ 50, 52, -295.6 },
		Vector3d{ 0, -0.042612, 1 }.Normalize(),
		Vector3d{ 0, 1, 0 },
		53, 
		Vector2i(width,height));

	auto scene = Scene::CreateSmallPTScene();

	unique_ptr<Integrator> integrator = make_unique<PathIntegrator>(10);
	integrator->Render(scene, *camera, *originalSampler, img);

	img.write_file("out.png");

#if defined(_WIN32) || defined(_WIN64)
	system("mspaint out.png");
#endif

	return 0;
}