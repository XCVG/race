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

MessagingSystem::MessagingSystem()
{

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
		_messageQueueMutex.lock();
		_subscriberGroupsMutex.lock();
		sendMessage(_messageQueue.front());
		_subscriberGroupsMutex.unlock();
		_messageQueue.pop();
		_messageQueueMutex.unlock();
	}
}

///
///<summary>Posts a message to the messaging system.</summary>
///<param name="messageToPost">The message to post.</param>
///
void MessagingSystem::postMessage(BaseMessage messageToPost)
{
	_messageQueueMutex.lock();
	_messageQueue.push(messageToPost);
	_messageQueueMutex.unlock();
}

///
///<summary>Posts a message to all subscribers immediately.</summary>
///<param name="messageToPost">The message to post immediately.</param>
///
void MessagingSystem::postMessageImmediate(BaseMessage messageToPost)
{
	_subscriberGroupsMutex.lock();
	sendMessage(messageToPost);
	_subscriberGroupsMutex.unlock();
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
	for (auto const& eachSubscriber : subscribers)
	{
		if (eachSubscriber.second(messageToSend))
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
int MessagingSystem::subscribe(MESSAGE_TYPE messageType, Subscriber subscriberToAdd)
{
	_nextSubscriberIdMutex.lock();
	_subscriberGroupsMutex.lock();

	/* If there is no subscriber group for this message type yet, add one. */
	if (_subscriberGroups.count(messageType) == 0)
	{
		_subscriberGroups.insert(pair<MESSAGE_TYPE, SubscriberGroup>(messageType,SubscriberGroup()));
	}

	/* Add the new subscriber, assign them a unique ID, and increment the unique ID counter. */
	SubscriberGroup& subscribers = _subscriberGroups[messageType];
	subscribers.insert(pair<int, Subscriber>(_nextSubscriberId, subscriberToAdd));
	int newSubscriberId = _nextSubscriberId;
	_nextSubscriberId++;

	_subscriberGroupsMutex.unlock();
	_nextSubscriberIdMutex.unlock();

	return newSubscriberId;
}

///
///<summary>Removes a subscriber to a particular type of message from the messaging system.</summary>
///<param name="messageType">The type of message to remove the subscriber from.</param>
///<param name="subscriberIdToRemove">The ID of the subscribing method to remove.</param>
///
void MessagingSystem::unsubscribe(MESSAGE_TYPE messageType, unsigned int subscriberIdToRemove)
{
	_subscriberGroupsMutex.lock();

	/* If there are no subscribers to this message type, do nothing. */
	if (_subscriberGroups.count(messageType) == 0)
	{
		return;
	}
	
	/* Otherwise, look for the given subscriber and remove it if it exists. */
	SubscriberGroup& subscribers = _subscriberGroups[messageType];
	for (auto const& eachSubscriber : subscribers)
	{
		if (eachSubscriber.first == subscriberIdToRemove)
		{
			subscribers.erase(subscriberIdToRemove);
			return;
		}
	}

	_subscriberGroupsMutex.unlock();
}
