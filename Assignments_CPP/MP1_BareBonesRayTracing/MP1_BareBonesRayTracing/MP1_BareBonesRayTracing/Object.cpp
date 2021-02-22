#include "Object.h"

/*
* Constructor for Object (abstract class)
* 
* @param type The ObjectType of the Object
*/
Object::Object(ObjectType type) : type(type) {}

/*
* @return The type of the Object
*/
const ObjectType& Object::getObjectType() const
{
	return type;
}

