#include "Object.h"

/*
* Constructor for Object (abstract class)
*
* @param type The ObjectType of the Object
* @param material The Material of the Object
*/
Object::Object(const ObjectType& type, const std::shared_ptr<Material>& material) : type(type), material(material) {}

/*
* @return The type of the Object
*/
const ObjectType& Object::getObjectType() const
{
	return type;
}

const std::shared_ptr<Material>& Object::getMaterial() const
{
	return material;
}

