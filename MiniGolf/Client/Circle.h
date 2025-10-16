#pragma once  
#include <raylib.h>  
#include <box2d/box2d.h>  
#include <memory>  

class Circle  
{  
private:  
   b2CircleShape shape;  
   float radius;  
   Color color;  
   b2Body* body;

public:  
   Circle(std::shared_ptr<b2World> world, Vector2 centerPosition, float radius, b2BodyType bodyType, Color color, float linearDamping, float angularDamping, float restitution, float friction, float density);  
   void Draw();  

   // Add a getter for body to provide controlled access  
   b2Body* GetBody() const { return body; }  
};
