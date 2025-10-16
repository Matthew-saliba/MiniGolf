#include "Box.h"
#include "Common.h"

Box::Box(std::shared_ptr<b2World> world, Vector2 centerPosition, float width, float height, b2BodyType bodyType, float angle, Color color, float linearDamping, float angularDamping, float restitution, float friction, float density)
    : width(width), height(height), color(color) {
    
    b2BodyDef bodyDef;
    bodyDef.type = bodyType; // Type of the box body (dynamic, static, or kinematic).
    bodyDef.position.Set(centerPosition.x / PIXELS_PER_METER, (GetScreenHeight() - centerPosition.y) / PIXELS_PER_METER);
    bodyDef.angle = angle * DEG2RAD; // Convert angle from degrees to radians.
    bodyDef.linearDamping = linearDamping;
    bodyDef.angularDamping = angularDamping;

    body = world->CreateBody(&bodyDef); // Create the body in the Box2D world.

    shape.SetAsBox((width / 2.0f) / PIXELS_PER_METER, (height / 2.0f) / PIXELS_PER_METER);  // Set the shape of the body to a box, adjusting size for the scaling factor.

    b2FixtureDef fixtureDef;  // Define the fixture, which adds physical properties.
    fixtureDef.shape = &shape;
    fixtureDef.density = density;  // Density, affects how mass is calculated.
    fixtureDef.friction = friction; // Friction coefficient, affects how it slides along surfaces.
    fixtureDef.restitution = restitution;  // Bounciness of the box.
    body->CreateFixture(&fixtureDef); // Attach the fixture to the body.

}

void Box::Draw() {
    Vector2 pos = getRaylibPosition(body); // Center position in Raylib coordinates
    float angleDegrees = -(body->GetAngle() * RAD2DEG); // Convert Box2D angle to degrees (Box2D would rotate the rectangle in the opposite direction in Raylib, multiply by negative)

    // The origin for rotation should be the center of the rectangle
    Vector2 origin = { width / 2, height / 2 };

    // Draw the rectangle with its center at 'pos'
    DrawRectanglePro({ pos.x, pos.y, width, height }, origin, angleDegrees, color);
}

