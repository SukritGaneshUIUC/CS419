#include "BVHNode.h"

BVHNode::BVHNode() : Object(ObjectType::BVHNode) {}

BVHNode::BVHNode(const std::vector<std::shared_ptr<Object>> list) : BVHNode(list, 0, list.size()) {}

BVHNode::BVHNode(const std::vector<std::shared_ptr<Object>>& src_objects, size_t start, size_t end) : Object(ObjectType::BVHNode) {
    //// Create a modifiable array of the source scene objects
    //auto objects = src_objects; 

    //// Select the axis of division

    //int axis = Arithmetic::random_int(0, 2);

    //bool (BVHNode::*comparator)(const std::shared_ptr<Object>&, const std::shared_ptr<Object>&) const;
    //if (axis == 0) {
    //    comparator = &BVHNode::box_x_compare;
    //}
    //else if (axis == 1) {
    //    comparator = &BVHNode::box_y_compare;
    //}
    //else {
    //    comparator = &BVHNode::box_z_compare;
    //}

    ////auto comparator = (axis == 0) ? box_x_compare
    ////    : (axis == 1) ? box_y_compare
    ////    : box_z_compare;

    //// Perform the division
    //size_t object_span = end - start;
    //if (object_span == 1) {
    //    left = right = objects[start];
    //}
    //else if (object_span == 2) {
    //    if (box_compare(objects[start], objects[start + 1], axis)) {
    //        left = objects[start];
    //        right = objects[start + 1];
    //    }
    //    else {
    //        left = objects[start + 1];
    //        right = objects[start];
    //    }
    //}
    //else {
    //    //std::sort(objects.begin() + start, objects.begin() + end, comparator);

    //    if (axis == 0) {
    //        std::sort(objects.begin() + start, objects.begin() + end, BVHNode::box_x_compare);
    //    }
    //    else if (axis == 1) {
    //        std::sort(objects.begin() + start, objects.begin() + end, BVHNode::box_y_compare);
    //    }
    //    else {
    //        std::sort(objects.begin() + start, objects.begin() + end, BVHNode::box_z_compare);
    //    }

    //    auto mid = start + object_span / 2;
    //    left = std::make_shared<BVHNode>(objects, start, mid);
    //    right = std::make_shared<BVHNode>(objects, mid, end);
    //}

    //AABB3D box_left, box_right;

    //if (!left->generateBoundingBox(box_left)
    //    || !right->generateBoundingBox(box_right)
    //    )
    //    std::cerr << "No bounding box in bvh_node constructor.\n";

    //box = surroundingBox(box_left, box_right);
}

bool BVHNode::generateBoundingBox(AABB3D& output_box) const
{
	output_box = box;
	return true;
}

AABB3D BVHNode::surroundingBox(const AABB3D& box0, const AABB3D& box1) const
{
    Point3D small(fmin(box0.min().x(), box1.min().x()),
        fmin(box0.min().y(), box1.min().y()),
        fmin(box0.min().z(), box1.min().z()));

    Point3D big(fmax(box0.max().x(), box1.max().x()),
        fmax(box0.max().y(), box1.max().y()),
        fmax(box0.max().z(), box1.max().z()));

    return AABB3D(small, big);
}

bool BVHNode::box_compare(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b, int axis) const
{
    AABB3D box_a;
    AABB3D box_b;

    if (!a->generateBoundingBox(box_a) || !b->generateBoundingBox(box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    return box_a.min()[axis] < box_b.min()[axis];
}

bool BVHNode::box_x_compare(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) const
{
    return box_compare(a, b, 0);
}

bool BVHNode::box_y_compare(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) const
{
    return box_compare(a, b, 1);
}

bool BVHNode::box_z_compare(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) const
{
    return box_compare(a, b, 2);
}

//int BVHNode::intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<double>& intTs) const
//{
//	if (!box.hit(ray, t_min, t_max)) {
//		return false;
//	}
//
//    intTs.clear();
//	int hit_left = left->intersection(ray, t_min, t_max, intTs);
//    intTs.clear();
//	int hit_right = right->intersection(ray, t_min, hit_left ? intTs[0] : t_max, intTs);
//    intTs.clear();
//
//	if (hit_left + hit_right > 0) {
//		return 1;
//	}
//	return 0;
//}

int BVHNode::intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<Point3D>& intPoints) const
{
    if (!box.hit(ray, t_min, t_max)) {
        return false;
    }

    intPoints.clear();
    int hit_left = left->intersection(ray, t_min, t_max, intPoints);
    intPoints.clear();
    int hit_right = right->intersection(ray, t_min, hit_left ? ray.getT(intPoints[0]) : t_max, intPoints);
    intPoints.clear();

    if (hit_left + hit_right > 0) {
        return 1;
    }
    return 0;
}