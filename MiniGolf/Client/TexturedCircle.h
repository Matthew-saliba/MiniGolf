#pragma once
#include <raylib.h>
#include <box2d/box2d.h>
#include <memory>
#include "Common.h"
#include "raymath.h"

class TexturedCircle
{
private:  
    b2CircleShape shape;
    b2Body* body;
    Texture2D texture;

public:
    TexturedCircle(std::shared_ptr<b2World> world, Vector2 centerPosition, Texture2D texture, b2BodyType bodyType, float linearDamping, float angularDamping, float restitution, float friction, float density);
    void Draw();
};

