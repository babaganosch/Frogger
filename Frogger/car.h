//
//  car.h
//  Frogger
//
//  Created by Lars Andersson on 2020-02-28.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef car_h
#define car_h

#include <iostream>

class Car : public GameObject
{
public:

    int speed;
    
    virtual ~Car()    {        SDL_Log("Car::~Car");    }

    virtual void Init(int x, int y, int spd)
    {
        //SDL_Log("Car::Car");
        GameObject::Init();
        horizontalPosition = x;
        verticalPosition   = y;
        speed = spd;
    }

    virtual void Receive(Message m)
    {
    }
};

class CarBehaviourComponent : public Component
{

public:
    virtual ~CarBehaviourComponent() {}

    virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects)
    {
        Component::Create(engine, go, game_objects);
    }

    virtual void Init() {}

    virtual void Update(float dt) {
        float speed = ((Log*)go)->speed * dt;
        go->horizontalPosition += speed;
        if (speed >= 0.f) {
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

#endif /* car_h */
