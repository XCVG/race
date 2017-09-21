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
	@see SubscriberGroup.cpp
	
*//*====================================================================================*/

#ifndef SUBSCRIBER_GROUP_H
#define SUBSCRIBER_GROUP_H

/*========================================================================================
	Dependencies
========================================================================================*/
#include <functional>
#include <vector>
#include "BaseMessage.h"
using namespace std;

/*========================================================================================
	SubscriberGroup	
========================================================================================*/
/**
	A group of subscribers for the MessagingSystem that subscribe to messages of the same 
	type.
	
	@see SubscriberGroup
	@see SubscriberGroup.cpp
*/
class SubscriberGroup
{
    /*------------------------------------------------------------------------------------
		Instance Fields
    ------------------------------------------------------------------------------------*/
    private:
		MessageType _messageType;
		vector<function<bool(BaseMessage&)>> _subscribers;

    /*------------------------------------------------------------------------------------
		Constructors and Destructors
    ------------------------------------------------------------------------------------*/
    public:
		/// <param name="type">The type of message subscribers in this group are listening for.</param>
		SubscriberGroup(MessageType type);

        ~SubscriberGroup();

	/*------------------------------------------------------------------------------------
		Instance Getter Methods
    ------------------------------------------------------------------------------------*/
    public:
		MessageType getMessageType();
    
	/*------------------------------------------------------------------------------------
		Instance Setter Methods
	------------------------------------------------------------------------------------*/
    public:


	/*------------------------------------------------------------------------------------
		Instance Methods
	------------------------------------------------------------------------------------*/
    public:
		void addSubscriber(function<bool(BaseMessage&)>& subscriber);

		void removeSubscriber(function<bool(BaseMessage&)>& subscriber);

    private:

};

#endif
