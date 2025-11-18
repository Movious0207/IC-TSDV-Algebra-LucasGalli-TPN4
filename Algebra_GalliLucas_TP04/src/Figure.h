#pragma once

#include <String>

#include "raylib.h"
#include "raymath.h"

#include "BoundingBox.h"

namespace figure
{
class Figure
{
public:

	bool isDrawable;

	std::string name;
	Model model;
	Vector3 scale;
	Vector3 pos;
	Color color;
	float rotationAngle;

	Matrix translateM;
	Matrix rotationM;
	Matrix scaleM;

	std::vector<Vector3> vertices;
	int maxVertices;

	boundingBox::BoundingBox boundingBox;
	bool isCollidingBoundingBox;

	std::vector<plane::Plane> planes;
	int maxPlanes;

	Figure(std::string name, std::string modelRef, Vector3 pos, Vector3 scale, Vector3 rotationAxis, float rotationAngle, Color color);
	~Figure();

	void initPositions();
	void initVertices();
	void updatePositions();
	void setPlanes();
	void updateVertices();
	void updatePlanes();
	void draw();
	bool isPointCol(Vector3 point);
};

}

