#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <utility>
#include <functional>
#include <algorithm>

#include "Vec3D.h"
#include "Ray3D.h"

#define MOLLER_TRUMBORE
#define CULLING

namespace Arithmetic {
    const double EPSILON = 0.00001;

    static int quadratic_solver(const double& a, const double& b, const double& c, std::vector<double>& sols) {
        double discriminant = b * b - 4.0 * a * c;

        if (discriminant < 0.0) {
            return 0;
        }
        else if (discriminant == 0.0) {
            sols.push_back((-1.0 * b) / (2.0 * a));
            return 1;
        }
        else {
            sols.push_back(((-1.0 * b) + std::sqrt(discriminant)) / (2.0 * a));
            sols.push_back(((-1.0 * b) - std::sqrt(discriminant)) / (2.0 * a));
            return 2;
        }
    };

    static int ray_intersect_sphere() {
        return 0;
    };

    static bool vecs_parallel(const Vec3D& v1, const Vec3D& v2) {
        double scale1 = v1[0] / v2[0];
        double scale2 = v1[1] / v2[1];
        double scale3 = v1[2] / v2[2];
        return (abs(scale1 - scale2) < EPSILON && abs(scale2 - scale3) < EPSILON && abs(scale1 - scale3) < EPSILON);
    }

    static bool ray_intersect_point(const Ray3D& ray, const Point3D& point) {
        Vec3D dir{ point - ray.getStart() };
        return vecs_parallel(dir, ray.getDirection());
    }

    // Intersection between triangle and ray
    // Credit to the geniuses at wikipedia
    static bool moller_trumbore(const Point3D& rayOrigin,
        const Vec3D& rayVector,
        const Point3D(&inTriangle)[3],
        Point3D& outIntersectionPoint)
    {
        //const double EPSILON = 0.0000001;
        Vec3D vertex0 = inTriangle[0];
        Vec3D vertex1 = inTriangle[1];
        Vec3D vertex2 = inTriangle[2];
        Vec3D edge1, edge2, h, s, q;
        double a, f, u, v;
        edge1 = vertex1 - vertex0;
        edge2 = vertex2 - vertex0;
        h = rayVector.crossProduct(edge2);
        a = edge1.dotProduct(h);
        if (a > -EPSILON && a < EPSILON)
            return false;    // This ray is parallel to this triangle.
        f = 1.0 / a;
        s = rayOrigin - vertex0;
        u = f * s.dotProduct(h);
        if (u < 0.0 || u > 1.0)
            return false;
        q = s.crossProduct(edge1);
        v = f * rayVector.dotProduct(q);
        if (v < 0.0 || u + v > 1.0)
            return false;
        // At this stage we can compute t to find out where the intersection point is on the line.
        double t = f * edge2.dotProduct(q);
        if (t > EPSILON) // ray intersection
        {
            outIntersectionPoint = rayOrigin + rayVector * t;
            return true;
        }
        else // This means that there is a line intersection but not a ray intersection.
            return false;
    }

    // credits to the geniuses at scratchapixel
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
    static bool ray_intersect_triangle(
        const Vec3D& orig, const Vec3D& dir,
        const Point3D& v0, const Point3D& v1, const Point3D& v2,
        double& t, double& u, double& v)
    {
        #ifdef MOLLER_TRUMBORE 
                Vec3D v0v1 = v1 - v0;
                Vec3D v0v2 = v2 - v0;
                Vec3D pvec = dir.crossProduct(v0v2);
                double det = v0v1.dotProduct(pvec);
        #ifdef CULLING 
                // if the determinant is negative the triangle is backfacing
                // if the determinant is close to 0, the ray misses the triangle
                if (det < EPSILON) return false;
        #else 
                // ray and triangle are parallel if det is close to 0
                if (fabs(det) < EPSILON) return false;
        #endif 
                double invDet = 1 / det;

                Vec3D tvec = orig - v0;
                u = tvec.dotProduct(pvec) * invDet;
                if (u < 0 || u > 1) return false;

                Vec3D qvec = tvec.crossProduct(v0v1);
                v = dir.dotProduct(qvec) * invDet;
                if (v < 0 || u + v > 1) return false;

                t = v0v2.dotProduct(qvec) * invDet;

                return true;
        #else 
                ...
        #endif 
    }

