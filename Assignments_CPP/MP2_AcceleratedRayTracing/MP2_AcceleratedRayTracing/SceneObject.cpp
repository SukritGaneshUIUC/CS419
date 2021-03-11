#include "SceneObject.h"

/*
* Constructor for SceneObject
*
* @param type The ObjectType of the SceneObject
* @param ambient The ambient color of the SceneObject
* @param diffuse The diffuse color of the SceneObject
* @param specular The specular color of the SceneObject
* @param alpha The alpha factor of the SceneObject
*/
SceneObject::SceneObject(const ObjectType& type, const ColorRGB& diffuse, const ColorRGB& ambient, const ColorRGB& specular, const double& alpha) : Object(type), ambient(ambient), diffuse(diffuse), specular(specular), alpha(alpha) {}

/*
* @return The ambient color of the SceneObject
*/
const ColorRGB& SceneObject::getAmbient() const
{
    return ambient;
}

/*
* @return The diffuse color of the SceneObject
*/
const ColorRGB& SceneObject::getDiffuse() const
{
    return diffuse;
}

/*
* @return The specular color of the SceneObject
*/
const ColorRGB& SceneObject::getSpecular() const
{
    return specular;
}

/*
* @return The alpha factor of the SceneObject
*/
const double& SceneObject::getAlpha() const
{
    return alpha;
}
