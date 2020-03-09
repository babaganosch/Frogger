//
//  bug.h
//  Frogger
//
//  Created by Lars Andersson on 2020-03-09.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef bug_h
#define bug_h

class Bug : public GameObject
{
public:
    
    virtual ~Bug()    {        SDL_Log("Bug::~Bug");    }

    virtual void Init(int x, int y)
    {
        GameObject::Init();
        horizontalPosition = x;
        verticalPosition   = y;
        bbox_left  = 8;
        bbox_right = 24;
        bbox_bot = height+1;
    }

    virtual void Receive(Message m) {}
};

class BugBehaviourComponent : public Component
{
    
public:
    virtual ~BugBehaviourComponent() {}

    virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects)
    {
        Component::Create(engine, go, game_objects);
    }

    virtual void Init() {
    }

    virtual void Update(float dt) {
    }
};

#endif /* bug_h */
