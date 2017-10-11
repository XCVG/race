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
#include <SDL2/SDL.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#endif
#include <thread>

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
class PhysicsEngine
{
    /*------------------------------------------------------------------------------------
		Instance Fields
    ------------------------------------------------------------------------------------*/
    private:
		bool _running = false;

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
		void update();
		void stop();

    private:
		void loop();
};
