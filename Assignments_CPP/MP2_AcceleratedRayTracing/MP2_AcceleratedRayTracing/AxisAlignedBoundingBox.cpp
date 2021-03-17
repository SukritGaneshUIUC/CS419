#include "AxisAlignedBoundingBox.h"

AxisAlignedBoundingBox::AxisAlignedBoundingBox() {}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Point3D& a, const Point3D& b) : minimum(a), maximum(b) {}

Point3D& AxisAlignedBoundingBox::min()
{
    return minimum;
}

Point3D& AxisAlignedBoundingBox::max()
{
    return maximum;
}

const Point3D& AxisAlignedBoundingBox::min() const
{
    return minimum;
}

const Point3D& AxisAlignedBoundingBox::max() const
{
    return maximum;
}

bool AxisAlignedBoundingBox::hit(const Ray3D& r, double t_min, double t_max) const
{
    for (int a = 0; a < 3; a++) {
        auto t0 = fmin((minimum[a] - r.getStart()[a]) / r.getDirection()[a],
            (maximum[a] - r.getStart()[a]) / r.getDirection()[a]);
        auto t1 = fmax((minimum[a] - r.getStart()[a]) / r.getDirection()[a],
            (maximum[a] - r.getStart()[a]) / r.getDirection()[a]);
        t_min = fmax(t0, t_min);
        t_max = fmin(t1, t_max);
        if (t_max <= t_min)
            return false;
    }
    return true;
}

std::string AxisAlignedBoundingBox::toString() const
{
    return "Min: [" + minimum.toString() + "] | Max: [" + maximum.toString() + "]";
}

//inline bool AxisAlignedBoundingBox::hit(const Ray3D& r, double t_min, double t_max) const {
//    for (int a = 0; a < 3; a++) {
//        auto invD = 1.0f / r.getDirection()[a];
//        auto t0 = (min()[a] - r.getStart()[a]) * invD;
//        auto t1 = (max()[a] - r.getStart()[a]) * invD;
//        if (invD < 0.0f)
//            std::swap(t0, t1);
//        t_min = t0 > t_min ? t0 : t_min;
//        t_max = t1 < t_max ? t1 : t_max;
//        if (t_max <= t_min)
//            return false;
//    }
//    return true;
//}