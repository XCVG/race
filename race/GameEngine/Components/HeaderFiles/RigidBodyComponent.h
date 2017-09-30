#pragma once
#include "Component.h"
class RigidBodyComponent : public Component
{
public: 
	int _weight;
	int _bouncyness;
	int _friction;
	int _resistance;
};