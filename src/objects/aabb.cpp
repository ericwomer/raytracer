//
// Created by rake on 8/21/19.
//

#include "aabb.h"

aabb surrounding_box(aabb box0, aabb box1)
{
    Vec3 small(fmin(box0.min().x(), box1.min().x()),
               fmin(box0.min().x(), box1.min().x()),
               fmin(box0.min().z(), box1.min().z()));
    Vec3 big(fmax(box0.max().x(), box1.max().x()),
             fmax(box0.max().y(), box1.max().y()),
             fmax(box0.max().z(), box1.max().z()));

    return aabb(small,big);
}