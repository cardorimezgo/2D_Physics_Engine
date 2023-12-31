#pragma once

#include "./Vec2.h"
#include "./Particle.h"

struct Force{
   static Vec2 GenerateDragForce(const Particle& particle, float k);
   static Vec2 GenerateFrictionForce(const Particle& particle, float k);
   static Vec2 GenerateSpringForce(const Particle& particle, Vec2 anchor, float restLenght, float k);
   static Vec2 GenerateSpringForce(const Particle& a, const Particle& b, float restLength, float k);
   static Vec2 GenerateGravitationalForce(const Particle& a , const Particle& b, float G, float minDistance, float maxDistance);
};
