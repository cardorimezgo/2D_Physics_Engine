#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    Body* body = new Body(CircleShape(50), Graphics::Width() / 2.0, Graphics::Height() * 0.05, 2.0);
    bodies.push_back(body);

}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = -50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = -50 * PIXELS_PER_METER;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 0;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = 0;
                break;
            case SDL_MOUSEMOTION:
                mouseCursor.x = event.motion.x;
                mouseCursor.y = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = true;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    mouseCursor.x = x;
                    mouseCursor.y = y;
                }
                break;
           /* case SDL_MOUSEBUTTONUP:
                if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = false;
                    Vec2 impulseDirection = (bodies[numBodies - 1]->position - mouseCursor).UnitVector();
                    float impulseMagnitude = (bodies[numBodies - 1]->position - mouseCursor).Magnitude() * 5.0;
                    bodies[numBodies - 1]->velocity = impulseDirection * impulseMagnitude;
                }
                break;
            */
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    // Wait some time until the reach the target frame time in milliseconds
    static int timePreviousFrame;
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0)
        SDL_Delay(timeToWait);

    // Calculate the deltatime in seconds
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016)
        deltaTime = 0.016;

    // Set the time of the current frame to be used in the next one
    timePreviousFrame = SDL_GetTicks();

    // Apply forces to the bodies
    for (auto body: bodies) {
        //body->AddForce(pushForce);

        // Apply a drag force
        //Vec2 drag = Force::GenerateDragForce(*body, 0.001);
        //body->AddForce(drag);
        
        // Apply weight force
        Vec2 weight = Vec2(0.0, body->mass * 9.8 * PIXELS_PER_METER);
        body->AddForce(weight);

        float torque = 200;
        body->addTorque(torque);
    }
    /*
    // Apply a spring force to the body connected to the anchor
    Vec2 springForce_anchor = Force::GenerateSpringForce(*bodies[0], anchor, 10, k);
    bodies[0]->AddForce(springForce_anchor);
    
    for (int i = 0; i < numBodies - 1; i++) { 
        Vec2 springForce = Force::GenerateSpringForce(*bodies[i], *bodies[i+1], 30, k); // 1/8 distance
        bodies[i]->AddForce(springForce);
        bodies[i + 1]->AddForce(-springForce);
    }
    */  
    // Integrate the acceleration and velocity to estimate the new position
    for (auto body: bodies) {
        body->IntegrateLinear(deltaTime);
        body->IntegrateAngular(deltaTime);
    }

    // Check the boundaries of the window
    for (auto body: bodies) {
        if (body->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = (CircleShape*)body->shape;
            if (body->position.x - circleShape->radius <= 0) {
                body->position.x = circleShape->radius;
                body->velocity.x *= -0.9;
            }
            else if (body->position.x + circleShape->radius >= Graphics::Width()) {
                body->position.x = Graphics::Width() - circleShape->radius;
                body->velocity.x *= -0.9;
            }
            if (body->position.y - circleShape->radius <= 0) {
                body->position.y = circleShape->radius;
                body->velocity.y *= -0.9;
            }
            else if (body->position.y + circleShape->radius >= Graphics::Height()) {
                body->position.y = Graphics::Height() - circleShape->radius;
                body->velocity.y *= -0.9;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF0F0721);
    /* /// Spring Forces ------------
    if (leftMouseButtonDown) {
            Graphics::DrawLine(bodies[numBodies - 1]->position.x, bodies[numBodies - 1]->position.y, mouseCursor.x, mouseCursor.y, 0xFF0000FF);
    }

    // Draw the spring
    Graphics::DrawLine(anchor.x, anchor.y, bodies[0]->position.x, bodies[0]->position.y, 0xFF313131);

    for (int i = 0; i < numBodies - 1; i++) {

        Graphics::DrawLine(bodies[i]->position.x, bodies[i]->position.y,
                           bodies[i + 1]->position.x, bodies[i + 1]->position.y,
                           0xFF313131);
    }
    
    // Draw the anchor
    Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF001155);
    */

    // Draw the bob
    for (auto body : bodies) {
        if (body->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = (CircleShape*)body->shape;
            Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, 0xFFFFFFFF);
        }
        else
        {
            //todo: draw other type of shape
        }
    }
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for (auto body: bodies) {
        delete body;
    }
    Graphics::CloseWindow();
}
