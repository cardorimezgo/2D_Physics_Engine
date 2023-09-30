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

    Particle* smallParticle = new Particle(200 , 200, 1.0);
    smallParticle->radius = 6;
    particles.push_back(smallParticle);

    Particle* bigParticle = new Particle(500, 500, 20.0);
    bigParticle->radius = 20;
    particles.push_back(bigParticle);
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
    for(auto particle: particles)
    {
        /*
        Vec2 wind = Vec2(0.9 * PIXELS_PER_METER, 0.0);
        particle->AddForce(wind);

        Vec2 weight = Vec2(0.0, particle->mass * 9.8 * PIXELS_PER_METER);
        particle->AddForce(weight);

         particle->AddForce(pushForce);

         if(particle->position.y >= liquid.y){
             Vec2 drag = Force::GenerateDragForce(*particle, 0.05);
             particle->AddForce(drag);
         }*/
         particle->AddForce(pushForce);

         //apply fruction force
	 Vec2 friction = Force::GenerateFrictionForce(*particle, 5);
//         Vec2 friction = Force::GenerateFrictionForce(*particle, 5.6 * PIXELS_PER_METER);
         particle->AddForce(friction);
    }

    //Apply gravitational force to our two particles/planets
    Vec2 attraction = Force::GenerateGravitationalForce(*particles[0] , *particles[1] , 1000.0, 5, 100);

    particles[0]->AddForce(attraction);
    particles[1]->AddForce(-attraction);	

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

    Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFFAA3300);
    Graphics::DrawFillCircle(particles[1]->position.x, particles[1]->position.y, particles[1]->radius, 0xFF00FFFF);
    
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
