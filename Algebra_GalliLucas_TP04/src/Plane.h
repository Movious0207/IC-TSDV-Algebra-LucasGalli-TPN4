#pragma once

#include "raymath.h"

namespace plane
{
	class Plane
	{
	public:
		Vector3 norm;
		Vector3 pos;

		Plane(Vector3 p1, Vector3 p2, Vector3 p3);
		~Plane();

		float getDotProduct(Vector3 point);
	};
}

