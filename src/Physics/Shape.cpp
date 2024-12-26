#include "Shape.h"
#include <iostream>

CircleShape :: CircleShape(float radius) {
	this->radius = radius;
	std::cout << "CircleShape constructor called" << std::endl;
};

CircleShape :: ~CircleShape() {
	std::cout << "CircleShape destructor called" << std::endl;
};

Shape* CircleShape::Clone() const {
	return new CircleShape(radius);
};
ShapeType CircleShape::GetType() const {
	return CIRCLE;
};

float CircleShape::GetMomentofInertia() const {
	//For solid circles the moment of inertia is 1/2 * r^2
	//But this still needs to be multiplied by the rigidbody's mass
	return 0.5 * (radius * radius);
};

PolygonShape::PolygonShape(const std::vector<Vec2> vertices) {
//TODO
};

PolygonShape :: ~PolygonShape() {
	std::cout << "PolygonShape destructor called" << std::endl;
};

ShapeType PolygonShape::GetType() const {
	return POLYGON;
};

Shape* PolygonShape::Clone() const {
	return new PolygonShape(localVertices);
}

float PolygonShape::GetMomentofInertia() const {
	//todo
	return 0.0;
};

void PolygonShape::updateVertices(float angle, const Vec2& position) {
	//Loop all the vertices ,  transform from local to world space
	for (int i = 0; i < localVertices.size(); i++) {
		//rotate vector
		worldVertices[i] = localVertices[i].Rotate(angle);
		//translate vector
		worldVertices[i] += position;
	}
}

BoxShape::BoxShape(float widht, float height) {
	std::cout << "BoxShape constructor called" << std::endl;
	this->width = width;
	this->height = height;

	// Load vertices of the box polygon
	localVertices.push_back(Vec2(-widht / 2.0, -height / 2.0));
	localVertices.push_back(Vec2(+widht / 2.0, -height / 2.0));
	localVertices.push_back(Vec2(+width / 2.0, +height / 2.0));
	localVertices.push_back(Vec2(-width / 2.0, +height / 2.0));

	worldVertices.push_back(Vec2(-widht / 2.0, -height / 2.0));
	worldVertices.push_back(Vec2(+widht / 2.0, -height / 2.0));
	worldVertices.push_back(Vec2(+width / 2.0, +height / 2.0));
	worldVertices.push_back(Vec2(-width / 2.0, +height / 2.0));

};

BoxShape :: ~BoxShape() {
	std::cout << "BoxShape destructor called" << std::endl;
};

ShapeType BoxShape::GetType() const {
	return BOX;
};

Shape* BoxShape::Clone()const {
	return new BoxShape(width, height);
};

float BoxShape::GetMomentofInertia()const {
	//For a rectangle, the momet of inertia is 1/12 * (w^2 + h^2)
	//But this still  needs to be multiplied by the rigidbody's mass 
	return (0.83333) * (width * width + height * height);
}