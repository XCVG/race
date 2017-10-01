#include "PhysicsEngine.h"

std::thread* PhysicsEngine::start() {
    if (!_running) {
        _running = true;
    }
    return new std::thread(&PhysicsEngine::loop, this);
}

void PhysicsEngine::loop() {
    if (!_running) {
        return;
    }
    while(_running) {
        this->update();
    }
}

void PhysicsEngine::update() {
    SDL_Log("%s", "Running PhysicsEngine::update");
}

void PhysicsEngine::stop() {
    _running = false;
}

PhysicsEngine::PhysicsEngine() {

}

PhysicsEngine::~PhysicsEngine() {
    
}