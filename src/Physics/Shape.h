#ifndef SHAPE_H
#define SHAPE_H

#include "Vec2.h"
#include <vector>

enum ShapeType {
	CIRCLE,
	POLYGON,
	BOX
};

struct Shape{
	virtual ~Shape() = default;
	virtual ShapeType GetType() const = 0;
	virtual Shape* Clone() const = 0;
};

struct CircleShape : public Shape {
	float radius;

	CircleShape(const float radius);
	virtual ~CircleShape();
	ShapeType GetType()const override;
	Shape* Clone() const override;
};

struct PolygonShape : public Shape {
	std::vector<Vec2> vertices;

	PolygonShape() = default;
	PolygonShape(const std::vector<Vec2> vertices);
	virtual ~PolygonShape();
	ShapeType GetType() const override;
	Shape* Clone() const override;

};

struct BoxShape : public PolygonShape {
	float width;
	float height;

	BoxShape(float width, float height);
	virtual ~BoxShape();
	ShapeType GetType() const override;
	Shape* Clone() const override;
};

#endif