#include <vector>
#include <iostream>

#include "raylib.h"

#include "Figure.h"
#include "Frustum.h"

enum class changeFrustum
{
	None,
	Fov,
	Near,
	Far,
	Width,
	Height
};

void cameraMove(Camera3D& camera, frustum::Frustum& frustum, bool& isCursorOn, float delta);
void drawControls(changeFrustum currentState);
void drawWorldLines(Vector3 origin);
void figuresUpdate(std::vector<figure::Figure*> figures, int maxFigures);
void figuresDraw(std::vector<figure::Figure*> figures, int maxFigures);
void input(changeFrustum& currentState, bool& shouldDrawControls);
void changeFrustumValues(frustum::Frustum& frustum, changeFrustum currentState, float delta);

int main()
{
	const float cameraDistance = 5.0f;
	const float rotationAngle = 0.05f;

	InitWindow(1280, 720, "TP_04");

	int maxFigures = 0;
	std::vector<figure::Figure*> figures;

	figures.push_back(new figure::Figure("Cube", "res/models/cube.obj", { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f, 0.0f }, 0.05f * RAD2DEG, WHITE));
	maxFigures++;

	figures.push_back(new figure::Figure("Deca", "res/models/decahedron.obj", { 3.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f, 0.0f }, 0.05f * RAD2DEG, WHITE));
	maxFigures++;

	figures.push_back(new figure::Figure("Octa", "res/models/octahedron.obj", { -3.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f, 0.0f }, 0.05f * RAD2DEG, WHITE));
	maxFigures++;

	Vector3 origin = { 0,0,0 };
	Camera3D camera = { 0 };

	camera.position = { cameraDistance, cameraDistance, cameraDistance };  // Camera position
	camera.target = origin;						// Camera looking at point
	camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

	changeFrustum currentState = changeFrustum::None;
	bool shouldDrawControls = true;

	frustum::Frustum frustum = frustum::Frustum(camera, 1920.0f, 1080.0f, 60.0f * DEG2RAD, 0.1f, 50.f);

	DisableCursor();

	bool isCursorOn = false;
	float delta = 0.0f;

	while (!WindowShouldClose())
	{
		//Update
		delta = GetFrameTime();
		cameraMove(camera, frustum, isCursorOn, delta);
		input(currentState, shouldDrawControls);
		frustum.updatePos(camera);
		figuresUpdate(figures, maxFigures);
		frustum.update(figures, maxFigures);
		changeFrustumValues(frustum, currentState, delta);

		//Draw
		
		BeginDrawing();
		BeginMode3D(camera);

		DrawLine3D(frustum.vertices[0], frustum.vertices[1], GREEN);
		DrawLine3D(frustum.vertices[1], frustum.vertices[2], GREEN);
		DrawLine3D(frustum.vertices[2], frustum.vertices[3], GREEN);
		DrawLine3D(frustum.vertices[3], frustum.vertices[0], GREEN);

		DrawLine3D(frustum.vertices[4], frustum.vertices[5], GREEN);
		DrawLine3D(frustum.vertices[5], frustum.vertices[6], GREEN);
		DrawLine3D(frustum.vertices[6], frustum.vertices[7], GREEN);
		DrawLine3D(frustum.vertices[7], frustum.vertices[4], GREEN);

		DrawLine3D(frustum.vertices[0], frustum.vertices[4], GREEN);
		DrawLine3D(frustum.vertices[1], frustum.vertices[5], GREEN);
		DrawLine3D(frustum.vertices[2], frustum.vertices[6], GREEN);
		DrawLine3D(frustum.vertices[3], frustum.vertices[7], GREEN);

		ClearBackground(BLACK);

		drawWorldLines(origin);
		figuresDraw(figures, maxFigures);
		frustum.draw(camera);

		EndMode3D();

		DrawText("TAB toggle controls", 40, 720 - 40, 20, WHITE);
		DrawText("ALT toggle camera move", 40, 720 - 70, 20, WHITE);

		if (shouldDrawControls)
		{
			drawControls(currentState);
		}

		EndDrawing();
	}
	CloseWindow();

	return 0;
}

