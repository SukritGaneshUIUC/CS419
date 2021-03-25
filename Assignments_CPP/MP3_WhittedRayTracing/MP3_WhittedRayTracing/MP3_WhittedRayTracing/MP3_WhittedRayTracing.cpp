#include <iostream>
#include <filesystem>
#include <fstream>
#include <cassert>
#include <cmath>

#include "World.h"
#include "TriangleMesh.h"

#define PI 3.14159265

// IMPLEMENT INTERSECTION FOR SPHERE
// AND THEN IMPLEMENT FOR OTHERS

//void testBench() {
//    std::cout << "Hello World!\n";
//
//    Vec3D x{ 1,2,3 };
//    std::cout << x.toString() << std::endl;
//
//    Ray3D y{ x, Vec3D{3,4,5} };
//    std::cout << y.toString() << std::endl;
//
//    Sphere sph({ 3,3,3 }, 2, WHITE_COLOR);
//    Ray3D r{ {0,0,0}, {3,3,3} };
//    std::vector<Point3D> intPoints;
//    sph.intersection(r, intPoints);
//    for (const Point3D& p : intPoints) {
//        std::cout << p.toString() << std::endl;
//    }
//
//    Ray3D r2({ 0,0,0 }, { 2,2,2 });
//    PointLightSource pl({ 4,4,4 });
//    intPoints.clear();
//    std::cout << pl.intersection(r2, intPoints) << std::endl;
//
//    Plane p({ 1,0,0 }, { 1,2,-0.5 }, WHITE_COLOR);
//    Ray3D r3({ 0,0,0 }, { 2,6,7 });
//    intPoints.clear();
//    p.intersection(r3, intPoints);
//    std::cout << "Plane intersect point: " << intPoints[0].toString() << std::endl;
//
//    Plane p2({ 0,-2,0 }, { 0,1,0 }, WHITE_COLOR);
//    Ray3D r4({ 0,0,0 }, { 0, -0.5, -1 });
//    intPoints.clear();
//    p2.intersection(r4, intPoints);
//    std::cout << "Does plane intersect point: " << intPoints.size() << std::endl;
//
//    // File Test
//    std::string testImageFilepath = "testImage1.ppm";
//    int testRows = 300;
//    int testCols = 400;
//    Image testImage{ testRows, testCols };
//    for (int i = 0; i < testRows; i++) {
//        for (int j = 0; j < testCols; j++) {
//            if (i < 100) {
//                testImage.set(i, j, ColorRGB(255, 0, 0));
//            }
//            else if (i < 200) {
//                testImage.set(i, j, ColorRGB(0, 255, 0));
//            }
//            else {
//                testImage.set(i, j, ColorRGB(0, 0, 255));
//            }
//        }
//    }
//    testImage.writeToFile(testImageFilepath);
//
//}

void perspectiveTest() {
    // SETUP WORLD
    World world;
    int rows = 500;
    int cols = 500;
    double width = 2.0;  // higher value = zoom out, lower value = zoom in

    Camera camera;
    camera.setPosition({ 0,0,0 });
    camera.setViewWindowPosition(Point3D(0, 0, -1));
    camera.setUpVector(Vec3D(0, 1, 0));
    camera.setViewWindowRows(rows);
    camera.setViewWindowCols(cols);
    camera.setPixelSize(width / rows);
    camera.setProjectionType(ProjectionType::PERSPECTIVE);
    camera.setWorldPosition({ 0,0,0 });
    world.setCamera(camera);

    Image backgroundImage{ rows, cols, ColorRGB(255, 219, 247) };
    world.setBackgroundImage(std::move(backgroundImage));
    world.setAmbientLight(WHITE_COLOR * 0.2);

    // BUILD WORLD
    world.addSceneObject(std::shared_ptr<Object>(new Plane({ 0,-2,0 }, { 0,1,0 }, Material(BLUE_COLOR, BLUE_COLOR))));
    world.addSceneObject(std::shared_ptr<Object>(new Plane({ 6,0,0 }, { -1,0,0 }, Material(GREEN_COLOR, GREEN_COLOR))));
    world.addSceneObject(std::shared_ptr<Object>(new Plane({ 0,0,-13 }, { 0,0,1 }, Material(YELLOW_COLOR, YELLOW_COLOR))));
    world.addSceneObject(std::shared_ptr<Object>(new Sphere(Point3D(1, 1, -7), 3, Material(RED_COLOR, RED_COLOR))));
    world.addSceneObject(std::shared_ptr<Object>(new Sphere(Point3D(-2, -1.5, -3), 0.5, Material(PINK_COLOR, PINK_COLOR))));
    world.addSceneObject(std::shared_ptr<Object>(new Triangle({ Point3D(0, -2, -3.5), Point3D(2.5, -0.5, -3.0), Point3D(2.0, -2, -3.0) }, Material(ORANGE_COLOR, ORANGE_COLOR))));

    world.addLightSource(std::shared_ptr<Object>(new PointLightSource(Point3D(-12, 20, 2), Material(WHITE_COLOR, WHITE_COLOR))));

    // RENDER
    std::cout << "Rendering ..." << std::endl;
    Image im{ world.render() };
    std::cout << "Done rendering ..." << std::endl;
    std::string filepath = "perspective_test.ppm";
    im.writeToFile(filepath);
}

