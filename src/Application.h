#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/Body.h"
#include <vector>

class Application {
    private:
        bool running = false;
        std::vector<Body*> bodies;
        Vec2 pushForce = Vec2(0, 0);
        Vec2 mouseCursor = Vec2(0, 0);
        bool leftMouseButtonDown = false;

        int numBodies = 10;
/*
        Vec2 anchor;
        float k = 50;
        float restLength = 20;
*/
    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
        void Render();
        void Destroy();
};

#endif
