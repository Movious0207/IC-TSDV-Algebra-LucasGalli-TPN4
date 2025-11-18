#pragma once

#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "Plane.h"

namespace boundingBox
{
	class BoundingBox
	{
	public:
		Vector3 min;
		Vector3 max;

		Color color;

		Vector3 vertices[8];
		int maxVertices;

		std::vector<plane::Plane> planes;
		int maxPlanes;

		BoundingBox();
		~BoundingBox();

		void init(std::vector <Vector3>& figureVertices, int figureMaxVert);
		void setPlanes();
		void updatePlanes();
		void updateValues(std::vector <Vector3>& figureVertices, int figureMaxVert);
		bool isPointCol(Vector3 point);
		void draw();
	};
}

