#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

#include "raytracer.h"
#include "ray.h"
// Vec3 sample(

// Rake: Speed this up, or find a better method of parallel computing.
struct sample_s {
    Vec3&     _col;
    int       _i, _j;
    camera*   _cam;
    Vec2<int> _res;
    hitable*  _world;

    sample_s(int i, int j, camera* cam, Vec2<int>& res, hitable* world, Vec3& col)
        : _i(i)
        , _j(j)
        , _cam(cam)
        , _res(res)
        , _col(col)
        , _world(world)
    {}

    void operator()(const tbb::blocked_range<size_t>& range) const
    {
        for (size_t s = range.begin(); s != range.end(); ++s) {
            double u = double(_i + double(rand()) / RAND_MAX) / double(_res.x());
            double v = double(_j + double(rand()) / RAND_MAX) / double(_res.y());
            ray    r = _cam->get_ray(u, v);
            _col += color(r, _world, 0);
        }
    }
};

/**
 *
 * @param cam
 * @param res
 * @param samples
 * @param elements
 * @return
 */
std::vector<Pixel_t> raytracer::render(camera* cam, Vec2<int> res, int samples, std::vector<hitable*> elements, bool threaded, std::size_t grains)
{
    hitable* world = new hitable_list(elements.data(), elements.size());

    // pixelrgba_t data[res.y()][res.x()];
    std::vector<Pixel_t> pixels;
    pixels.resize(res.x() * res.y());

    for (int j = res.y() - 1; j >= 0; j--) {
        for (int i = 0; i < res.x(); i++) {
            Vec3 col(0, 0, 0);

            if (threaded) {
                struct sample_s sample(i, j, cam, res, world, col);
                tbb::parallel_for(tbb::blocked_range<size_t>(0, samples, grains), sample, tbb::simple_partitioner());
            } else {
                // Single threaded rendering.
                for (int s = 0; s < samples; s++) {
                    double u = double(i + double(rand()) / RAND_MAX) / double(res.x());
                    double v = double(j + double(rand()) / RAND_MAX) / double(res.y());
                    ray    r = cam->get_ray(u, v);
                    // Vec3 p = r.point_at_parameter(2.0);
                    col += color(r, world, 0);
                }
            }
            col /= double(samples);
            col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            uint8_t ir = uint8_t(255.99 * col[0]);
            uint8_t ig = uint8_t(255.99 * col[1]);
            uint8_t ib = uint8_t(255.99 * col[2]);
            uint8_t ia = uint8_t(255.99 * 1);  // Eric: leave this as the default for now

            pixels[(res.y() - 1 - j) * res.x() + i].pixelData.r = ir;
            pixels[(res.y() - 1 - j) * res.x() + i].pixelData.g = ig;
            pixels[(res.y() - 1 - j) * res.x() + i].pixelData.b = ib;
            pixels[(res.y() - 1 - j) * res.x() + i].pixelData.a = ia;
        }
    }
    return pixels;
}