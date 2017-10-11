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
	@see PhysicsEngine.h
	
*//*====================================================================================*/

/*========================================================================================
	Dependencies
========================================================================================*/
#include "PhysicsEngine.h"

/*----------------------------------------------------------------------------------------
	Constructors and Destructors
----------------------------------------------------------------------------------------*/
///
///	Physics engine constructor.
///
PhysicsEngine::PhysicsEngine() {

}

///
///	Physics engine destructor.
///
PhysicsEngine::~PhysicsEngine() {

}

/*----------------------------------------------------------------------------------------
	Instance Getter Methods
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Instance Setter Methods
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Instance Methods
----------------------------------------------------------------------------------------*/
///
///	Starts the physics engine loop on its own thread and returns a pointer to the thread.
///
std::thread* PhysicsEngine::start() {
	if (!_running) {
		_running = true;
	}
	return new std::thread(&PhysicsEngine::loop, this);
}

///
///	The physics engine loop.
///
void PhysicsEngine::loop() {
    if (!_running) {
        return;
    }
    while(_running) {
        this->update();
    }
}

///
///	The physics engine update function.
///
void PhysicsEngine::update() {
    // Commenting this out to avoid spamming the debug log with dummy messages.
	//SDL_Log("%s", "Running PhysicsEngine::update");
}

///
///	Stops the physics engine.
///
void PhysicsEngine::stop() {
    _running = false;
}
