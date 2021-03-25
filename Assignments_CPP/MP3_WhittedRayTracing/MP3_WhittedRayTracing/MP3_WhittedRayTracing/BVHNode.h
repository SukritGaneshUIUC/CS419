#pragma once

#include "Object.h"
#include "Arithmetic.h"
#include "TriangleMesh.h"

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
    BVHNode(const std::shared_ptr<TriangleMesh>& triangleMesh);
    BVHNode(const std::vector<std::shared_ptr<Object>> list);
    BVHNode(const std::vector<std::shared_ptr<Object>>& src_objects, size_t start, size_t end);

    const std::shared_ptr<Object>& getLeft() const;
    const std::shared_ptr<Object>& getRight() const;
    const AABB3D& getBoundingBox() const;
    const bool& isLeaf() const;

    bool generateBoundingBox(AABB3D& output_box) const;
    AABB3D surroundingBox(const AABB3D& box0, const AABB3D& box1) const;
    bool box_compare(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b, int axis) const;

    int intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const;

    std::string toStringHelper(const BVHNode* curr, size_t level) const;
    std::string toString() const;

    // JUST TO COMPLY
    const ColorRGB& getAmbient() const;
    const ColorRGB& getDiffuse() const;
    const ColorRGB& getSpecular() const;
    const double& getAlpha() const;

    Vec3D normal(const Point3D& intersection) const;

};
