#include "Figure.h"

namespace figure
{
	Figure::Figure(std::string name, std::string modelRef, Vector3 pos, Vector3 scale, Vector3 rotationAxis, float rotationAngle, Color color) : name(name), scale(scale), pos(pos), rotationAngle(rotationAngle), color(color)
	{
		isDrawable = false;

		model = LoadModel(modelRef.c_str());

		translateM = MatrixTranslate(pos.x, pos.y, pos.z);
		rotationM = MatrixRotate(rotationAxis, rotationAngle);
		scaleM = MatrixScale(scale.x, scale.y, scale.z);

		initPositions();
	}

	Figure::~Figure() { UnloadModel(model); }

	void Figure::initPositions()
	{
		model.transform = MatrixMultiply(MatrixMultiply(scaleM, rotationM), translateM);

		initVertices();
		boundingBox.init(vertices, maxVertices);
		setPlanes();
	}

	void Figure::initVertices()
	{
		Vector3 vertex = { 0.0f,0.0f,0.0f };

		Mesh modelMesh = model.meshes[0];

		for (int i = 0; i < modelMesh.vertexCount; i++)
		{
			vertex.x = modelMesh.vertices[i * 3];
			vertex.y = modelMesh.vertices[i * 3 + 1];
			vertex.z = modelMesh.vertices[i * 3 + 2];

			vertex = Vector3Transform(vertex, model.transform);

			vertices.push_back(vertex);
			maxVertices++;
		}
	}

	void Figure::updatePositions()
	{
		model.transform = MatrixMultiply(MatrixMultiply(scaleM, rotationM), translateM);

		updateVertices();
		boundingBox.updateValues(vertices, maxVertices);
		updatePlanes();
	}

	void Figure::setPlanes()
	{
		for (int i = 0; i < model.meshes[0].triangleCount; i++)
		{
			plane::Plane auxPlane = plane::Plane(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);

			planes.push_back(auxPlane);
			maxPlanes++;
		}
	}

	void Figure::updateVertices()
	{
		Vector3 vertex = { 0.0f,0.0f,0.0f };

		Mesh modelMesh = model.meshes[0];

		for (int i = 0; i < modelMesh.vertexCount; i++)
		{
			vertex.x = modelMesh.vertices[i * 3];
			vertex.y = modelMesh.vertices[i * 3 + 1];
			vertex.z = modelMesh.vertices[i * 3 + 2];

			vertex = Vector3Transform(vertex, model.transform);

			vertices[i] = vertex;
		}
	}

	void Figure::updatePlanes()
	{
		for (int i = 0; i < model.meshes[0].triangleCount; i++)
		{
			plane::Plane auxPlane = plane::Plane(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);

			planes[i] = auxPlane;
		}
	}

	bool Figure::isPointCol(Vector3 point)
	{
		bool isInside = true;

		for (int i = 0; i < maxPlanes; i++)
		{
			isInside = true;

			Vector3 diff = point - planes[i].pos;

			if (Vector3DotProduct(diff, planes[i].norm) < 0)
			{
				isInside = false;
				break;
			}
		}

		return isInside;
	}

	void Figure::draw()
	{
		DrawModel(model, { 0.0f,0.0f,0.f }, 1, color);

		boundingBox.draw();

		for (int i = 0; i < maxPlanes; i++)
		{
			DrawLine3D(planes[i].pos, planes[i].pos + Vector3Scale(planes[i].norm, 1.0f), RED);
		}


		for (int i = 0; i < boundingBox.planes.size(); i++)
		{
			//DrawLine3D(boundingBox.planes[i].pos, boundingBox.planes[i].pos + Vector3Scale(boundingBox.planes[i].norm, 1.0f), RED);
		}

		DrawLine3D(pos, { 0.0f,0.0f,0.0f }, BROWN);
	}
}

