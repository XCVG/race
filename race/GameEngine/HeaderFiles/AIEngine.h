#pragma once

class AIEngine {
public:
    void start();
    void update();
    void cleanUp();
    AIEngine();
    ~AIEngine();
private:
    void loop();
};