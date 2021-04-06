#include <iostream>
#include <filesystem>
#include <fstream>
#include <cassert>
#include <cmath>

#include "World.h"
#include "SolidMaterial.h"
#include "Mirror.h"
#include "Dielectric.h"

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

    world.addSceneObject(std::shared_ptr<Object>(new Plane({ 0,-2,0 }, { 0,1,0 }, std::make_shared<Material>(Material(BLUE_COLOR, BLUE_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Plane({ 6,0,0 }, { -1,0,0 }, std::make_shared<Material>(Material(GREEN_COLOR, GREEN_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Plane({ 0,0,-13 }, { 0,0,1 }, std::make_shared<Material>(Material(YELLOW_COLOR, YELLOW_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Sphere(Point3D(1, 1, -7), 3, std::make_shared<Material>(Material(RED_COLOR, RED_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Sphere(Point3D(-2, -1.5, -3), 0.5, std::make_shared<Material>(Material(PINK_COLOR, PINK_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Triangle({ Point3D(0, -2, -3.5), Point3D(2.5, -0.5, -3.0), Point3D(2.0, -2, -3.0) }, std::make_shared<Material>(Material(ORANGE_COLOR, ORANGE_COLOR)))));
    
    world.addLightSource(std::shared_ptr<AreaLightSource>(new AreaLightSource({ Point3D(-12, 20, 2), Point3D(-10, 20, 2), Point3D(-12, 22, 1) }, std::make_shared<Material>(Material(WHITE_COLOR, WHITE_COLOR, WHITE_COLOR)))));

    // RENDER
    std::cout << "Rendering ..." << std::endl;
    Image im{ world.render() };
    std::cout << "Done rendering ..." << std::endl;
    std::string filepath = "perspective_test.ppm";
    im.writeToFile(filepath);
}

void areaLightTest() {
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

    world.addSceneObject(std::shared_ptr<Object>(new Plane({ 0,0,-15 }, { 0,0,-1 }, std::make_shared<Material>(SolidMaterial(BLUE_COLOR, BLUE_COLOR, WHITE_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Plane({ 0,-2,0 }, { 0,1,0 }, std::make_shared<Material>(SolidMaterial(YELLOW_COLOR, YELLOW_COLOR, WHITE_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Sphere(Point3D(-3, -1, -7), 1, std::make_shared<Material>(SolidMaterial(RED_COLOR, RED_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Sphere(Point3D(-0.5, -1, -7), 1, std::make_shared<Material>(SolidMaterial(RED_COLOR, RED_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Sphere(Point3D(2, -1, -7), 1, std::make_shared<Material>(SolidMaterial(RED_COLOR, RED_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Triangle({ Point3D(3, -2, -5), Point3D(5, -2, -7), Point3D(4, 0, -7) }, std::make_shared<Material>(SolidMaterial(ORANGE_COLOR, ORANGE_COLOR)))));

    world.addLightSource(std::shared_ptr<AreaLightSource>(new AreaLightSource({ Point3D(-1, -2, -11), Point3D(2, -2, -11), Point3D(0.5, 2, -11) }, std::make_shared<Material>(SolidMaterial(WHITE_COLOR, WHITE_COLOR, WHITE_COLOR)))));
    //world.addRenderOption(RenderOption::ANTI_ALIASING);
    // RENDER
    std::cout << "Rendering ..." << std::endl;
    Image im{ world.render() };
    std::cout << "Done rendering ..." << std::endl;
    std::string filepath = "area_light_test.ppm";
    im.writeToFile(filepath);
}

void reflectionTest() {
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

    world.addSceneObject(std::shared_ptr<Object>(new Plane({ 0,0,-15 }, { 0,0,-1 }, std::make_shared<Material>(SolidMaterial(BLUE_COLOR, BLUE_COLOR, WHITE_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Plane({ 0,-2,0 }, { 0,1,0 }, std::make_shared<Material>(SolidMaterial(YELLOW_COLOR, YELLOW_COLOR, WHITE_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Plane({ 0,0,-15 }, { 1,0,1 }, std::make_shared<Material>(SolidMaterial(GRAY_COLOR, GRAY_COLOR, WHITE_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Triangle({ Point3D{ -3, -2, -5 }, Point3D{0, -2, -8}, Point3D{-1.5, 1, -6.5} }, std::make_shared<Material>(Mirror()))));
    world.addSceneObject(std::shared_ptr<Object>(new Sphere(Point3D(4, -1, -7), 1, std::make_shared<Material>(SolidMaterial(PINK_COLOR, PINK_COLOR)))));

    world.addLightSource(std::shared_ptr<AreaLightSource>(new AreaLightSource({ Point3D(-12, 20, 2), Point3D(-10, 20, 2), Point3D(-12, 22, 1) }, std::make_shared<Material>(SolidMaterial(WHITE_COLOR, WHITE_COLOR, WHITE_COLOR)))));
    //world.addRenderOption(RenderOption::ANTI_ALIASING);
    // RENDER
    std::cout << "Rendering ..." << std::endl;
    Image im{ world.render() };
    std::cout << "Done rendering ..." << std::endl;
    std::string filepath = "reflection_test.ppm";
    im.writeToFile(filepath);
}

void dielectricTest() {
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

    world.addSceneObject(std::shared_ptr<Object>(new Plane({ 0,0,-15 }, { 0,0,1 }, std::make_shared<Material>(SolidMaterial(BLUE_COLOR, BLUE_COLOR, WHITE_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Plane({ 0,-2,0 }, { 0,1,0 }, std::make_shared<Material>(SolidMaterial(YELLOW_COLOR, YELLOW_COLOR, WHITE_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Plane({ 0,0,25 }, { 0,0,-1 }, std::make_shared<Material>(SolidMaterial(ORANGE_COLOR, ORANGE_COLOR, WHITE_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Triangle({ Point3D{ -7, -2, -9 }, Point3D{1, -2, -7}, Point3D{1, 4, -7} }, std::make_shared<Material>(Mirror()))));
    world.addSceneObject(std::shared_ptr<Object>(new Triangle({ Point3D{ -7, -2, -8 }, Point3D{-4, -2, -6.5}, Point3D{-4, 2, -6.5} }, std::make_shared<Material>(SolidMaterial(PINK_COLOR, PINK_COLOR, WHITE_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Triangle({ Point3D{ -1, -2, -4 }, Point3D{1, -2, -4}, Point3D{0, -2, -6} }, std::make_shared<Material>(SolidMaterial(RED_COLOR, RED_COLOR, WHITE_COLOR)))));
    world.addSceneObject(std::shared_ptr<Object>(new Sphere(Point3D(0, -0.5, -5), 1.5, std::make_shared<Material>(Dielectric(PINK_COLOR, PINK_COLOR)))));

    world.addLightSource(std::shared_ptr<AreaLightSource>(new AreaLightSource({ Point3D(-12, 20, 2), Point3D(-10, 20, 2), Point3D(-12, 22, 1) }, std::make_shared<Material>(SolidMaterial(WHITE_COLOR, WHITE_COLOR, WHITE_COLOR)))));

    // RENDER
    std::cout << "Rendering ..." << std::endl;
    Image im{ world.render() };
    std::cout << "Done rendering ..." << std::endl;
    std::string filepath = "dielectric_test.ppm";
    im.writeToFile(filepath);
}

int main()
{
    //testBench();
    //perspectiveTest();
    areaLightTest();
    //reflectionTest();
    //dielectricTest();
}