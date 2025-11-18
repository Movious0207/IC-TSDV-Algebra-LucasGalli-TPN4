#include "BoundingBox.h"

namespace boundingBox
{
	const Color linesColor = YELLOW;

	BoundingBox::BoundingBox() : color(linesColor)
	{
		max = { 0.0f,0.0f,0.0f };
		min = { 0.0f,0.0f,0.0f };
		maxVertices = 0;
	}
	BoundingBox::~BoundingBox() {}

	void BoundingBox::init(std::vector <Vector3>& figureVertices, int figureMaxVert)
	{
		Vector3 minAux = { INT_MAX,INT_MAX,INT_MAX };
		Vector3 maxAux = { INT_MIN,INT_MIN,INT_MIN };

		for (int i = 0; i < figureMaxVert; i++)
		{
			minAux.x = fminf(minAux.x, figureVertices[i].x);
			minAux.y = fminf(minAux.y, figureVertices[i].y);
			minAux.z = fminf(minAux.z, figureVertices[i].z);

			maxAux.x = fmaxf(maxAux.x, figureVertices[i].x);
			maxAux.y = fmaxf(maxAux.y, figureVertices[i].y);
			maxAux.z = fmaxf(maxAux.z, figureVertices[i].z);
		}

		min = minAux;
		max = maxAux;

		maxVertices = 8;

		vertices[0] = { min.x, min.y, min.z };
		vertices[1] = { max.x, min.y, min.z };
		vertices[2] = { max.x, max.y, min.z };
		vertices[3] = { min.x, max.y, min.z };
		vertices[4] = { min.x, min.y, max.z };
		vertices[5] = { max.x, min.y, max.z };
		vertices[6] = { max.x, max.y, max.z };
		vertices[7] = { min.x, max.y, max.z};

		setPlanes();
	}

	void BoundingBox::setPlanes()
	{
		//atras
		planes.push_back(plane::Plane(vertices[0], vertices[1], vertices[2]));
		planes.push_back(plane::Plane(vertices[0], vertices[2], vertices[3]));
		maxPlanes += 2;

		//adelante
		planes.push_back(plane::Plane(vertices[5], vertices[4], vertices[7]));
		planes.push_back(plane::Plane(vertices[5], vertices[7], vertices[6]));
		maxPlanes += 2;

		//izq
		planes.push_back(plane::Plane(vertices[4], vertices[0], vertices[3]));
		planes.push_back(plane::Plane(vertices[4], vertices[3], vertices[7]));
		maxPlanes += 2;

		//der
		planes.push_back(plane::Plane(vertices[1], vertices[5], vertices[6]));
		planes.push_back(plane::Plane(vertices[1], vertices[6], vertices[2]));
		maxPlanes += 2;

		//abajo
		planes.push_back(plane::Plane(vertices[4], vertices[5], vertices[1]));
		planes.push_back(plane::Plane(vertices[4], vertices[1], vertices[0]));
		maxPlanes += 2;

		//arriba
		planes.push_back(plane::Plane(vertices[3], vertices[2], vertices[6]));
		planes.push_back(plane::Plane(vertices[3], vertices[6], vertices[7]));
		maxPlanes += 2;
	}

	void BoundingBox::updatePlanes()
	{
		//atras
		planes[0] = plane::Plane(vertices[0], vertices[1], vertices[2]);
		planes[1] = plane::Plane(vertices[0], vertices[2], vertices[3]);

		//adelante
		planes[2] = plane::Plane(vertices[5], vertices[4], vertices[7]);
		planes[3] = plane::Plane(vertices[5], vertices[7], vertices[6]);

		//izq
		planes[4] = plane::Plane(vertices[4], vertices[0], vertices[3]);
		planes[5] = plane::Plane(vertices[4], vertices[3], vertices[7]);

		//der
		planes[6] = plane::Plane(vertices[1], vertices[5], vertices[6]);
		planes[7] = plane::Plane(vertices[1], vertices[6], vertices[2]);

		//abajo
		planes[8] = plane::Plane(vertices[4], vertices[5], vertices[1]);
		planes[9] = plane::Plane(vertices[4], vertices[1], vertices[0]);

		//arriba
		planes[10] = plane::Plane(vertices[3], vertices[2], vertices[6]);
		planes[11] = plane::Plane(vertices[3], vertices[6], vertices[7]);
	}

	void BoundingBox::updateValues(std::vector <Vector3>& figureVertices, int figureMaxVert)
	{
		Vector3 minAux = { INT_MAX,INT_MAX,INT_MAX };
		Vector3 maxAux = { INT_MIN,INT_MIN,INT_MIN };

		for (int i = 0; i < figureMaxVert; i++)
		{
			minAux.x = fminf(minAux.x, figureVertices[i].x);
			minAux.y = fminf(minAux.y, figureVertices[i].y);
			minAux.z = fminf(minAux.z, figureVertices[i].z);

			maxAux.x = fmaxf(maxAux.x, figureVertices[i].x);
			maxAux.y = fmaxf(maxAux.y, figureVertices[i].y);
			maxAux.z = fmaxf(maxAux.z, figureVertices[i].z);
		}

		min = minAux;
		max = maxAux;

		vertices[0] = min;
		vertices[1] = min + Vector3{ 0.0f,0.0f,max.z - min.z };
		vertices[2] = max - Vector3{ 0.0f,max.y - min.y,0.0f };
		vertices[3] = min + Vector3{ max.x - min.x,0.0f,0.0f };
		vertices[4] = min + Vector3{ 0.0f,max.y - min.y,0.0f };
		vertices[5] = max - Vector3{ max.x - min.x,0.0f,0.0f };
		vertices[6] = max;
		vertices[7] = max - Vector3{ 0.0f,0.0f, max.z - min.z };

		updatePlanes();
	}

	bool BoundingBox::isPointCol(Vector3 point)
	{
		bool isInside = true;

		for (int i = 0; i < maxPlanes; i++)
		{
			Vector3 diff = point - planes[i].pos;

			if (Vector3DotProduct(diff, planes[i].norm) < 0)
			{
				return false;
			}
		}

		return isInside;
	}

	void BoundingBox::draw()
	{
		DrawLine3D(vertices[0], vertices[1], color);
		DrawLine3D(vertices[1], vertices[2], color);
		DrawLine3D(vertices[2], vertices[3], color);
		DrawLine3D(vertices[3], vertices[0], color);
		DrawLine3D(vertices[4], vertices[5], color);
		DrawLine3D(vertices[5], vertices[6], color);
		DrawLine3D(vertices[6], vertices[7], color);
		DrawLine3D(vertices[7], vertices[4], color);
		DrawLine3D(vertices[4], vertices[0], color);
		DrawLine3D(vertices[5], vertices[1], color);
		DrawLine3D(vertices[6], vertices[2], color);
		DrawLine3D(vertices[7], vertices[3], color);
	}
}

