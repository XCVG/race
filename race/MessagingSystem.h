/*===================================================================================*//**
	MessagingSystem
	
	Singleton for sending messages to objects that have subscribed to particular event 
    types.
    
    Copyright 2017 Erick Fernandez de Arteaga. All rights reserved.
        https://www.linkedin.com/in/erick-fda
        https://bitbucket.org/erick-fda

    @author Erick Fernandez de Arteaga
	@version 0.0.0
	@file
	
	@see MessagingSystem
	@see MessagingSystem.cpp
	
*//*====================================================================================*/

#ifndef MESSAGING_SYSTEM_H
#define MESSAGING_SYSTEM_H

/*========================================================================================
	Dependencies
========================================================================================*/
#include <algorithm>
#include <functional>
#include <map>
#include <queue>
#include <vector>
#include "BaseMessage.h"
using namespace std;
using Subscriber = function<bool(BaseMessage&)>&;
using SubscriberGroup = vector<Subscriber>;

/*========================================================================================
	MessagingSystem	
========================================================================================*/
/**
	Singleton for sending messages to objects that have subscribed to particular event 
    types.
	
	@see MessagingSystem
	@see MessagingSystem.cpp
*/
class MessagingSystem
{
    /*------------------------------------------------------------------------------------
		Singleton
    ------------------------------------------------------------------------------------*/
    public:
        /* Ensure constructor and assignment operator for singleton are not implemented. */
		MessagingSystem(MessagingSystem const&)   = delete;
        void operator=(MessagingSystem const&)     = delete;

        /**
            Declares, instantiates, and returns the static instance.
        */
        static MessagingSystem& instance();

	private:
		MessagingSystem();

    /*------------------------------------------------------------------------------------
		Instance Fields
    ------------------------------------------------------------------------------------*/
    private:
		map <MESSAGE_TYPE, SubscriberGroup> _subscriberGroups;
		queue<BaseMessage&> _messageQueue;

    /*------------------------------------------------------------------------------------
		Constructors and Destructors
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
		void addSubscriber(MESSAGE_TYPE messageType, Subscriber messageHandler);
		void removeSubscriber(MESSAGE_TYPE messageType, Subscriber messageHandler);
		void queueMessage(BaseMessage& messageToQueue);
		void triggerMessage(BaseMessage& messageToTrigger);

    private:

};

#endif
