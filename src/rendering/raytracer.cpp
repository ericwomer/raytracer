#include <tbb/parallel_for.h>
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
    Render_t* _render;
    rendering_s(Render_t* render)
            : _render(render)
    {}

    void operator()(const tbb::blocked_range3d<size_t>& range) const {
        for (size_t j = range.rows().begin(); j != range.rows().end(); j++) {
            for (size_t i = range.cols().begin(); i != range.cols().end(); i++) {
                Vec3 col(0, 0, 0);
                // Single threaded rendering.
                for (size_t s = range.pages().begin(); s != range.pages().end(); s++) {
                    double u = double(i + double(rand()) / RAND_MAX) / double(_render->res.x());
                    double v = double(j + double(rand()) / RAND_MAX) / double(_render->res.y());
                    ray r = _render->cam->get_ray(u, v);
                    // Vec3 p = r.point_at_parameter(2.0);
                    col += color(r, _render->world, 0);
                    // std::cout << "s:" << s << " u: " << u << " v: " << v << "\n";
                }

                col /= double(_render->samples);
                col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
                uint8_t ir = uint8_t(255.99 * col[0]);
                uint8_t ig = uint8_t(255.99 * col[1]);
                uint8_t ib = uint8_t(255.99 * col[2]);
                uint8_t ia = uint8_t(255.99 * 1);  // Eric: leave this as the default for now

                // _data->pixels[(_resolution.x() - 1 - j) * _resolution.y() + i].pixelData.r = ir;
                // (1 + j - _resolution.x()) * _resolution.y() - i
                _render->pixels[(_render->res.y() - j) * _render->res.x() + i].pixelData.r = ir;
                _render->pixels[(_render->res.y() - j) * _render->res.x() + i].pixelData.g = ig;
                _render->pixels[(_render->res.y() - j) * _render->res.x() + i].pixelData.b = ib;
                _render->pixels[(_render->res.y() - j) * _render->res.x() + i].pixelData.a = ia;
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
std::vector<Pixel_t> raytracer::render(Render_t render)
{
    // pixelrgba_t data[res.y()][res.x()];

    // render_data_s render_data;
    render.pixels.resize(render.res.x() * render.res.y());
    std::vector<Pixel_t> pixels;
    pixels.resize(render.res.x() * render.res.y());

#if defined(USING_TBB)
    if (render.threaded) {
        rendering_s rendering(&render);
        tbb::parallel_for(tbb::blocked_range3d<size_t>(0, render.samples, render.grains, 0, render.res.y() + 1, render.grains, 0, render.res.x() + 1, render.grains), rendering, tbb::simple_partitioner());

        //struct sample_s sample(i, j, cam, res, world, col);
        //tbb::parallel_for(tbb::blocked_range<size_t>(0, samples, grains), sample, tbb::simple_partitioner());

    } else { // Single threaded rendering.
#endif // USING_TBB
        for (int j = render.res.y() - 1; j >= 0; j--) {
            for (int i = 0; i < render.res.x(); i++) {
                Vec3 col(0, 0, 0);

                for (int s = 0; s < render.samples; s++) {
                    double u = double(i + double(rand()) / RAND_MAX) / double(render.res.x());
                    double v = double(j + double(rand()) / RAND_MAX) / double(render.res.y());
                    ray r = render.cam->get_ray(u, v);
                    // Vec3 p = r.point_at_parameter(2.0);
                    col += color(r, render.world, 0);
                }

                col /= double(render.samples);
                col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
                uint8_t ir = uint8_t(255.99 * col[0]);
                uint8_t ig = uint8_t(255.99 * col[1]);
                uint8_t ib = uint8_t(255.99 * col[2]);
                uint8_t ia = uint8_t(255.99 * 1);  // Eric: leave this as the default for now

                render.pixels[(render.res.y() - 1 - j) * render.res.x() + i].pixelData.r = ir;
                render.pixels[(render.res.y() - 1 - j) * render.res.x() + i].pixelData.g = ig;
                render.pixels[(render.res.y() - 1 - j) * render.res.x() + i].pixelData.b = ib;
                render.pixels[(render.res.y() - 1 - j) * render.res.x() + i].pixelData.a = ia;
                // writeFile("default.tga", Vec2<int>(640, 480), pixels);
            }
        }
#if defined(USING_TBB)
    }
#endif // USING_TBB
    return render.pixels;
}