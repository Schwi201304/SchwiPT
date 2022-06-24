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
#include<integrators/directlight.h>
#include<core/film.h>
#include<filter/gaussian.h>

using namespace schwi;
using namespace std;

constexpr int width = 800;
constexpr int height = 800;

constexpr int spp = 10;
constexpr int maxDepth = 10;

int main() {
	Film film(Point2i(width, height), make_unique<GaussianFilter>(Vector2d(1, 1), 1), "out.png");

	unique_ptr<Sampler> originalSampler = make_unique<RandomSampler>(spp);

	unique_ptr<Camera> camera = make_unique<PerspectiveCamera>(
		Point3d{ 0, 0, 80 },
		Vector3d{ 0, 0, -1 },
		Vector3d{ 0, 1, 0 },
		120,
		Vector2i(width, height));

	//Scene scene = Scene::CreatHeadScene();
	Scene scene = Scene::CreatCornellBox();
	//Scene scene = Scene::LightSample();
	//Scene scene = Scene::SkinTestScene();

	//unique_ptr<Integrator> integrator = make_unique<SimplePathTracingRecursion>(maxDepth, DirectSampleEnum::BothMis);
	unique_ptr<Integrator> integrator = make_unique<PathTracingRecursion>(maxDepth, DirectSampleEnum::BothMis, LightingEnum::AllLighting);
	//unique_ptr<Integrator> integrator = make_unique<DebugIntegrator>(1);
	//unique_ptr<Integrator> integrator = make_unique<DirectLightIntegrator>(DirectSampleEnum::BothMis);
	//unique_ptr<Integrator> integrator = make_unique<SkinIntegrator>();
	//unique_ptr<Integrator> integrator = make_unique<ScreenSpaceSSSIntegrator>();

	integrator->Render(scene, *camera, *originalSampler, film);

	//ScreenSpaceSSSIntegrator::SSSSS(scene, *camera, *originalSampler, film);

	film.WriteImage();
	//Film film_filter = film.Filter();
	//film_filter.WriteImage();

#if defined(_WIN32) || defined(_WIN64)
	system("mspaint out.png");
#endif
	return 0;
}