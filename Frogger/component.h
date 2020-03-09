#pragma once

#include <set>
#include <vector>
#include "object_pool.h"
#include "game_object.h"

class GameObject;
class AvancezLib;
class Sprite;

class Component
{
protected:
	AvancezLib * engine;	// used to access the engine
	GameObject * go;		// the game object this component is part of
	std::set<GameObject*> * game_objects;	// the global container of game objects

public:
	virtual ~Component() {}

	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects);

	virtual void Init() {}
	virtual void Update(float dt) = 0;
	virtual void Receive(int message) {}
	virtual void Destroy() {}
};

enum AnimationMode { REGULAR, PING_PONG };

class RenderComponent : public Component
{
    std::vector<Sprite*> sprites;
    unsigned int image_number;
    AnimationMode animation_mode;
    double image_index;
    double animation_speed;

public:

	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, const char * sprite_name, const double speed);
	virtual void Update(float dt);
	virtual void Destroy();
    
    void AddSprite(const char * sprite_name);
    void SetImageIndex(const unsigned int index);
    void SetImageSpeed(const double speed);
    void SetAnimationMode(AnimationMode mode);
    unsigned int GetImageIndex() {return image_index;};
};


class CollideComponent : public Component
{
	ObjectPool<GameObject> * coll_objects; // collision will be tested with these objects
    Message msg;

public:
	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects, Message messageType);
	virtual void Update(float dt);
};

class SingleCollideComponent : public Component
{
    GameObject * coll_object; // collision will be tested with these objects
    Message msg;

public:
    virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, GameObject * coll_object, Message messageType);
    virtual void Update(float dt);
};
