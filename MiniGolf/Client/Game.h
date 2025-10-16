#pragma once
#include <raylib.h>
#include <box2d/box2d.h>
#include <memory>
#include "Circle.h"
#include "Box.h"
#include "TexturedBox.h"
#include "TexturedCircle.h"
#include "CustomDebugDraw.h"

class Game
{
private:
	//Scoring variables
	int score = 0;
	int bestScore = 0;
	bool hasBestScore = false;
	int ballsUsed = 0;

	Sound goalSound;

	//Physics constants
	Vector2 dragStart = { 0, 0 };
	Vector2 dragEnd = { 0, 0 };
	bool isDragging = false;
	const float VELOCITY_THRESHOLD = 0.1f;
	const float FORCE_MULTIPLIER = 20.0f;


	//Physics world
	std::shared_ptr<b2World> world;

	//Game objects
	//Walls of the course
	std::shared_ptr<Box> wall;
	std::shared_ptr<Box> wall1;
	std::shared_ptr<Box> wall2;
	std::shared_ptr<Box> wall3;
	std::shared_ptr<Box> wall4;
	std::shared_ptr<Box> wall5;
	std::shared_ptr<Box> wall6;
	std::shared_ptr<Box> wall7;
	std::shared_ptr<Box> wall8;
	std::shared_ptr<Box> wall9;


	//Golf ball
	std::shared_ptr<Circle> ball;


	//Debug draw
	std::shared_ptr<CustomDebugDraw> debugDraw;

public:
	Game();
	void Update();
	void Draw();
	~Game() {
		UnloadSound(goalSound);
		CloseAudioDevice();
	}
};

