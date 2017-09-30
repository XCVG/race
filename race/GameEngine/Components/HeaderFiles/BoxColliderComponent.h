#pragma once
#include "ColliderComponent.h"
class BoxColliderComponent : public ColliderComponent
{
public:
	int _minX, _minY, _minZ;
	int _maxX, _maxY, _maxZ;
};