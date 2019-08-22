#include "sphere.h"

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    Vec3   oc = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = dot(oc, r.direction());
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b * b - a * c;
    if (discriminant > 0) {
        double temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;  // / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

bool sphere::bounding_box(double t0, double t1, aabb &box) const
{
    box = aabb(center - Vec3(radius, radius, radius), center + Vec3(radius, radius, radius));
    return true;
}