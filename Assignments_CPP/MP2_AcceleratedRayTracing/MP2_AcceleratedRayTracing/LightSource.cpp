#include "LightSource.h"

/*
* Constructor for LightSource
*
* @param type The ObjectType of the LightSource
* @param diffuse The diffuse color of the LightSource
* @param specular The specular color of the LightSource
* @param brightness The brightness of the LightSource
*/
LightSource::LightSource(const ObjectType& type, const ColorRGB& diffuse, const ColorRGB& specular, const double& brightness) : Object(type), diffuse(diffuse), specular(specular), brightness(brightness) {}

/*
* @return The diffuse color of the LightSource
*/
const ColorRGB& LightSource::getDiffuse() const
{
    return diffuse;
}

/*
* @return The sepcular color of the LightSource
*/
const ColorRGB& LightSource::getSpecular() const
{
    return specular;
}

/*
* @return The brightness of the LightSource
*/
const double& LightSource::getBrightness() const
{
    return brightness;
}
