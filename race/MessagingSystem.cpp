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
///
///<summary>Continuously sends out messages from the message queue.</summary>
///
void MessagingSystem::loop()
{
	while (true)
	{
		_messageQueueLock.lock();
		_subscriberGroupsLock.lock();
		sendMessage(_messageQueue.front());
		_subscriberGroupsLock.unlock();
		_messageQueue.pop();
		_messageQueueLock.unlock();
	}
}

///
///<summary>Posts a message to the messaging system.</summary>
///<param name="messageToPost">The message to post.</param>
///
void MessagingSystem::postMessage(BaseMessage messageToPost)
{
	_messageQueueLock.lock();
	_messageQueue.push(messageToPost);
	_messageQueueLock.unlock();
}

///
///<summary>Posts a message to all subscribers immediately.</summary>
///<param name="messageToPost">The message to post immediately.</param>
///
void MessagingSystem::postMessageImmediate(BaseMessage messageToPost)
{
	_subscriberGroupsLock.lock();
	sendMessage(messageToPost);
	_subscriberGroupsLock.unlock();
}

///
///<summary>Sends a message to all subscribers.</summary>
///<param name="messageToSend">The message to send.</param>
///
void MessagingSystem::sendMessage(BaseMessage messageToSend)
{
	/* If there are no subscribers to this message type, do nothing. */
	if (_subscriberGroups.count(messageToSend.getType()) == 0)
	{
		return;
	}

	SubscriberGroup& subscribers = _subscriberGroups[messageToSend.getType()];
	for each (Subscriber eachSubscriber in subscribers)
	{
		if (eachSubscriber(messageToSend))
		{
			return;
		}
	}
}

///
///<summary>Begins the MessagingSystem loop.</summary>
///
void MessagingSystem::start()
{
	loop();
}

///
///<summary>Adds a subscriber to a particular type of message to the messaging system</summary>
///<param name="messageType">The type of message to subscribe to.</param>
///<param name="subscriberToAdd">The subscribing method to add.</param>
///
void MessagingSystem::subscribe(MESSAGE_TYPE messageType, Subscriber subscriberToAdd)
{
	_subscriberGroupsLock.lock();
	/* If there is no subscriber group for this message type yet, add one. */
	if (_subscriberGroups.count(messageType) == 0)
	{
		_subscriberGroups.insert(pair<MESSAGE_TYPE, SubscriberGroup>(messageType,SubscriberGroup()));
	}

	/* If the given subscriber already exists, do nothing. */
	SubscriberGroup& subscribers = _subscriberGroups[messageType];
	for each (Subscriber eachSubscriber in subscribers)
	{
		if (eachSubscriber.target == subscriberToAdd.target)
		{
			return;
		}
	}

	subscribers.push_back(subscriberToAdd);
	_subscriberGroupsLock.unlock();
}

///
///<summary>Removes a subscriber to a particular type of message from the messaging system</summary>
///<param name="messageType">The type of message to remove the subscriber from.</param>
///<param name="subscriberToRemove">The subscribing method to remove.</param>
///
void MessagingSystem::unsubscribe(MESSAGE_TYPE messageType, Subscriber subscriberToRemove)
{
	_subscriberGroupsLock.lock();
	/* If there are no subscribers to this message type, do nothing. */
	if (_subscriberGroups.count(messageType) == 0)
	{
		return;
	}
	
	/* Otherwise, look for the given subscriber and remove it if it exists. */
	SubscriberGroup& subscribers = _subscriberGroups[messageType];
	for each(Subscriber eachSubscriber in subscribers)
	{
		if (eachSubscriber.target == subscriberToRemove.target)
		{
			subscribers.erase(remove(subscribers.begin(), subscribers.end(), subscriberToRemove), subscribers.end());
			return;
		}
	}
	_subscriberGroupsLock.unlock();
}
