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
using namespace std;

/*========================================================================================
	Enums
========================================================================================*/
/**
	List of all message types.
	
	Each type should correspond to [existing subclass of BaseMessage] + "Type".
	
	Each subclass of BaseMessage should redeclare the static const MessageType _type 
	field and set it to the type that corresponds with its class type.

	This seems redundant, but allows the other messsaging classes to avoid the inherent 
	difficulties of using type detection to differentiate between messages.
*/
enum MessageType
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
		static const MessageType _type = BaseMessageType;

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
