#include "BVHNode.h"

/*
* Default constructor for BVHNode
*/
BVHNode::BVHNode() : Object(ObjectType::BVHNode) {}

/*
* Constructor for BVHNode
* 
* @param triangleMesh The TriangleMesh with which to build a BVH Tree
*/
BVHNode::BVHNode(const std::shared_ptr<TriangleMesh>& triangleMesh) : Object(ObjectType::BVHNode)
{
    std::vector<std::shared_ptr<Object>> objs;
    for (std::shared_ptr<Triangle> so : triangleMesh->getTriangles()) {
        //std::cout << "Triangle: " << so->toString() << std::endl;
        objs.push_back(so);
    }
    auto box_x_compare = [&](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) -> bool {return box_compare(a, b, 0); };
    std::sort(objs.begin(), objs.end(), box_x_compare);
    std::cout << "Objects to insert into BVH Tree: " << objs.size() << std::endl;
    *this = BVHNode(objs, 0, objs.size());
    std::cout << std::endl << objs.size() << " objects in BVH tree!" << std::endl;
}

/*
* Constructor for BVHNode
* 
* @param list A list of SceneObjects to use to build the BVH Tree
*/
BVHNode::BVHNode(const std::vector<std::shared_ptr<SceneObject>>& list) : Object(ObjectType::BVHNode) 
{
    std::vector<std::shared_ptr<Object>> objs;
    for (std::shared_ptr<SceneObject> so : list) {
        objs.push_back(so);
    }
    auto box_x_compare = [&](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) -> bool {return box_compare(a, b, 0); };
    std::sort(objs.begin(), objs.end(), box_x_compare);
    std::cout << "Objects to insert into BVH: " << objs.size() << std::endl;
    *this = BVHNode(objs, 0, objs.size());
    std::cout << std::endl << objs.size() << " objects in BVH tree!" << std::endl;
}


/*
* Constructor for BVHNode
* 
* @param list A list of Objects to use to build the BVH Tree
*/
BVHNode::BVHNode(const std::vector<std::shared_ptr<Object>> list) : BVHNode(list, 0, list.size()) {}

/*
* Primary Constructor for BVHNode (do not call directly)
* 
* @param src_objects A list of Objects to build the current BVH Node
* @param start The left-bound index of the subset of Objects to use in constructing the current node
* @param end The right-bound index of the subset of Objects to use in constructing the current node
*/
BVHNode::BVHNode(const std::vector<std::shared_ptr<Object>>& src_objects, size_t start, size_t end) : Object(ObjectType::BVHNode) 
{
    leaf = false;

    // Lambda expressions for box compare
    auto box_x_compare = [&](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) -> bool {return box_compare(a, b, 0); };
    auto box_y_compare = [&](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) -> bool {return box_compare(a, b, 1); };
    auto box_z_compare = [&](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) -> bool {return box_compare(a, b, 2); };

    // Create a modifiable array of the source scene objects
    auto objects = src_objects; 

    // Select the axis of division
    int axis = Arithmetic::random_int(0, 2);
    
    // Perform the division
    size_t object_span = end - start;
    if (object_span == 1) {
        left = right = objects[start];
        leaf = true;
    }
    else if (object_span == 2) {
        if (box_compare(objects[start], objects[start + 1], axis)) {
            left = objects[start];
            right = objects[start + 1];
        }
        else {
            left = objects[start + 1];
            right = objects[start];
        }
        leaf = true;
    }
    else {
        //if (axis == 0) {
        //    std::sort(objects.begin() + start, objects.begin() + end, box_x_compare);
        //}
        //else if (axis == 1) {
        //    std::sort(objects.begin() + start, objects.begin() + end, box_y_compare);
        //}
        //else {
        //    std::sort(objects.begin() + start, objects.begin() + end, box_z_compare);
        //}

        leaf = false;

        auto mid = start + object_span / 2;
        left = std::make_shared<BVHNode>(objects, start, mid);
        right = std::make_shared<BVHNode>(objects, mid, end);
    }

    AABB3D box_left, box_right;

    if (!left->generateBoundingBox(box_left)
        || !right->generateBoundingBox(box_right)
        )
        std::cerr << "No bounding box in bvh_node constructor.\n";

    box = surroundingBox(box_left, box_right);
}

