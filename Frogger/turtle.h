//
//  turtle.h
//  Frogger
//
//  Created by Lars Andersson on 2020-02-25.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef turtle_h
#define turtle_h

class Turtle : public GameObject
{
public:

    int speed;

    virtual ~Turtle()    {        SDL_Log("Turtle::~Turtle");    }

    virtual void Init(int x, int y, int spd)
    {
        //SDL_Log("Turtle::Turtle");
        GameObject::Init();
        horizontalPosition = x;
        verticalPosition   = y;
        speed = spd;
    }

    virtual void Receive(Message m) {}
};

class TurtleBehaviourComponent : public Component
{

public:
    virtual ~TurtleBehaviourComponent() {}

    virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects)
    {
        Component::Create(engine, go, game_objects);
    }

    virtual void Init() {
        /* Reset animation */
        RenderComponent* rendererComponent = go->GetComponent<RenderComponent*>();
        rendererComponent->SetImageIndex(0);
        /* Setup turtle bounding box */
        go->bbox_left  = go->bbox_top = 4;
        go->bbox_right = go->bbox_bot = go->bbox_bot-4;
    }

    virtual void Update(float dt) {
        go->horizontalPosition += ((Turtle*)go)->speed * dt;
        if (go->horizontalPosition < -go->width) {
            go->enabled = false;
        }
    }
};



#endif /* turtle_h */
