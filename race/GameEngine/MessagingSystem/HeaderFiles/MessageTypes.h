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

#include "../../RenderEngine/HeaderFiles/RenderableTypes.h" //should probably fix that path

/*========================================================================================
	Enums
========================================================================================*/
/**
	Enum of message types.
	
	-	Naming convention for message types is [Message type] + "Type".
		For example, "BaseMessage" has type "BaseMessageType".
*/
enum MESSAGE_TYPE
{
	BaseMessageType,
	FileLoadMessageType,
	FileLoadedMessageType,
	RenderLoadMessageType,
	RenderReadyMessageType,
	RenderLoadSingleMessageType,
	RenderDrawMessageType,
	RenderDrawOverlayMessageType,
	RenderUnloadMessageType,
	RenderFinishedMessageType
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

//*****FILE MESSAGES

class FileLoadMessageContent : BaseMessageContent
{
	public:
		std::string path;
		bool relative;
};

class FileLoadedMessageContent: BaseMessageContent
{
	public:
		std::string path;
		bool relative;
		bool success;
		std::string fileContents;
};

//*****RENDERER MESSAGES

class RenderLoadMessageContent : BaseMessageContent
{
	public:
		RenderableSetupData data;
};

class RenderReadyMessageContent : BaseMessageContent
{

};

class RenderLoadSingleMessageContent : BaseMessageContent
{
	public:
		RenderableModel model;
		RenderableTexture texture;
};

class RenderDrawMessageContent : BaseMessageContent
{
	public:
		RenderableScene *scene_p; //renderer will ALWAYS be responsible for deletion
};

class RenderDrawOverlayMessageContent : BaseMessageContent
{
	public:
		RenderableOverlay *overlay_p; //renderer will ALWAYS be responsible for deletion
};

class RenderUnloadMessageContent : BaseMessageContent
{

};

class RenderFinishedMessageContent : BaseMessageContent
{

};

#endif
