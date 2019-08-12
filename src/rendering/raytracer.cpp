#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

#include <thread>
#include <future>

#include "raytracer.h"
#include "ray.h"
// Vec3 sample(

struct sample_s {
  Vec3&     _col;
  int       _i, _j;
  camera*   _cam;
  Vec2<int> _res;
  hitable*  _world;

  sample_s(sample_s const& other)
      : _i(other._i)
      , _j(other._j)
      , _cam(other._cam)
      , _res(other._res)
      , _col(other._col)
      , _world(other._world)

  {
  }
  sample_s(int i, int j, camera* cam, Vec2<int>& res, hitable* world, Vec3& col)
      : _i(i)
      , _j(j)
      , _cam(cam)
      , _res(res)
      , _col(col)
      , _world(world)

  {
  }

  Vec3 operator()(const tbb::blocked_range<size_t>& range) const
  {
    std::cout << "Begin: " << range.begin() << " End: " << range.end() << "\n";
    for (size_t s = range.begin(); s != range.end(); ++s) {
      std::cout << s << "\n";
      double u = double(_i + drand48()) / double(_res.x());
      double v = double(_j + drand48()) / double(_res.y());
      ray    r = _cam->get_ray(u, v);
      _col += color(r, _world, 0);
    }
  }
};

// Eric: Rewrite this so it renders to the screen instead of blotting to it. like, takes in an
// object that can write the pixels to the screen after the sample bit maybe.
std::stringstream raytracer::renderPPM(camera* cam, Vec2<int> res, int samples, std::vector<hitable*> elements)
{
  std::stringstream buf;
  buf << "P3\n" << res.x() << " " << res.y() << "\n255\n";
  hitable* world     = new hitable_list(elements.data(), elements.size());
  size_t   grainsize = 10;

  for (int j = res.y() - 1; j >= 0; j--) {
    for (int i = 0; i < res.x(); i++) {
      Vec3 col(0, 0, 0);

      /*
      struct sample_s sample {
        i, j, cam, res, world, col
      };

      tbb::parallel_for(tbb::blocked_range<size_t>(0, samples), sample);
      */

      for (int s = 0; s < samples; s++) {
        double u = double(i + drand48()) / double(res.x());
        double v = double(j + drand48()) / double(res.y());
        ray    r = cam->get_ray(u, v);
        col += color(r, world, 0);
      }

      col /= double(samples);
      col    = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
      int ir = int(255.99 * col[0]);
      int ig = int(255.99 * col[1]);
      int ib = int(255.99 * col[2]);
      buf << ir << " " << ig << " " << ib << "\n";
    }
  }
  return buf;
}

std::vector<Pixel_t> raytracer::render(camera* cam, Vec2<int> res, int samples, std::vector<hitable*> elements)
{
  hitable* world = new hitable_list(elements.data(), elements.size());

  // pixelrgba_t data[res.y()][res.x()];
  std::vector<Pixel_t> pixels;
  pixels.resize(res.x() * res.y());

  for (int j = res.y() - 1; j >= 0; j--) {
    for (int i = 0; i < res.x(); i++) {
      Vec3 col(0, 0, 0);
      for (int s = 0; s < samples; s++) {
        double u = double(i + drand48()) / double(res.x());
        double v = double(j + drand48()) / double(res.y());
        ray    r = cam->get_ray(u, v);
        // Vec3 p = r.point_at_parameter(2.0);
        col += color(r, world, 0);
      }
      col /= double(samples);
      col        = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
      uint8_t ir = uint8_t(255.99 * col[0]);
      uint8_t ig = uint8_t(255.99 * col[1]);
      uint8_t ib = uint8_t(255.99 * col[2]);
      uint8_t ia = uint8_t(255.99 * 1);  // Eric: leve this as the default for now

      pixels[(res.y() - 1 - j) * res.x() + i].pixelData.r = ir;
      pixels[(res.y() - 1 - j) * res.x() + i].pixelData.g = ig;
      pixels[(res.y() - 1 - j) * res.x() + i].pixelData.b = ib;
      pixels[(res.y() - 1 - j) * res.x() + i].pixelData.a = ia;
    }
  }
  return pixels;
}
