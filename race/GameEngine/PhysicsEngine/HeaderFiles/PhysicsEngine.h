/*===================================================================================*//**
	PhysicsEngine

	The physics engine for the RACE game engine.
    
    Copyright 2017 Erick Fernandez de Arteaga. All rights reserved.
        https://www.linkedin.com/in/erick-fda
        https://bitbucket.org/erick-fda

    @author Erick Fernandez de Arteaga, John Janzen
	@version 0.0.0
	@file
	
	@see PhysicsEngine
	@see PhysicsEngine.cpp
	
*//*====================================================================================*/

#pragma once
#ifdef __APPLE__
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\euler_angles.hpp>
#include <SDL.h>
#endif
#include <thread>
#include <math.h>
#include "MessageReceiver.h"
#include "../../MessagingSystem/HeaderFiles/MessagingSystem.h"
#include "GameObject.h"
#include "../../HeaderFiles/InputTypes.h"
#include "../../Components/HeaderFiles/ComponentHeaders.h"


/*========================================================================================
	Dependencies
========================================================================================*/


/*========================================================================================
	PhysicsEngine	
========================================================================================*/
/**
	The physics engine for the RACE game engine.
	
	@see PhysicsEngine
	@see PhysicsEngine.cpp
*/
class PhysicsEngine : public MessageReceiver
{
    /*------------------------------------------------------------------------------------
		Instance Fields
    ------------------------------------------------------------------------------------*/
    private:
		bool _running = false;
		GLfloat _deltaTime;
		const float MATH_PI = 3.14159f;
		float count = 0;
		float rotationAccel = 0;

    /*------------------------------------------------------------------------------------
		Constructors and Destructors
    ------------------------------------------------------------------------------------*/
    public:
		PhysicsEngine();

		~PhysicsEngine();

	/*------------------------------------------------------------------------------------
		Instance Getter Methods
    ------------------------------------------------------------------------------------*/
    public:
        
    
	/*------------------------------------------------------------------------------------
		Instance Setter Methods
	------------------------------------------------------------------------------------*/
    public:


	/*------------------------------------------------------------------------------------
		Instance Methods
	------------------------------------------------------------------------------------*/
    public:
			std::thread* start();
			void stop();
			void flagLoop();
			// TODO: Physics function calls
	#pragma region Physics Calculation Methods
			void accelerate(GameObject *go, RigidBodyComponent* rbc);
			void accelerate(GameObject *go, GLfloat x, GLfloat y, GLfloat z);
			void decelerate(GameObject *go, GLfloat x, GLfloat y, GLfloat z);
	#pragma endregion

    private:
		void loop();
		void checkMessage(std::shared_ptr<Message>);
		void applyAcceleration(GameObject*);
		void applyTurning(GameObject* go);
		void generalPhysicsCall(GameObject*);
		GLfloat getAngleFromTurn(GameObject *go, GLfloat tireDegree);
		void turnGameObject(GameObject *go);
};