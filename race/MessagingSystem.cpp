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
	@see MessagingSystem.h
	
*//*====================================================================================*/

/*========================================================================================
	Dependencies
========================================================================================*/
#include "MessagingSystem.h"
using namespace std;

/*----------------------------------------------------------------------------------------
	Singleton
----------------------------------------------------------------------------------------*/
MessagingSystem& MessagingSystem::instance()
{
    static MessagingSystem instance;
    return instance;
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
void MessagingSystem::addSubscriber(MESSAGE_TYPE messageType, Subscriber subscriberToAdd)
{
	/* If _subscribers does not yet contain a subscriber group for this message type, add one. */
	if (_subscriberGroups.find(messageType) == _subscriberGroups.end())
	{
		_subscriberGroups.insert(pair<MESSAGE_TYPE, SubscriberGroup>(messageType,SubscriberGroup()));
	}

	/* Get the subscriber group for the appropriate message type. */
	SubscriberGroup& subscribers = _subscriberGroups[messageType];

	/* If the subscriber group for this message type does not yet contain the given subscriber, add it. */
	if (find(subscribers.begin(), subscribers.end(), subscriberToAdd) == subscribers.end())
	{
		subscribers.push_back(subscriberToAdd);
	}
}
