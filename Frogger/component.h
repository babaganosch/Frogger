#pragma once

#include <set>
#include <vector>
#include "object_pool.h"

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


class RenderComponent : public Component
{
    std::vector<Sprite*> sprites;
    SDL_RendererFlip image_flip;
    unsigned int image_number;
    double image_index;
    double animation_speed;

public:

	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, const char * sprite_name, const double speed);
	virtual void Update(float dt);
	virtual void Destroy();
    
    void AddSprite(const char * sprite_name);
    void SetImageIndex(const unsigned int index);
    void SetImageSpeed(const double speed);
    void SetImageFlip(SDL_RendererFlip flip);
    unsigned int GetImageIndex() {return image_index;};
};


class CollideComponent : public Component
{
	ObjectPool<GameObject> * coll_objects; // collision will be tested with these objects

public:
	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects);
	virtual void Update(float dt);
};

class GroundComponent : public Component
{
    ObjectPool<GameObject> * coll_objects; // collision will be tested with these objects

public:
    virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects);
    virtual void Update(float dt);
};
