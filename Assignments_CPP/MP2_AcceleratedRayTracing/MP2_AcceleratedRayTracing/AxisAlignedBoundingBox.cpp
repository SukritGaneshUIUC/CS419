#include "AxisAlignedBoundingBox.h"

/*
* Default constructor for AABB3D
*/
AxisAlignedBoundingBox::AxisAlignedBoundingBox() {}

/*
* AABB3D constructor
* 
* @param a The minimum point
* @param b The maximum point
*/
AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Point3D& a, const Point3D& b) : minimum(a), maximum(b) {}

/*
* @eturn The mimimum point
*/
Point3D& AxisAlignedBoundingBox::min()
{
    return minimum;
}

/*
* @return The maximum point
*/
Point3D& AxisAlignedBoundingBox::max()
{
    return maximum;
}

/*
* @return The mimimum point
*/
const Point3D& AxisAlignedBoundingBox::min() const
{
    return minimum;
}

/*
* @return The maximum point
*/
const Point3D& AxisAlignedBoundingBox::max() const
{
    return maximum;
}

/*
* Checks whether a ray intersects the AABB3D
* 
* @param r The ray
* @param t_min Mimimum intersection t-value
* @param t_max Maximum intersection t-value
* 
* @return True if r intersects this, false otherwise
*/
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

/*
* @return AABB3D in pretty string format
*/
std::string AxisAlignedBoundingBox::toString() const
{
    return "Min: [" + minimum.toString() + "] | Max: [" + maximum.toString() + "]";
}