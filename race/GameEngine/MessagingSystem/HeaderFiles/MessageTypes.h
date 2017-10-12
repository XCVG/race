/*===================================================================================*//**
	MessageTypes
	
	Header file for message and message content types.
    
    Copyright 2017 Erick Fernandez de Arteaga. All rights reserved.
        https://www.linkedin.com/in/erick-fda
        https://bitbucket.org/erick-fda

    @author Erick Fernandez de Arteaga
	@version 0.0.0
	@file
	
	@see MyClass
	@see MyClass.cpp
	
*//*====================================================================================*/

#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

/*========================================================================================
	Dependencies
========================================================================================*/

#include <string>

/*========================================================================================
	Enums
========================================================================================*/
/**
	Enum of message types.
	
	-	Naming convention for message types is [Message type] + "Type".
		For example, "BaseMessage" has type "BaseMessageType".
*/
enum class MESSAGE_TYPE
{
	BaseMessageType,
	FileLoadMessageType,
	FileLoadedMessageType
};

/*========================================================================================
	Message Content Types
========================================================================================*/
/**
	Each type of message should have a small class for its content.
	
	-	Naming convention for content types is [Message type] + "Content".
		For example, "Base Message" has content type "BaseMessageContent".
	
	-	All content variables should be public.

	-	Setting a message's content deletes the old content's memory! Because of this, 
		messages should not share pointers to the same content object!
*/
class BaseMessageContent
{};

class FileLoadMessageContent : public BaseMessageContent
{
public:
	std::string path;
	bool relative;
};

class FileLoadedMessageContent : public BaseMessageContent
{
public:
	size_t hash;
	std::string path;
	bool relative;
	std::string content;
};

#endif
