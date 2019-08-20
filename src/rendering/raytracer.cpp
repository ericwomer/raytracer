#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/blocked_range2d.h>
#include <tbb/blocked_range3d.h>

#include "raytracer.h"
#include "ray.h"
#include "system/file.h"

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


struct rendering_s {
    camera* _cam;
    Vec2<int> _resolution;
    int _samples;
    std::vector<hitable*> _elements;
    hitable* _world;
    render_data_s* _data;

    rendering_s(camera* cam, Vec2<int>& res, int samples, std::vector<hitable*> elements, hitable* world, render_data_s* data)
            : _cam(cam), _resolution(res), _samples(samples), _elements(elements), _world(world), _data(data)
    {}

    void operator()(const tbb::blocked_range3d<size_t>& range) const {
        for (size_t j = range.rows().begin(); j != range.rows().end(); j++) {
            for (size_t i = range.cols().begin(); i != range.cols().end(); i++) {
                Vec3 col(0, 0, 0);
                // Single threaded rendering.
                for (size_t s = range.pages().begin(); s != range.pages().end(); s++) {
                    double u = double(i + double(rand()) / RAND_MAX) / double(_resolution.x());
                    double v = double(j + double(rand()) / RAND_MAX) / double(_resolution.y());
                    ray r = _cam->get_ray(u, v);
                    // Vec3 p = r.point_at_parameter(2.0);
                    col += color(r, _world, 0);
                    // std::cout << "s:" << s << " u: " << u << " v: " << v << "\n";
                }

                col /= double(_samples);
                col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
                uint8_t ir = uint8_t(255.99 * col[0]);
                uint8_t ig = uint8_t(255.99 * col[1]);
                uint8_t ib = uint8_t(255.99 * col[2]);
                uint8_t ia = uint8_t(255.99 * 1);  // Eric: leave this as the default for now

                // _data->pixels[(_resolution.x() - 1 - j) * _resolution.y() + i].pixelData.r = ir;
                // (1 + j - _resolution.x()) * _resolution.y() - i
                _data->pixels[(_resolution.y() - j) * _resolution.x() + i].pixelData.r = ir;
                _data->pixels[(_resolution.y() - j) * _resolution.x() + i].pixelData.g = ig;
                _data->pixels[(_resolution.y() - j) * _resolution.x() + i].pixelData.b = ib;
                _data->pixels[(_resolution.y() - j) * _resolution.x() + i].pixelData.a = ia;
                // std::cout << "i: " << i << " j: " << j << "\n";
                // writeFile("default.tga", Vec2<int>(640, 480),_data->pixels);
            }
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

    render_data_s render_data;
    render_data.pixels.resize(res.x() * res.y());
    std::vector<Pixel_t> pixels;
    pixels.resize(res.x() * res.y());

#if defined(USING_TBB)
    if (threaded) {
        rendering_s rendering(cam, res, samples, elements,  world, &render_data);
        tbb::parallel_for(tbb::blocked_range3d<size_t>(0, samples, grains, 0, res.y() + 1, grains, 0, res.x() + 1, grains), rendering, tbb::simple_partitioner());

        //struct sample_s sample(i, j, cam, res, world, col);
        //tbb::parallel_for(tbb::blocked_range<size_t>(0, samples, grains), sample, tbb::simple_partitioner());

    } else { // Single threaded rendering.
#endif // USING_TBB
        for (int j = res.y() - 1; j >= 0; j--) {
            for (int i = 0; i < res.x(); i++) {
                Vec3 col(0, 0, 0);

                for (int s = 0; s < samples; s++) {
                    double u = double(i + double(rand()) / RAND_MAX) / double(res.x());
                    double v = double(j + double(rand()) / RAND_MAX) / double(res.y());
                    ray r = cam->get_ray(u, v);
                    // Vec3 p = r.point_at_parameter(2.0);
                    col += color(r, world, 0);
                }

                col /= double(samples);
                col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
                uint8_t ir = uint8_t(255.99 * col[0]);
                uint8_t ig = uint8_t(255.99 * col[1]);
                uint8_t ib = uint8_t(255.99 * col[2]);
                uint8_t ia = uint8_t(255.99 * 1);  // Eric: leave this as the default for now

                render_data.pixels[(res.y() - 1 - j) * res.x() + i].pixelData.r = ir;
                render_data.pixels[(res.y() - 1 - j) * res.x() + i].pixelData.g = ig;
                render_data.pixels[(res.y() - 1 - j) * res.x() + i].pixelData.b = ib;
                render_data.pixels[(res.y() - 1 - j) * res.x() + i].pixelData.a = ia;
                // writeFile("default.tga", Vec2<int>(640, 480), pixels);
            }
        }
#if defined(USING_TBB)
    }
#endif // USING_TBB
    return render_data.pixels;
}