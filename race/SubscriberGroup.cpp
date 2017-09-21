/*===================================================================================*//**
	SubscriberGroup
	
	A group of subscribers for the MessagingSystem that subscribe to messages of the same 
	type.
    
    Copyright 2017 Erick Fernandez de Arteaga. All rights reserved.
        https://www.linkedin.com/in/erick-fda
        https://bitbucket.org/erick-fda

    @author Erick Fernandez de Arteaga
	@version 0.0.0
	@file
	
	@see SubscriberGroup
	@see SubscriberGroup.h
	
*//*====================================================================================*/

/*========================================================================================
	Dependencies
========================================================================================*/
#include "SubscriberGroup.h"
using namespace std;

/*----------------------------------------------------------------------------------------
	Constructors and Destructors
----------------------------------------------------------------------------------------*/
///
/// <param name="type">The type of message subscribers in this group are listening for.</param>
///
SubscriberGroup::SubscriberGroup(MESSAGE_TYPE type):
	_messageType(type)
{}

SubscriberGroup::~SubscriberGroup()
{}

/*----------------------------------------------------------------------------------------
	Instance Getter Methods
----------------------------------------------------------------------------------------*/
MESSAGE_TYPE SubscriberGroup::getMessageType()
{
	return _messageType;
}

/*----------------------------------------------------------------------------------------
	Instance Setter Methods
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Instance Methods
----------------------------------------------------------------------------------------*/
///
///<summary>Adds the given subscriber to the SubscriberGroup's list of subscribed methods.</summary>
///<param name="subscriberToAdd">The subscribing method to add.</param>
///
void SubscriberGroup::addSubscriber(function<bool(BaseMessage&)>& subscriberToAdd)
{
	/* Do not add the subscriber if it already on the subscriber list. */
	for each (function<bool(BaseMessage&)> eachSubscriber in _subscribers)
	{
		if (eachSubscriber.target == subscriberToAdd.target)
		{
			cout << "Cannot add the following subscriber to the " << _messageType << 
				" SubscriberGroup because the SubscriberGroup already holds a reference to this method: " << 
				subscriberToAdd.target_type().name();
			return;
		}
	}

	_subscribers.push_back(subscriberToAdd);
}

///
///<summary>Looks for the given subscriber in the subscribers list. If it exists, removes it.</summary>
///<param name="subscriberToRemove">The subscribing method to remove.</param>
///
void SubscriberGroup::removeSubscriber(function<bool(BaseMessage&)>& subscriberToRemove)
{
	for each(function<bool(BaseMessage&)> eachSubscriber in _subscribers)
	{
		if (eachSubscriber.target == subscriberToRemove.target)
		{
			_subscribers.erase(remove(_subscribers.begin(), _subscribers.end(), eachSubscriber), _subscribers.end());
			return;
		}
	}

	/* If we couldn't find a matching subscriber to remove, print a message saying so. */
	cout << "Couldn't remove the following subscriber from the " << _messageType <<
		" SubscriberGroup because the SubscriberGroup doesn't hold any reference to this method: " <<
		subscriberToRemove.target_type().name();
}
