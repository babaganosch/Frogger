//
//  log.h
//  Frogger
//
//  Created by Lars Andersson on 2020-02-27.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef log_h
#define log_h

class Log : public GameObject
{
public:

    int speed;
    
    virtual ~Log()    {        SDL_Log("Log::~Log");    }

    virtual void Init(int x, int y, int spd)
    {
        //SDL_Log("Log::Log");
        GameObject::Init();
        horizontalPosition = x;
        verticalPosition   = y;
        speed = spd;
        bbox_left = 2;
        bbox_right = width - 6;
    }

    virtual void Receive(Message m)
    {
    }
};

class LogBehaviourComponent : public Component
{

public:
    virtual ~LogBehaviourComponent() {}

    virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects)
    {
        Component::Create(engine, go, game_objects);
    }

    virtual void Init() {}

    virtual void Update(float dt) {
        go->horizontalPosition += ((Log*)go)->speed * dt;
        if (go->horizontalPosition > SCREEN_WIDTH) {
            go->enabled = false;
        }
    }
};

#endif /* log_h */