void input(changeFrustum& currentState, bool& shouldDrawControls)
{
	if (IsKeyPressed(KEY_TAB))
	{
		shouldDrawControls = !shouldDrawControls;
	}

	if (IsKeyDown(KEY_C))
	{
		currentState = changeFrustum::Fov;
	}
	else if (IsKeyDown(KEY_N))
	{
		currentState = changeFrustum::Near;
	}
	else if (IsKeyDown(KEY_F))
	{
		currentState = changeFrustum::Far;
	}
	else if (IsKeyDown(KEY_V))
	{
		currentState = changeFrustum::Width;
	}
	else if (IsKeyDown(KEY_H))
	{
		currentState = changeFrustum::Height;
	}
	else
	{
		currentState = changeFrustum::None;
	}
}

void changeFrustumValues(frustum::Frustum& frustum, changeFrustum currentState, float delta)
{
	if (currentState != changeFrustum::None)
	{
		float change = 0.0f;
		const float changePerFrame = 1.0f;

		if (IsKeyDown(KEY_KP_ADD))
		{
			change += changePerFrame * delta;
		}
		else if (IsKeyDown(KEY_KP_SUBTRACT))
		{
			change -= changePerFrame * delta;
		}

		if (change != 0)
		{
			switch (currentState)
			{
			case changeFrustum::Fov:
				frustum.fov += change;
				break;

			case changeFrustum::Near:
				if (frustum.near + change >= 0.05 && frustum.near + change < frustum.far)
				{
					frustum.near += change;
				}
				break;

			case changeFrustum::Far:
				if (frustum.far + change >= 0.05 && frustum.far + change > frustum.near)
				{
					frustum.far += change;
				}
				break;

			case changeFrustum::Width:
				if (frustum.width + change *100> 0.0f)
				{
					frustum.width += change * 100;
				}
				break;

			case changeFrustum::Height:
				if (frustum.height + change * 100 > 0.0f)
				{
					frustum.height += change * 100;
				}
				break;

			default:
				break;
			}
		}
	}

}

void cameraMove(Camera3D& camera, frustum::Frustum& frustum, bool& isCursorOn, float delta)
{
	if (IsKeyPressed(KEY_LEFT_ALT))
	{
		isCursorOn = !isCursorOn;

		if (isCursorOn)
		{
			EnableCursor();
		}
		else
		{
			DisableCursor();
		}
	}

	if (!isCursorOn)
	{
		UpdateCamera(&camera, CAMERA_FREE);
		frustum.updatePos(camera);
	}
}

void drawControls(changeFrustum currentState)
{
	DrawText("WASD move camera", 40, 40, 20, WHITE);
	DrawText("SPACE BAR camera up", 40, 70, 20, WHITE);
	DrawText("CTRL camera down", 40, 100, 20, WHITE);

	DrawText("+ & - (NUMPAD) change frustum values", 40, 130, 20, WHITE);

	Color fovColor = WHITE;
	Color nearColor = WHITE;
	Color farColor = WHITE;
	Color widthColor = WHITE;
	Color heightColor = WHITE;

	switch (currentState)
	{
	case changeFrustum::Fov:
		fovColor = GREEN;
		break;
	case changeFrustum::Near:
		nearColor = GREEN;
		break;
	case changeFrustum::Far:
		farColor = GREEN;
		break;
	case changeFrustum::Width:
		widthColor = GREEN;
		break;
	case changeFrustum::Height:
		heightColor = GREEN;
		break;
	default:
		break;
	}

	DrawText("C (HOLD) select FOV", 40, 160, 20, fovColor);
	DrawText("N (HOLD) select Near", 40, 190, 20, nearColor);
	DrawText("F (HOLD) select far", 40, 220, 20, farColor);
	DrawText("V (HOLD) width", 40, 250, 20, widthColor);
	DrawText("H (HOLD) height", 40, 280, 20, heightColor);
}

void drawWorldLines(Vector3 origin)
{
	DrawGrid(100, 5);
	DrawLine3D(origin, { 10,0,0 }, RED);
	DrawLine3D(origin, { 0,10,0 }, GREEN);
	DrawLine3D(origin, { 0,0,10 }, BLUE);
}

void figuresUpdate(std::vector<figure::Figure*> figures, int maxFigures)
{
	for (int i = 0; i < maxFigures; i++)
	{

	}
}

void figuresDraw(std::vector<figure::Figure*> figures, int maxFigures)
{
	for (int i = 0; i < maxFigures; i++)
	{
		if (figures[i]->isDrawable)
		{
			figures[i]->draw();
		}
	}
}