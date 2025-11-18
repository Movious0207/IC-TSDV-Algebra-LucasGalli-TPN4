#include "Frustum.h"

#include <iostream>

namespace frustum
{
	Frustum::Frustum(Camera camera, float width, float height, float fov, float near, float far) : width(width), height(height), fov(fov), near(near), far(far)
	{
		maxPlanes = 0;

		cameraDir = Vector3Normalize(camera.target - camera.position);

		fovy = fov * (height / width);

		aspectRatio = width / height;

		float halfFarHeight = tanf(fovy / 2) * far;
		float halfFarWidth = halfFarHeight * aspectRatio;

		Vector3 cameraLeft = Vector3Normalize(Vector3CrossProduct(camera.up, cameraDir));

		Vector3 cameraUp = Vector3Normalize(Vector3CrossProduct(cameraLeft, cameraDir));
		cameraLeft = Vector3Normalize(Vector3CrossProduct(cameraDir, cameraUp));
		Vector3 cameraRight = cameraLeft * -1;
		Vector3 cameraDown = cameraUp * -1;

		std::cout << cameraUp.x << " " << cameraUp.y << " " << cameraUp.z << std::endl;

		vertices[0] = camera.position + (cameraDir * far) + (cameraDown * halfFarHeight) + (cameraLeft * halfFarWidth);
		vertices[1] = camera.position + (cameraDir * far) + (cameraDown * halfFarHeight) + (cameraRight * halfFarWidth);
		vertices[2] = camera.position + (cameraDir * far) + (cameraUp * halfFarHeight) + (cameraRight * halfFarWidth);
		vertices[3] = camera.position + (cameraDir * far) + (cameraUp * halfFarHeight) + (cameraLeft * halfFarWidth);

		float halfNearHeight = tanf(fovy / 2) * near;
		float halfNearWidth = halfNearHeight * aspectRatio;

		vertices[4] = camera.position + (cameraDir * near) + (cameraDown * halfNearHeight) + (cameraLeft * halfNearWidth);
		vertices[5] = camera.position + (cameraDir * near) + (cameraDown * halfNearHeight) + (cameraRight * halfNearWidth);
		vertices[6] = camera.position + (cameraDir * near) + (cameraUp * halfNearHeight) + (cameraRight * halfNearWidth);
		vertices[7] = camera.position + (cameraDir * near) + (cameraUp * halfNearHeight) + (cameraLeft * halfNearWidth);

		setPlanes();
	}

	Frustum::~Frustum() {}

	void Frustum::setPlanes()
	{
		planes.push_back(plane::Plane(vertices[0], vertices[1], vertices[2]));
		planes.push_back(plane::Plane(vertices[0], vertices[2], vertices[3]));
		maxPlanes += 2;

		//adelante
		planes.push_back(plane::Plane(vertices[5], vertices[4], vertices[7]));
		planes.push_back(plane::Plane(vertices[5], vertices[7], vertices[6]));
		maxPlanes += 2;

		//left
		planes.push_back(plane::Plane(vertices[4], vertices[0], vertices[3]));
		planes.push_back(plane::Plane(vertices[4], vertices[3], vertices[7]));
		maxPlanes += 2;

		//right
		planes.push_back(plane::Plane(vertices[1], vertices[5], vertices[6]));
		planes.push_back(plane::Plane(vertices[1], vertices[6], vertices[2]));
		maxPlanes += 2;

		//bottom
		planes.push_back(plane::Plane(vertices[4], vertices[5], vertices[1]));
		planes.push_back(plane::Plane(vertices[4], vertices[1], vertices[0]));
		maxPlanes += 2;

		//top
		planes.push_back(plane::Plane(vertices[3], vertices[2], vertices[6]));
		planes.push_back(plane::Plane(vertices[3], vertices[6], vertices[7]));
		maxPlanes += 2;
	}

	void Frustum::updatePlanes()
	{
		//far
		planes[0] = (plane::Plane(vertices[0], vertices[1], vertices[2]));
		planes[1] = (plane::Plane(vertices[0], vertices[2], vertices[3]));

		//near
		planes[2] = (plane::Plane(vertices[5], vertices[4], vertices[7]));
		planes[3] = (plane::Plane(vertices[5], vertices[7], vertices[6]));

		//izq
		planes[4] = (plane::Plane(vertices[4], vertices[0], vertices[3]));
		planes[5] = (plane::Plane(vertices[4], vertices[3], vertices[7]));

		//der
		planes[6] = (plane::Plane(vertices[1], vertices[5], vertices[6]));
		planes[7] = (plane::Plane(vertices[1], vertices[6], vertices[2]));

		//abajo
		planes[8] = (plane::Plane(vertices[4], vertices[5], vertices[1]));
		planes[9] = (plane::Plane(vertices[4], vertices[1], vertices[0]));

		//arriba
		planes[10] = (plane::Plane(vertices[3], vertices[2], vertices[6]));
		planes[11] = (plane::Plane(vertices[3], vertices[6], vertices[7]));
	}

