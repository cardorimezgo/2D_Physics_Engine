#include "Force.h"
#include <algorithm>

Vec2 Force::GenerateDragForce(const Particle& particle, float k){
    Vec2 dragForce = Vec2(0,0);
    if(particle.velocity.MagnitudeSquared() > 0){
        //calculate the drag direction(inverse of velocity unit vector)
        Vec2 dragDirection = particle.velocity.UnitVector() * -1.0;

        //Calculate the drag magnitude, k * |v|^2
        float dragMagnitud = k * particle.velocity.MagnitudeSquared();

        //generate the final drag force with direction and magnitude
        dragForce = dragDirection * dragMagnitud;
    }
    return dragForce;
}


Vec2 Force::GenerateFrictionForce(const Particle& particle, float k){
    Vec2 frictionForce = Vec2(0,0);

    //Calculate the friction direction (inverse of velocity unit vector)
    Vec2 frictionDirection = particle.velocity.UnitVector() * -1.0;

    //Calculate the friction magnitude
    float frictionMagnitude = k;

    //Calculate the final friction force
    frictionForce = frictionDirection * frictionMagnitude;

    return frictionForce;
}

Vec2 Force::GenerateGravitationalForce(const Particle& a , const Particle& b, float G, float minDistance, float maxDistance){
	//Calculate distance between two objects
	Vec2 d = (b.position - a.position);

	float distanceSquared = d.MagnitudeSquared();
	//Clamp the value of the distance(to allow for some interesting visual effects)
	distanceSquared = std::clamp(distanceSquared , minDistance, maxDistance);
	//Calculate the direction of the attraction force
	Vec2 attractionDirection = d.UnitVector();
	//Calcualte the strength of the attraction force
	float attractionMagnitude = G * (a.mass * b.mass) / distanceSquared;
	//Calcualte the final resulting attraction force vector
	Vec2 attractionForce = attractionDirection * attractionMagnitude;
	return attractionForce;
}

