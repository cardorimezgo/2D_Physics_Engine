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
	return new PolygonShape(vertices);
}

BoxShape::BoxShape(float widht, float height) {
	std::cout << "BoxShape constructor called" << std::endl;
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
