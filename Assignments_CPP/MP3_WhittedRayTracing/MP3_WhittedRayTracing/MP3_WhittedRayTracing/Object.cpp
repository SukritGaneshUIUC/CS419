#include "Object.h"

/*
* Constructor for Object (abstract class)
*
* @param type The ObjectType of the Object
* @param material The Material of the Object
*/
Object::Object(const ObjectType& type, const Material& material) : type(type), material(material) {}

/*
* @return The type of the Object
*/
const ObjectType& Object::getObjectType() const
{
	return type;
}

const Material& Object::getMaterial() const
{
	return material;
}

