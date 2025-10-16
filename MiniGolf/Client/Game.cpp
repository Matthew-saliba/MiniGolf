#include "Game.h"
#include "Common.h"
#include "CustomDebugDraw.h"

Game::Game()
{
	InitAudioDevice();

	goalSound = LoadSound("audios/goalSound.mp3");

	world = std::make_shared<b2World>(b2Vec2(0.0f, 0.0f));//gravity vector (0, 0) means gravity that there is no gravity since the game is top down

	// Create the walls of the course

	// Wall 0: Bottom horizontal wall (13 cells wide)
	wall = std::make_shared<Box>(world, Vector2{ 160, 303 }, 32, 192 ,  b2_staticBody, 0 , BROWN , 0, 0, 0, 0, 0);

	// Wall 1: Top horizontal wall (17 cells wide)
	wall1 = std::make_shared<Box>(world, Vector2{ 264, 16 },  464, 32 ,  b2_staticBody, 0 , BROWN, 0, 0, 0, 0, 0);

	// Wall 2: Left vertical wall (13 cells tall)
	wall2 = std::make_shared<Box>(world, Vector2{ 16, 214 },  32, 434 ,  b2_staticBody, 0 , BROWN, 0, 0, 0, 0, 0);

	// Wall 3: Right mid vertical wall (17 cells -> rows 0-8)
	wall3 = std::make_shared<Box>(world, Vector2{ 512, 135 },  32, 272 ,  b2_staticBody, 0 , BROWN, 0, 0, 0, 0, 0);

	// Wall 4: Center horizontal wall (5 cells wide)
	wall4 = std::make_shared<Box>(world, Vector2{ 224, 192 },  160, 32 ,  b2_staticBody, 0 , BROWN, 0, 0, 0, 0, 0);

	// Wall 5: Center vertical wall (7 cells tall)
	wall5 = std::make_shared<Box>(world, Vector2{ 320, 327 },  32, 305 ,  b2_staticBody, 0 , BROWN, 0, 0, 0, 0, 0);

	// Wall 6: Bottom left horizontal wall (6 cells wide)
	wall6 = std::make_shared<Box>(world, Vector2{ 105, 416 },  144, 32 , b2_staticBody, 0 , BROWN, 0, 0, 0, 0, 0);

	// Wall 7: Right side horizontal wall (8 cells wide)
	wall7 = std::make_shared<Box>(world, Vector2{ 640, 256 },  227, 32 ,  b2_staticBody, 0 , BROWN, 0, 0, 0, 0, 0);

	// Wall 8: Right bottom vertical wall (6 cells tall)
	wall8 = std::make_shared<Box>(world, Vector2{ 736, 360 },  32, 178 , b2_staticBody, 0 , BROWN, 0, 0, 0, 0, 0);

	// Wall 9: Bottom horizontal wall (13 cells wide)
	wall9 = std::make_shared<Box>(world, Vector2{ 545, 464 }, 416, 32 , b2_staticBody, 0 , BROWN, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	ball = std::make_shared<Circle>(world , Vector2{80 , 358} , 10 , b2_dynamicBody , RAYWHITE , 1.0f , 1.0f , 0.8f , 0.3f , 0.5f);

	


	// Create an instance of the custom debug drawing class (this is used for visual physics debugging)
	debugDraw = std::make_shared<CustomDebugDraw>();
	debugDraw->DrawSolidOrOutline(CustomDebugDraw::DRAW_OUTLINE); // Set the debug drawing to draw outlines (we can also draw solid shapes)
	debugDraw->SetFlags(b2Draw::e_shapeBit);

	// Set the debug drawing object for the Box2D world
	world->SetDebugDraw(debugDraw.get()); //using .get() to get the raw pointer from the shared pointer (because SetDebugDraw expects a raw pointer)
		
}

void Game::Update()
{
	if (ballsUsed >= 3) {
		return; 
	}
	world->Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

	b2Vec2 velocity = ball->GetBody()->GetLinearVelocity();
	float velocityMagnitude = static_cast<float>(sqrtf(velocity.x * velocity.x + velocity.y * velocity.y));

	Vector2 ballScreenPos = getRaylibPosition(ball->GetBody());
	float ballRadiusPx = ball->GetBody()->GetFixtureList()->GetShape()->m_radius * PIXELS_PER_METER;

	// --- Collision check with static circle at (650, 380) ---
	static bool goalScored = false;
	bool hitGoal = CheckCollisionCircles(ballScreenPos, ballRadiusPx, Vector2{ 650, 380 }, 11);
	if (hitGoal && !goalScored) {
		PlaySound(goalSound);

		// Update best score if this is the first round or a new best
		if (!hasBestScore || score < bestScore) {
			bestScore = score;
			hasBestScore = true;
		}
		score = 0; // Reset score
		goalScored = true;

		// Reset ball to starting position and stop its movement
		b2Body* ballBody = ball->GetBody();
		// Set position (80, 358) in screen coordinates, convert to Box2D world coordinates
		float startX = 80.0f / PIXELS_PER_METER;
		float startY = (GetScreenHeight() - 358.0f) / PIXELS_PER_METER;
		ballBody->SetTransform(b2Vec2(startX, startY), 0.0f);
		ballBody->SetLinearVelocity(b2Vec2(0, 0));
		ballBody->SetAngularVelocity(0);
		ballsUsed++; // Increment the number of balls used
	}
	else if (!hitGoal) {
		goalScored = false;
	}

	if (velocityMagnitude < VELOCITY_THRESHOLD) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 mousePos = GetMousePosition();
			if (CheckCollisionCircles(mousePos, 1, ballScreenPos, ballRadiusPx)) {
				dragStart = mousePos;
				isDragging = true;
			}
		}
	}

	if (isDragging) {
		dragEnd = GetMousePosition();

		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			isDragging = false;

			// Calculate force in screen coordinates
			Vector2 force = {
				(dragStart.x - dragEnd.x),
				(dragStart.y - dragEnd.y)
			};

			// Convert screen force to physics force and apply multiplier
			b2Vec2 physicsForce = b2Vec2(
				force.x * FORCE_MULTIPLIER / PIXELS_PER_METER,
				-force.y * FORCE_MULTIPLIER / PIXELS_PER_METER
			);

			ball->GetBody()->ApplyForceToCenter(physicsForce, true);

			// Increase score every time a force is applied
			score++;
		}
	}
}




void Game::Draw()
{
	if (ballsUsed >= 3) {
		DrawText("Game Over! You used all your balls.", GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 - 20, 20, RED);
		DrawText(TextFormat("Best Score : %d", bestScore), GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 + 20, 20, SKYBLUE);
		return; // Stop drawing if the game is over
	}


	ball->Draw();
	wall->Draw();
	wall1->Draw();
	wall2->Draw();
	wall3->Draw();
	wall4->Draw();
	wall5->Draw();
	wall6->Draw();
	wall7->Draw();
	wall8->Draw();
	wall9->Draw();


	//End of the course
	DrawCircle(650, 380, 12, BLACK);

	if (isDragging) {
		DrawLineEx(dragStart, dragEnd, 2, RED);
	}

	DrawText(TextFormat("Score: %d", score), 10, 10, 30, PURPLE);
	DrawText(TextFormat("Best: %d", hasBestScore ? bestScore : 0), GetScreenWidth() - 120, 10, 30, SKYBLUE);

}



