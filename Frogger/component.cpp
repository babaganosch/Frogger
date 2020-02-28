#include "component.h"
#include "game_object.h"
#include "avancezlib.h"
#include "util.h"
#include <iostream>

void Component::Create(AvancezLib * engine, GameObject * go, std::set<GameObject*>* game_objects)
{
	this->go = go;
	this->engine = engine;
	this->game_objects = game_objects;
}

void RenderComponent::Create(AvancezLib * engine, GameObject * go, std::set<GameObject*>* game_objects, const char * sprite_name, const double speed)
{
	Component::Create(engine, go, game_objects);
    Sprite* sprite = engine->createSprite(sprite_name);
    sprites.push_back(sprite);
    image_number   = 1;
    image_index    = 0.f;
    animation_speed = speed;
    image_flip = SDL_FLIP_NONE;
    /* Setup standard BBOX */
    go->width  = sprite->getImageWidth();
    go->height = sprite->getImageHeight();;
    go->bbox_left  = go->bbox_top = 0;
    go->bbox_right = go->width;
    go->bbox_bot   = go->height;
}

void RenderComponent::AddSprite(const char * sprite_name) {
    sprites.push_back(engine->createSprite(sprite_name));
    image_number++;
}

void RenderComponent::SetImageIndex(const unsigned int index) {
    image_index = clamp(index, 0, image_number-1);
}

void RenderComponent::SetImageSpeed(const double speed) {
    animation_speed = speed;
}

void RenderComponent::SetImageFlip(SDL_RendererFlip flip) {
    image_flip = flip;
}

void RenderComponent::Update(float dt)
{
	if (!go->enabled)
		return;
    
    if (sprites.at(floor(image_index)))
        sprites.at(floor(image_index))->draw(int(go->horizontalPosition), int(go->verticalPosition), image_flip);
    
    image_index = fmod(image_index + (animation_speed * dt), image_number);
}

void RenderComponent::Destroy()
{
    for (Sprite* sprite : sprites) {
        if (sprite != NULL)
            sprite->destroy();
        sprite = NULL;
    }
}


void CollideComponent::Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects)
{
	Component::Create(engine, go, game_objects);
	this->coll_objects = coll_objects;
}


void CollideComponent::Update(float dt)
{
	for (auto i = 0; i < coll_objects->pool.size(); i++)
	{
		GameObject * go0 = coll_objects->pool[i];
		if (go0->enabled)
		{
            /* Other bounding box */
            unsigned int go0_bbox_left  = go0->horizontalPosition + go0->bbox_left;
            unsigned int go0_bbox_right = go0->horizontalPosition + go0->bbox_right;
            unsigned int go0_bbox_top   = go0->verticalPosition   + go0->bbox_top;
            unsigned int go0_bbox_bot   = go0->verticalPosition   + go0->bbox_bot;
            /* Our bounding box */
            unsigned int go_bbox_left   = go->horizontalPosition  + go->bbox_left;
            unsigned int go_bbox_right  = go->horizontalPosition  + go->bbox_right;
            unsigned int go_bbox_top    = go->verticalPosition    + go->bbox_top;
            unsigned int go_bbox_bot    = go->verticalPosition    + go->bbox_bot;
            /* Are we colliding? */
			if ((go0_bbox_left  < go_bbox_right) &&
				(go0_bbox_right > go_bbox_left)  &&
				(go0_bbox_bot   > go_bbox_top)   &&
				(go0_bbox_top   < go_bbox_bot))
			{
				go->Receive(HIT);
				go0->Receive(HIT);
			}
		}
	}
}

void GroundComponent::Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects)
{
    Component::Create(engine, go, game_objects);
    this->coll_objects = coll_objects;
}


void GroundComponent::Update(float dt)
{
    for (auto i = 0; i < coll_objects->pool.size(); i++)
    {
        GameObject * go0 = coll_objects->pool[i];
        if (go0->enabled)
        {
            /* Other bounding box */
            unsigned int go0_bbox_left  = go0->horizontalPosition + go0->bbox_left;
            unsigned int go0_bbox_right = go0->horizontalPosition + go0->bbox_right;
            unsigned int go0_bbox_top   = go0->verticalPosition   + go0->bbox_top;
            unsigned int go0_bbox_bot   = go0->verticalPosition   + go0->bbox_bot;
            /* Our bounding box */
            unsigned int go_bbox_left   = go->horizontalPosition  + go->bbox_left;
            unsigned int go_bbox_right  = go->horizontalPosition  + go->bbox_right;
            unsigned int go_bbox_top    = go->verticalPosition    + go->bbox_top;
            unsigned int go_bbox_bot    = go->verticalPosition    + go->bbox_bot;
            /* Are we colliding? */
            if ((go0_bbox_left  < go_bbox_right) &&
                (go0_bbox_right > go_bbox_left)  &&
                (go0_bbox_bot   > go_bbox_top)   &&
                (go0_bbox_top   < go_bbox_bot))
            {
                // Tell the frog it's on ground
                go->Receive(ON_GROUND);
            }
        }
    }
}
