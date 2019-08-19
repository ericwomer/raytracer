//
// Created by Rake on 8/18/2019.
//

#ifndef RAYTRACE_MOVING_SPHERE_H
#define RAYTRACE_MOVING_SPHERE_H

#include "hitable.h"
#include "materials/material.h"

class moving_sphere: public hitable {
public:
    moving_sphere();
    moving_sphere(Vec3 cen0, Vec3 cen1, double t0, double t1, double r, Material* m)
    : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};

    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
    Vec3 center(double time) const;

    Vec3 center0, center1;
    double time0, time1;
    double radius;
    Material* mat_ptr;
};

#endif //RAYTRACE_MOVING_SPHERE_H
