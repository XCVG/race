#pragma once
class PhysicsEngine {
public:
    void start();
    void update();
    PhysicsEngine();
    ~PhysicsEngine();
private:
    void loop();
};