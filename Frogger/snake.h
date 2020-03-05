//
//  snake.h
//  Frogger
//
//  Created by Lars Andersson on 2020-03-05.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef snake_h
#define snake_h

class Snake : public GameObject
{
public:

    int speed;
    
    virtual ~Snake()    {        SDL_Log("Snake::~Snake");    }

    virtual void Init(int x, int y, int spd, SDL_RendererFlip flip)
    {
        //SDL_Log("Snake::Snake");
        GameObject::Init();
        horizontalPosition = x;
        verticalPosition   = y;
        speed = spd;
        image_flip = flip;
    }

    virtual void Receive(Message m)
    {
    }
};

class SnakeBehaviourComponent : public Component
{

public:
    virtual ~SnakeBehaviourComponent() {}

    virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects)
    {
        Component::Create(engine, go, game_objects);
    }

    virtual void Init() {}

    virtual void Update(float dt) {
        float speed = ((Log*)go)->speed * dt;
        go->horizontalPosition += speed;
        if (go->image_flip == SDL_FLIP_HORIZONTAL) {
            if (go->horizontalPosition > SCREEN_WIDTH) {
                go->enabled = false;
            }
        } else {
            if (go->horizontalPosition < -go->width) {
                go->enabled = false;
            }
        }
    }
};

#endif /* snake_h */
