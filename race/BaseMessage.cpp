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
	@see BaseMessage.h
	
*//*====================================================================================*/

/*========================================================================================
	Dependencies
========================================================================================*/
#include "BaseMessage.h"
using namespace std;

/*----------------------------------------------------------------------------------------
	Constructors and Destructors
----------------------------------------------------------------------------------------*/
BaseMessage::BaseMessage():
	_type(BaseMessageType)
{}

BaseMessage::~BaseMessage()
{}

/*----------------------------------------------------------------------------------------
	Instance Getter Methods
----------------------------------------------------------------------------------------*/
MESSAGE_TYPE BaseMessage::getType()
{
	return _type;
}

/*----------------------------------------------------------------------------------------
	Instance Setter Methods
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Instance Methods
----------------------------------------------------------------------------------------*/

