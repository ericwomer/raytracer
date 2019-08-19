#if !defined(MATERIAL_H)
#define MATERIAL_H

#include "objects/hitable.h"
#include "rendering/ray.h"
#include "types/vec3.h"

inline double schlick(double cosine, double ref_idx)
{
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 + r0) * pow((1 - cosine), 5);
}

inline Vec3 reflect(const Vec3& v, const Vec3& n)
{
    return v - 2 * dot(v, n) * n;
}

inline bool refract(const Vec3& v, const Vec3& n, double ni_over_nt, Vec3& refracted)
{
    Vec3   uv = unit_vector(v);
    double dt = dot(uv, n);
    double discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0) {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    } else {
        return false;
    }
}

inline Vec3 random_in_unit_sphere()
{
    Vec3 p;
    do {
        p = 2.0 * Vec3(double(rand()) / RAND_MAX, double(rand()) / RAND_MAX, double(rand()) / RAND_MAX) - Vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);
    return p;
}

class Material {
public:
    virtual ~Material(){};
    virtual bool scatter(const ray& r_in, const hit_record& rec, Vec3& attenuation, ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
    Lambertian(const Vec3& a)
        : albedo(a)
    {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, Vec3& attenuation, ray& scattered) const
    {
        Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p, r_in.time());
        attenuation = albedo;
        return true;
    }
    Vec3 albedo;
};

class Metal : public Material {
public:
    Metal(const Vec3& a, double f)
        : albedo(a)
    {
        if (f < 1) {
            fuzz = f;
        } else {
            fuzz = 1;
        }
    }
    virtual ~Metal(){};
    virtual bool scatter(const ray& r_in, const hit_record& rec, Vec3& attenuation, ray& scattered) const
    {
        Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    Vec3   albedo;
    double fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(double ri)
        : ref_idx(ri)
    {}
    virtual ~Dielectric(){};
    virtual bool scatter(const ray& r_in, const hit_record& rec, Vec3& attenuation, ray& scattered) const
    {
        Vec3   outward_normal;
        Vec3   reflected = reflect(r_in.direction(), rec.normal);
        double ni_over_nt;
        attenuation = Vec3(1.0, 1.0, 1.0);
        Vec3   refracted;
        double reflect_prob;
        double cosine;
        if (dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
        } else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }

        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
            reflect_prob = schlick(cosine, ref_idx);
        } else {
            scattered = ray(rec.p, reflected);
            reflect_prob = 1.0;
        }
        if (double(rand()) / RAND_MAX < reflect_prob) {
            scattered = ray(rec.p, reflected);
        } else {
            scattered = ray(rec.p, refracted);
        }
        return true;
    }

    double ref_idx;
};

#endif  // MATERIAL_H