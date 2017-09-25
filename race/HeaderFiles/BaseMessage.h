/*===================================================================================*//**
	BaseMessage
	
	The base class for all messages to be sent by the MessagingSystem.
	Specific message classes should inherit from this one.
    
    Copyright 2017 Erick Fernandez de Arteaga. All rights reserved.
        https://www.linkedin.com/in/erick-fda
        https://bitbucket.org/erick-fda

    @author Erick Fernandez de Arteaga
	@version 0.0.0
	@file
	
	@see BaseMessage
	@see BaseMessage.cpp
	
*//*====================================================================================*/

#ifndef BASE_MESSAGE_H
#define BASE_MESSAGE_H

/*========================================================================================
	Dependencies
========================================================================================*/
#include <iostream>
using namespace std;

/*========================================================================================
	Enums
========================================================================================*/
/**
	List of all message types.
	
	Each type should correspond to [existing subclass of BaseMessage] + "Type".
	
	Each subclass of BaseMessage should initialize the message type to be that which 
	corresponds with its class in its constructor.

	This seems redundant, but allows the other messsaging system classes to avoid the 
	inherent difficulties of using type detection to differentiate between messages.
*/
enum MESSAGE_TYPE
{
	BaseMessageType
};

/*========================================================================================
	BaseMessage	
========================================================================================*/
/**
	The base class for all messages to be sent by the MessagingSystem.
	Specific message classes should inherit from this one.
	
	@see BaseMessage
	@see BaseMessage.cpp
*/
class BaseMessage
{
	/*------------------------------------------------------------------------------------
		Class Fields
	------------------------------------------------------------------------------------*/
	private:
		MESSAGE_TYPE _type;

    /*------------------------------------------------------------------------------------
		Instance Fields
    ------------------------------------------------------------------------------------*/
    private:


    /*------------------------------------------------------------------------------------
		Constructors and Destructors
    ------------------------------------------------------------------------------------*/
    public:
		BaseMessage();

        ~BaseMessage();

	/*------------------------------------------------------------------------------------
		Instance Getter Methods
    ------------------------------------------------------------------------------------*/
    public:
		MESSAGE_TYPE getType();
    
	/*------------------------------------------------------------------------------------
		Instance Setter Methods
	------------------------------------------------------------------------------------*/
    public:


	/*------------------------------------------------------------------------------------
		Instance Methods
	------------------------------------------------------------------------------------*/
    public:


    private:

};

#endif
