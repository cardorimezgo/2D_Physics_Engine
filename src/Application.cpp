#include "Application.h"
#include "./Physics/Constants.h"


bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();
  
    particle = new Particle(50, 100, 1.0);
    particle->radius = 20;
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
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    //Wait some time until frames reach the target time in milliseconds
    static int timePreviousFrame;
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0) {
        SDL_Delay(timeToWait);
    }

    //Calculate dela time in seconds
    float deltatime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltatime > 0.016) {
        deltatime = 0.016;
    }

    //Set the time to the currect frame to be used in the next one
    timePreviousFrame = SDL_GetTicks();

    //Proceed to update the objects in the scene 
    particle->acceleration = Vec2(0.0, 9.8 * PIXELS_PER_METER);
    // Integrate the acceleration and velocity to find the new velocity
    particle->velocity += particle->acceleration * deltatime;
    particle->position += particle->velocity * deltatime;

    // if particle posX+rad == width: flip Xvel.
    // if particle  posY+rad == height : flip Yvel.  
    if (particle->position.x + (particle->radius/2) == Graphics::Width())
    {

    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);
    Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    delete particle;
    Graphics::CloseWindow();
}
