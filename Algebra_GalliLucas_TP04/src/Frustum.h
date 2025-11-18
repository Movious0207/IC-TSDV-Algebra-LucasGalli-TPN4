#pragma once

#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "Plane.h"
#include "Figure.h"

namespace frustum
{
	const int maxVertices = 8;

	class Frustum
	{
	public:

		float width;
		float height;
		float aspectRatio;
		float fov;
		float fovy;
		float near;
		float far;

		Vector3 cameraDir;

		Vector3 vertices[maxVertices];

		std::vector<plane::Plane> planes;
		int maxPlanes;
		
		Frustum(Camera camera, float width, float height, float fov, float near, float far);
		~Frustum();

		void setPlanes();
		void updatePlanes();
		void updatePos(Camera camera);
		void update(std::vector<figure::Figure*> figures, int maxFigures);
		void draw(Camera camera);
	};
}

