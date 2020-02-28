//
//  pocket.h
//  Frogger
//
//  Created by Lars Andersson on 2020-02-28.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef pocket_h
#define pocket_h

class Pocket : public GameObject
{
public:
    
    virtual ~Pocket()    {        SDL_Log("Pocket::~Pocket");    }

    virtual void Init(int x, int y)
    {
        //SDL_Log("Car::Car");
        GameObject::Init();
        horizontalPosition = x;
        verticalPosition   = y;
    }

    virtual void Receive(Message m)
    {
    }
};

class PocketBehaviourComponent : public Component
{

public:
    virtual ~PocketBehaviourComponent() {}

    virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects)
    {
        Component::Create(engine, go, game_objects);
    }

    virtual void Init() {}

    virtual void Update(float dt) {
    }
};

#endif /* pocket_h */
