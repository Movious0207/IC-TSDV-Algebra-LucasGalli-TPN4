#include "Plane.h"

namespace plane
{
	Plane::Plane(Vector3 p1, Vector3 p2, Vector3 p3) 
	{
		Vector3 line1 = p2 - p1;
		Vector3 line2 = p3 - p1;

		norm = Vector3Normalize(Vector3CrossProduct(line1,line2));

		pos = p1;
	}
	Plane::~Plane() {}

	float Plane::getDotProduct(Vector3 point)
	{
		Vector3 diff = point - pos;

		return Vector3DotProduct(diff, norm);
	}
}