	void Frustum::updatePos(Camera camera)
	{
		cameraDir = Vector3Normalize(camera.target - camera.position);

		aspectRatio = width / height;

		fovy = fov / aspectRatio;

		float halfFarHeight = tanf(fovy / 2) * far;
		float halfFarWidth = halfFarHeight * aspectRatio;

		Vector3 cameraLeft = Vector3Normalize(Vector3CrossProduct(camera.up, cameraDir));

		Vector3 cameraUp = Vector3Normalize(Vector3CrossProduct(cameraDir, cameraLeft));
		cameraLeft = Vector3Normalize(Vector3CrossProduct(cameraUp, cameraDir));
		Vector3 cameraRight = cameraLeft * -1;
		Vector3 cameraDown = cameraUp * -1;

		std::cout << cameraUp.x << " " << cameraUp.y << " " << cameraUp.z << std::endl;

		vertices[0] = camera.position + (cameraDir * far) + (cameraDown * halfFarHeight) + (cameraLeft * halfFarWidth);
		vertices[1] = camera.position + (cameraDir * far) + (cameraDown * halfFarHeight) + (cameraRight * halfFarWidth);
		vertices[2] = camera.position + (cameraDir * far) + (cameraUp * halfFarHeight) + (cameraRight * halfFarWidth);
		vertices[3] = camera.position + (cameraDir * far) + (cameraUp * halfFarHeight) + (cameraLeft * halfFarWidth);

		float halfNearHeight = tanf(fovy / 2) * near;
		float halfNearWidth = halfNearHeight * aspectRatio;

		vertices[4] = camera.position + (cameraDir * near) + (cameraDown * halfNearHeight) + (cameraLeft * halfNearWidth);
		vertices[5] = camera.position + (cameraDir * near) + (cameraDown * halfNearHeight) + (cameraRight * halfNearWidth);
		vertices[6] = camera.position + (cameraDir * near) + (cameraUp * halfNearHeight) + (cameraRight * halfNearWidth);
		vertices[7] = camera.position + (cameraDir * near) + (cameraUp * halfNearHeight) + (cameraLeft * halfNearWidth);

		updatePlanes();
	}

	void Frustum::update(std::vector<figure::Figure*> figures, int maxFigures)
	{
		for (int i = 0; i < maxFigures; i++)
		{
			figures[i]->isDrawable = false;

			for (int j = 0; j < figures[i]->boundingBox.maxVertices; j++)
			{
				bool isInsideBB = true;

				for (int l = 0; l < maxPlanes; l++)
				{
					if (planes[l].getDotProduct(figures[i]->boundingBox.vertices[j]) < 0)
					{
						isInsideBB = false;
						break;
					}
				}

				if (isInsideBB)
				{
					for (int k = 0; k < figures[i]->maxVertices; k++)
					{
						bool isInside = true;

						for (int l = 0; l < maxPlanes; l++)
						{
							if (planes[l].getDotProduct(figures[i]->vertices[k]) < 0)
							{
								isInside = false;
								break;
							}
						}

						if (isInside)
						{
							figures[i]->isDrawable = true;
							break;
						}
					}
				}

				if (figures[i]->isDrawable)
				{
					break;
				}

			}
		}
	}

	void Frustum::draw(Camera camera)
	{
		DrawSphere(vertices[0], 1.00f, GREEN);
		DrawSphere(vertices[1], 1.00f, GREEN);
		DrawSphere(vertices[2], 1.00f, GREEN);
		DrawSphere(vertices[3], 1.00f, GREEN);
		//
		DrawSphere(vertices[4], 0.0005f, VIOLET);
		DrawSphere(vertices[5], 0.0005f, VIOLET);
		DrawSphere(vertices[6], 0.0005f, VIOLET);
		DrawSphere(vertices[7], 0.0005f, VIOLET);

		for (int i = 0; i < maxPlanes; i++)
		{
			//DrawLine3D(planes[i].pos, planes[i].norm, PINK);
		}
	}
}