/*
* @return The left node
*/
const std::shared_ptr<Object>& BVHNode::getLeft() const
{
    return left;
}

/*
* @return The right node
*/
const std::shared_ptr<Object>& BVHNode::getRight() const
{
    return right;
}

/*
* @return The bounding box of the node
*/
const AABB3D& BVHNode::getBoundingBox() const
{
    return box;
}

/*
* @return True if all the node's children are NOT BVHNodes, False otherwise
*/
const bool& BVHNode::isLeaf() const
{
    return leaf;
}

/*
* Retrieve the bounding box of the BVHNode
* 
* @param output_box The variable to hold the bounding box. Modified by function.
*/
bool BVHNode::generateBoundingBox(AABB3D& output_box) const
{
    output_box = box;
    return true;
}

/*
* Generates a bounding box which surrounds two given bounding boxes
* 
* @param box0 The first bounding box
* @param box1 The second bounding box
* 
* @return A bounding box surrounding box0 and box1
*/
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

/*
* Compares two bounding objects based a selected axis of their bounding boxes; the boxes' minimum points (at a certain axis) are compared
* 
* @param a The first object
* @param b The second object
* @param axis The axis to compare
* 
* @return True if the bounding box of a < bounding box of b, false otherwise
*/
bool BVHNode::box_compare(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b, int axis) const
{
    AABB3D box_a;
    AABB3D box_b;

    if (!a->generateBoundingBox(box_a) || !b->generateBoundingBox(box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    return box_a.min()[axis] < box_b.min()[axis];
}

/*
* Check if a ray intersects with any descendant Objects of the BVHNode
* 
* @param ray The Ray3D.
* @param t_min The minimum intersection t.
* @param t_max The maximum intersection t.
* @param hitRecord A HitRecord struct which will store information related to the intersection (if any). Modified by function.
*/
int BVHNode::intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const
{
    if (!box.hit(ray, t_min, t_max)) {
        return false;
    }

    bool hit_left = left->intersection(ray, t_min, t_max, hitRecord);
    //double tm = t_max;
    //if (hit_left) {
    //    tm = hitRecord.intT;
    //}
    //bool hit_right = right->intersection(ray, t_min, tm, hitRecord);
    bool hit_right = right->intersection(ray, t_min, hit_left ? hitRecord.intT : t_max, hitRecord);

    if (hit_left || hit_right) {
        return 1;
    }
    return 0;
}

/*
* Helper method for recursive toString
* 
* @param curr The current BVHNode
* @param level The current level in the BVH Tree (top is level 0)
* 
* @return the BVHNode curr in a pretty string format
*/
std::string BVHNode::toStringHelper(const BVHNode* curr, size_t level) const
{
    std::string printNode = "Level " + std::to_string(level) + ": " + curr->getBoundingBox().toString() + "\n";
    
    if (curr->isLeaf()) {
        return printNode;
    }

    BVHNode* left = std::dynamic_pointer_cast<BVHNode>(curr->getLeft()).get();
    BVHNode* right = std::dynamic_pointer_cast<BVHNode>(curr->getRight()).get();

    std::string leftString = toStringHelper(left, level + 1);
    std::string rightString = toStringHelper(right, level + 1);

    return printNode + leftString + rightString;
}

/*
* @return the node in a pretty string format
*/
std::string BVHNode::toString() const
{
    std::string bvhtree = "";
    return toStringHelper(this, 0);
}

// NOTE: THESE FUNCTIONS DON'T HAVE ANY USE! THEY'RE SIMPLY TO COMPLY WITH THE PURE VIRTUAL OVERRIDE REQUIREMENTS OF THE PARENT CLASS, OBJECT!

const ColorRGB& BVHNode::getAmbient() const
{
    // Nothing here
    return WHITE_COLOR;
}

const ColorRGB& BVHNode::getDiffuse() const
{
    return WHITE_COLOR;
}

const ColorRGB& BVHNode::getSpecular() const
{
    return WHITE_COLOR;
}

const double& BVHNode::getAlpha() const
{
    return 0;
}

Vec3D BVHNode::normal(const Point3D& intersection) const
{
    return Vec3D(0,0,1);
}