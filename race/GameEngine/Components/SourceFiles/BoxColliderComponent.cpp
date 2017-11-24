#include "BoxColliderComponent.h"
BoxColliderComponent::BoxColliderComponent(std::string filename)
{
    std::string fileContents = FileHelper::loadFileFromStringRelative(MODEL_BASEPATH_CONST + filename + MODEL_EXTENSION_CONST);
    std::vector<GLfloat> vertices = OBJImport::importObjInfoVertices(fileContents);
	for (int i = 0; i < vertices.size() - 1; i += 3)
	{
		if (this->_minX > vertices.at(i)) this->_minX = vertices.at(i);
		else if (this->_maxX < vertices.at(i)) this->_maxX = vertices.at(i);
	}
}

void BoxColliderComponent::setMinX(int _x)
{
    this->_minX = _x;
};
void BoxColliderComponent::setMinY(int _y)
{
    this->_minY = _y;
};
void BoxColliderComponent::setMinZ(int _z)
{
    this->_minZ = _z;
};
void BoxColliderComponent::setMaxX(int _x)
{
    this->_maxX = _x;
};
void BoxColliderComponent::setMaxY(int _y)
{
    this->_maxY = _y;
};
void BoxColliderComponent::setMaxZ(int _z)
{
    this->_maxZ = _z;
};
int BoxColliderComponent::getMinX()
{
    return this->_minX;
};
int BoxColliderComponent::getMinY()
{
    return this->_minY;
};
int BoxColliderComponent::getMinZ()
{
    return this->_minZ;
};
int BoxColliderComponent::getMaxX()
{
    return this->_maxX;
};
int BoxColliderComponent::getMaxY()
{
    return this->_maxY;
};
int BoxColliderComponent::getMaxZ()
{
    return this->_maxZ;
};