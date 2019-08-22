//
// Created by rake on 8/21/19.
//

#ifndef RAYTRACE_BVH_NODE_H
#define RAYTRACE_BVH_NODE_H

#include "hitable.h"

class bvh_node : public hitable {
public:
    bvh_node() {}
    bvh_node(hitable**l, int n, double time0, double time1);
    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
    virtual bool bounding_box(double t0, double t1, aabb& box) const;
    hitable* left;
    hitable* right;
    aabb box;

};


#endif //RAYTRACE_BVH_NODE_H
