#pragma once

#include "Object.h"
#include "Arithmetic.h"
#include "SceneObject.h"

class BVHNode :
    public Object
{
private:
    std::shared_ptr<Object> left;
    std::shared_ptr<Object> right;
    AABB3D box;
    bool leaf;
public:
    BVHNode();
    BVHNode(const std::vector<std::shared_ptr<SceneObject>>& list);
    BVHNode(const std::vector<std::shared_ptr<Object>> list);
    BVHNode(const std::vector<std::shared_ptr<Object>>& src_objects, size_t start, size_t end);

    const bool& isLeaf() const;
    const std::shared_ptr<Object>& getLeft() const;
    const std::shared_ptr<Object>& getRight() const;
    const AABB3D& getBoundingBox() const;

    bool generateBoundingBox(AABB3D& output_box) const;

    AABB3D surroundingBox(const AABB3D& box0, const AABB3D& box1) const;

    bool box_compare(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b, int axis) const;
    bool box_x_compare(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) const;
    bool box_y_compare(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) const;
    bool box_z_compare(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) const;

    int intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<double>& intTs) const;
    int intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<Point3D>& intPoints) const;

    // JUST TO COMPLY
    const ColorRGB& getAmbient() const;
    const ColorRGB& getDiffuse() const;
    const ColorRGB& getSpecular() const;
    const double& getAlpha() const;

    Vec3D normal(const Point3D& intersection) const;

};

















//bool box_compare(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b, int axis) const
//{
//    AABB3D box_a;
//    AABB3D box_b;

//    if (!a->generateBoundingBox(box_a) || !b->generateBoundingBox(box_b))
//        std::cerr << "No bounding box in bvh_node constructor.\n";

//    return box_a.min()[axis] < box_b.min()[axis];
//}

//bool box_x_compare(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) const
//{
//    return box_compare(a, b, 0);
//}

//bool box_y_compare(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) const
//{
//    return box_compare(a, b, 1);
//}

//bool box_z_compare(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) const
//{
//    return box_compare(a, b, 2);
//}



















































