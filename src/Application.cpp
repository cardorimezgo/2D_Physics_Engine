#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include <cstdlib>

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();
    
    anchor = Vec2(Graphics::Width() / 2.0, 30);

    Particle* bob = new Particle(Graphics::Width() / 2.0, Graphics::Height() / 2.0 , 2.0);
    bob->radius = 10;
    particles.push_back(bob);
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
            if(!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT){
                leftMouseButtonDown = true;
                int x, y;
                SDL_GetMouseState(&x , &y);
                mouseCursor.x = x;
                mouseCursor.y = y;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT){
                leftMouseButtonDown = false;
                Vec2 impulseDirection = (particles[0]->position - mouseCursor).UnitVector();
                float impulseMagnitude = (particles[0]->position - mouseCursor).Magnitude() * 5.0;
                particles[0]->velocity= impulseDirection * impulseMagnitude;
            }
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

    //Proceed to apply forces to particle(s)
    for(auto particle: particles){
         particle->AddForce(pushForce);

         //apply drag force
	 Vec2 drag = Force::GenerateDragForce(*particle, 0.005);
         particle->AddForce(drag);

	 //Apply weight force
	 Vec2 weight = Vec2(0.0, particle->mass * 9.8 * PIXELS_PER_METER);
	 particle->AddForce(weight);
    }

    //Apply spring force to the particle connected to the anchor
    Vec2 springForce = Force::GenerateSpringForce(*particles[0], anchor, restLength, k);
    particles[0]->AddForce(springForce);

    // Integrate the acceleration and velocity to find the new velocity
    for(auto particle: particles){
        particle->Integrate(deltatime);
    }

    //calculate boundary bouncing screen
    for(auto particle: particles)
    {
    if (particle->position.x + particle->radius >= Graphics::Width())
    {
        particle->position.x = Graphics::Width() - particle->radius;
        particle->velocity.x *= -1.0;
    }
    else if(particle->position.x - particle->radius <= 0)
    {
        particle->position.x = particle->radius;
        particle->velocity.x *= -1.0;
    }
    if(particle->position.y + particle->radius >= Graphics::Height())
    {
        particle->position.y = Graphics::Height() - particle->radius;
        particle->velocity.y *= -1.0;
    }
    else if(particle->position.y - particle->radius <= 0)
    {
        particle->position.y = particle->radius;
        particle->velocity.y *= -1.0;
    }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

    if(leftMouseButtonDown){
        Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, mouseCursor.x, mouseCursor.y, 0xFFFFFFFF);
    }
   	//Draw the spring
        Graphics::DrawLine(anchor.x, anchor.y, particles[0]->position.x, particles[0]->position.y, 0xFF313131);
	 
	//Draw the anchor
	Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF001155);

	//Draw bob
	Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFFFFFFFF);
    
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for(auto particle: particles){    
        delete particle;
    }
    Graphics::CloseWindow();
}