    // credit to the geniuses at scratchpixel
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
    static bool ray_intersect_plane(const Point3D& planePoint, const Vec3D& planeNormal, const Ray3D& ray, Point3D& intPoint)
    {
        //// normalize vectors
        //const Point3D& p0 = planePoint;
        //const Vec3D n = planeNormal.get_normalized(2);  // 2-norm
        //const Point3D& l0 = ray.getStart();
        //const Vec3D l = ray.getDirection().get_normalized(2);   // 2-norm

        // normalize vectors
        const Point3D& p0 = planePoint;
        const Vec3D& n = planeNormal;
        const Point3D& l0 = ray.getStart();
        const Vec3D& l = ray.getDirection();

        // assuming vectors are all normalized
        double denom = n.dotProduct(l);
        // if n dot l is below EPSILON, we treat it as having no intersection
        if (abs(denom) >= EPSILON) {
            Vec3D p0l0 = p0 - l0;
            double intersectionT = (p0l0.dotProduct(n)) / denom;
            if (intersectionT >= EPSILON) {
                Ray3D temp{ l0, l };    // the ray, used to check intersection
                intPoint.setPoint(temp.pos(intersectionT));     // calculate intersection point from intersectionT
                return true;
            }
        }
        return false;
    }

    // find the closest point in the vector "pts" to the starting point "start"
    // default euclidean, set dist_l=1 for manhattan distance
    // returns the index to that point
    static int closestPoint(const Point3D& start, const std::vector<Point3D>& pts, int dist_l=2) {
        if (pts.size() == 0) {
            return -1;
        }

        int closestPointIdx = 0;
        double shortestDistance = (pts[closestPointIdx] - start).euclideanSquared();
        for (int i = 0; i < pts.size(); i++) {
            const Point3D& p = pts[i];
            double d = (p - start).euclideanSquared();
            if (d < shortestDistance) {
                shortestDistance = d;
                closestPointIdx = i;
            }
        }
        return closestPointIdx;
    }

    static Vec3D averageVec3D(const std::vector<Vec3D>& vecs) {
        Vec3D sum;
        for (const Vec3D& v : vecs) {
            sum += v;
        }
        return sum / ((double)vecs.size());
    }

    static void range(int start, int end, std::vector<int>& rangeVec) {
        for (int i = start; i < end; i++) {
            rangeVec.push_back(i);
        }
    }

    static void multi_jittered_sampling(const int& division_factor, std::vector<std::pair<double, double>>& samples) {
        auto rng = std::default_random_engine{};

        // keep track of coarse grid rows
        std::vector<int> coarse;
        double coarse_grid_size = 1.0 / division_factor;

        // keep track of fine grid rows
        // 2d vector: each element is a vector containing rows of a coarse grid row
        int fineGridDivisions = division_factor * division_factor;
        std::vector<std::vector<int>> fine;
        std::vector<int> currFineVec;
        for (int row = 0; row < fineGridDivisions + 1; row++) {
            if (currFineVec.size() >= division_factor) {
                std::vector ee = currFineVec;
                std::shuffle(std::begin(ee), std::end(ee), rng);
                fine.push_back(ee);
                currFineVec.clear();
            }
            currFineVec.push_back(row);
        }

        double fine_grid_size = coarse_grid_size / division_factor;
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution(0, fine_grid_size);

        // perform multi jittered sampling
        // go column by column
        // and pick a remaining fine-grid row which is in a different coarse grid row for the current coarse grid column
        for (int col = 0; col < fineGridDivisions; col++) {
            // refill coarse vector if we have moved to a new coarse column
            if (coarse.size() == 0) {
                range(0, division_factor, coarse);
                std::shuffle(std::begin(coarse), std::end(coarse), rng);
            }

            // pick a coarse grid row
            int cgr = coarse.back();
            coarse.pop_back();

            // pick a fine grid row
            int fgr = fine[cgr].back();
            fine[cgr].pop_back();

            // generate a random point within the current fine grid
            double x = fine_grid_size * fgr + distribution(generator);
            double y = fine_grid_size * col + distribution(generator);
            samples.push_back(std::make_pair(x, y));
        }
    }

};
