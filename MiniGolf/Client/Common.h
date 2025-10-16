#pragma once
#include <raylib.h>
#include <box2d/box2d.h>

const float PIXELS_PER_METER = 32.0f;  // Defines a constant for converting between Box2D's meters and pixels on the screen.
const float TIME_STEP = 1.0f / 60.0f; // Defines the physics world's time step, typically set to 60 times per second (1/60 seconds).

const int VELOCITY_ITERATIONS = 8; // Higher numbers can improve stability and accuracy at the cost of performance.
const int POSITION_ITERATIONS = 3; 

// Utility function to convert a Box2D body's position to a Raylib position vector, taking into account the screen's height and scaling factor.
static Vector2 getRaylibPosition(b2Body* body) {
	b2Vec2 pos = body->GetPosition(); // Gets the Box2D body's position in meters.

	// Converts the position to pixels and adjusts the y-coordinate to match Raylib's coordinate system.
	// Raylib's y=0 is at the top of the screen, but Box2D considers y=0 at the bottom.

	return Vector2{ pos.x * PIXELS_PER_METER, GetScreenHeight() - pos.y * PIXELS_PER_METER };
}

