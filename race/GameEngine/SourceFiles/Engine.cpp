#include "Engine.h"
#include "ErrorHandler.h"
Engine::Engine() {
    
};
Engine::~Engine() {
    _soundEngine_p->~SoundEngine();
    _inputEngine_p->~InputEngine();
    _aiEngine_p->~AIEngine();
    _physicsEngine_p->~PhysicsEngine();
    _renderEngine_p->~RenderEngine();
};
void Engine::start() {
    // Create the other engines, or at least get pointer to them
    _renderEngine_p = new RenderEngine();
    if (_renderEngine_p == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    _physicsEngine_p = new PhysicsEngine();
    if (_physicsEngine_p == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    _aiEngine_p = new AIEngine();
    if (_aiEngine_p == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    _inputEngine_p = new InputEngine();
    if (_inputEngine_p == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    _soundEngine_p = new SoundEngine();
    if (_soundEngine_p == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    try {
        _renderEngine_p->start();
        _physicsThread_p = _physicsEngine_p->start();
        _aiThread_p = _aiEngine_p->start();
        _inputEngine_p->start();
        _soundEngine_p->start();
    } catch (std::exception e) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    _engineThread_p = new std::thread(&Engine::loop, this);
    if (!_running) {
        _running = true;
    } else {
        this->stop();
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
        delete this;
    }
	GameObject *go = new GameObject();
	go->addComponent(new VelocityComponent(new glm::vec3(0, 0, 1), 100));
	VelocityComponent *vc = go->getComponent<VelocityComponent *>();
	SDL_Log("%s", vc.ToString());
};
void Engine::update() {
    SDL_Log("%s", "Running Engine::udpate");
};

void Engine::loop() {
    if (!_running) {
        return;
    }
    while(_running) {
        this->update();
    }
};
///
/// <title>
/// Engine Stop
/// </title>
/// <summary>
/// Will stop the engine from running. This will halt the main loop and other 
/// threads. The main purpose is to halt the game here. This can be a 
/// catch-all for other cleanup that is required in a stop/start of the game.
/// </summary>
/// 
void Engine::stop() {
    _running = false;
    _physicsEngine_p->stop();
    _aiEngine_p->stop();
};