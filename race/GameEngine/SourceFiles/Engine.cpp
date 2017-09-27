#include "Engine.h"
#include "ErrorHandler.h"

void Engine::start() {
    // Create the other engines, or at least get pointer to them
	_fileEngine = new FileEngine();
	if (_fileEngine == nullptr) {
		std::cout << ErrorHandler::getErrorString(1) << std::endl;
	}
    _renderEngine = new RenderEngine();
    if (_renderEngine == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    _physicsEngine = new PhysicsEngine();
    if (_physicsEngine == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    _aiEngine = new AIEngine();
    if (_aiEngine == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    _inputEngine = new InputEngine();
    if (_inputEngine == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    _soundEngine = new SoundEngine();
    if (_soundEngine == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    try {
		_fileEngine->start();
        _renderEngine->start();
        _physicsEngine->start();
        _aiEngine->start();
        _inputEngine->start();
        _soundEngine->start();
    } catch (std::exception e) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
}

void Engine::update() {

}

void Engine::loop() {

}

Engine::Engine() {

}

Engine::~Engine() {
    _soundEngine->~SoundEngine();
    _inputEngine->~InputEngine();
    _aiEngine->~AIEngine();
    _physicsEngine->~PhysicsEngine();
    _renderEngine->~RenderEngine();
	_fileEngine->~FileEngine();
}