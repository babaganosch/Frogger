//
//  grudge.h
//  Frogger
//
//  Created by Lars Andersson on 2020-03-06.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef grudge_h
#define grudge_h

class Grudge : public GameObject
{
public:

    int target_x, target_y;
    
    virtual ~Grudge()    {        SDL_Log("Grudge::~Grudge");    }

    virtual void Init(int x, int y)
    {
        GameObject::Init();
        horizontalPosition = x;
        verticalPosition   = y;
        bbox_left  = 4;
        bbox_top   = 8;
        target_x = 0;
        target_y = 0;
    }
    
    void SetTarget(int x, int y) {
        target_x = x - (CELL_S/2);
        target_y = y - (CELL_S/2);
    }

    virtual void Receive(Message m) {}
};

class GrudgeBehaviourComponent : public Component
{

    float speed;
    
public:
    virtual ~GrudgeBehaviourComponent() {}

    virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects)
    {
        Component::Create(engine, go, game_objects);
    }

    virtual void Init() {
        /* Reset animation */
        speed = 30;
    }

    virtual void Update(float dt) {
        if (go->horizontalPosition < ((Grudge*)go)->target_x) {
            go->horizontalPosition += dt * speed;
        } else {
            go->horizontalPosition -= dt * speed;
        }
        if (go->verticalPosition < ((Grudge*)go)->target_y) {
            go->verticalPosition += dt * speed;
        } else {
            go->verticalPosition -= dt * speed;
        }
    }
};

#endif /* grudge_h */