void sphereTest(int sphereCount = 10) {
    // SETUP WORLD
    World world;
    int rows = 500;
    int cols = 500;
    double width = 3.0;  // higher value = zoom out, lower value = zoom in

    Camera camera;
    camera.setPosition({ 0,0,0 });
    camera.setViewWindowPosition(Point3D(0, 0, -1));
    camera.setUpVector(Vec3D(0, 1, 0));
    camera.setViewWindowRows(rows);
    camera.setViewWindowCols(cols);
    camera.setPixelSize(width / rows);
    camera.setProjectionType(ProjectionType::PERSPECTIVE);
    camera.setWorldPosition({ 0,0,0 });
    world.setCamera(camera);

    Image backgroundImage{ rows, cols, ColorRGB(255, 219, 247) };
    world.setBackgroundImage(std::move(backgroundImage));
    world.setAmbientLight(WHITE_COLOR * 0.2);

    // BUILD WORLD
    Point3D lowerLimit{ -10, -10, -10 };
    Point3D upperLimit{ 10, 10, -5 };
    double sphereRadius = 0.2;

    for (size_t i = 0; i < sphereCount; i++) {
        Point3D randomCenter = Arithmetic::randomVec3D(lowerLimit, upperLimit);
        Sphere* s = new Sphere(randomCenter, sphereRadius, Material(Arithmetic::randomVec3D(BLACK_COLOR, WHITE_COLOR), Arithmetic::randomVec3D(BLACK_COLOR, WHITE_COLOR), Arithmetic::randomVec3D(BLACK_COLOR, WHITE_COLOR)));
        world.addSceneObject(std::shared_ptr<Object>(s));
    }

    world.addLightSource(std::shared_ptr<Object>(new PointLightSource(Point3D(-12, 20, 2), Material(WHITE_COLOR, WHITE_COLOR))));

    world.addRenderOption(RenderOption::BVH);

    // RENDER
    std::cout << "Rendering ...\n" << std::endl;
    Image im{ world.render() };
    std::cout << "\nDone rendering ..." << std::endl << std::endl;
    std::string filepath = "spheres" + std::to_string(sphereCount) + ".ppm";
    im.writeToFile(filepath);
}

void objTest(const std::string& objFilepath) {
    // SETUP WORLD    
    World world;
    int rows = 500;
    int cols = 500;
    double width = 1.0;  // higher value = zoom out, lower value = zoom in

    Camera camera;
    camera.setPosition({ 0,0,0 });
    camera.setViewWindowPosition(Point3D(0, 0, -1));
    camera.setUpVector(Vec3D(0, 1, 0));
    camera.setViewWindowRows(rows);
    camera.setViewWindowCols(cols);
    camera.setPixelSize(width / rows);
    camera.setProjectionType(ProjectionType::PERSPECTIVE);
    camera.setWorldPosition({ 0,2,8 });
    world.setCamera(camera);

    Image backgroundImage{ rows, cols, YELLOW_COLOR };
    world.setBackgroundImage(std::move(backgroundImage));
    world.setAmbientLight(WHITE_COLOR * 0.2);

    // BUILD WORLD
    std::shared_ptr<TriangleMesh> tm{ new TriangleMesh };
    tm->loadFromOBJFile(objFilepath);
    world.setTriangleMesh(tm);

    world.addLightSource(std::shared_ptr<Object>(new PointLightSource(Point3D(-12, -30, 12), Material(WHITE_COLOR, WHITE_COLOR))));

    world.addRenderOption(RenderOption::TRIANGLE_MESH);

    // RENDER
    std::cout << "Rendering ..." << std::endl;
    Image im{ world.render() };
    std::cout << "Done rendering ..." << std::endl;
    std::string filepath = objFilepath.substr(0, objFilepath.size() - 4) + "Test.ppm";
    im.writeToFile(filepath);
}

void intTest() {
    std::vector<Point3D> intPoints;
    AABB3D bb(Point3D(-5, -5, -5), Point3D(5, 5, -10));
    Ray3D rr(Point3D(0, 0, 0), Vec3D(0, 0, -1));
    bool hh = bb.hit(rr, 0, 10000);
    std::cout << hh << std::endl;
}

int main()
{
    //testBench();
    perspectiveTest();
    //singleSphereTest(3);

    //sphereTest(5);
    //sphereTest(25);
    sphereTest(100);
    //sphereTest(1000);
    //sphereTest(10000);
    //sphereTest(100000);

    objTest("teapotObj.txt");
    //objTest("dragonObj.txt");


}