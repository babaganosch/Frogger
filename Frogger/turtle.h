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
    bool afloat;

    virtual ~Turtle()    {        SDL_Log("Turtle::~Turtle");    }

    virtual void Init(int x, int y, int spd)
    {
        //SDL_Log("Turtle::Turtle");
        GameObject::Init();
        horizontalPosition = x;
        verticalPosition   = y;
        speed = spd;
        afloat = true;
    }

    virtual void Receive(Message m) {}
};

class TurtleBehaviourComponent : public Component
{
    RenderComponent * render_component;
public:
    virtual ~TurtleBehaviourComponent() {}

    virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects)
    {
        Component::Create(engine, go, game_objects);
    }

    virtual void Init() {
        /* Reset animation */
        render_component = go->GetComponent<RenderComponent*>();
        render_component->SetImageIndex(0);
        /* Setup turtle bounding box */
        go->bbox_left  = 4;
        go->bbox_top   = 4;
        go->bbox_right = go->width  -4;
        go->bbox_bot   = go->height -4;
    }

    virtual void Update(float dt) {
        go->horizontalPosition += ((Turtle*)go)->speed * dt;
        if (go->horizontalPosition < -go->width) {
            go->enabled = false;
        }
        if (render_component->GetImageIndex() >= 4.f) {
            ((Turtle*)go)->afloat = false;
        } else {
            ((Turtle*)go)->afloat = true;
        }
    }
};

class TurtleCollideComponent : public Component
{
    ObjectPool<GameObject> * coll_objects; // collision will be tested with these objects
    Message msg;

public:
    virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects, Message messageType) {
        
        Component::Create(engine, go, game_objects);
        this->coll_objects = coll_objects;
        this->msg = messageType;
        
    }
    virtual void Update(float dt) {
        for (auto i = 0; i < coll_objects->pool.size(); i++)
        {
            GameObject * go0 = coll_objects->pool[i];
            if (go0->enabled && ((Turtle*)go0)->afloat)
            {
                /* Other bounding box */
                int go0_bbox_left  = go0->horizontalPosition + go0->bbox_left;
                int go0_bbox_right = go0->horizontalPosition + go0->bbox_right;
                int go0_bbox_top   = go0->verticalPosition   + go0->bbox_top;
                int go0_bbox_bot   = go0->verticalPosition   + go0->bbox_bot;
                /* Our bounding box */
                int go_bbox_left   = go->horizontalPosition  + go->bbox_left;
                int go_bbox_right  = go->horizontalPosition  + go->bbox_right;
                int go_bbox_top    = go->verticalPosition    + go->bbox_top;
                int go_bbox_bot    = go->verticalPosition    + go->bbox_bot;
                /* Are we colliding? */
                if ((go0_bbox_left  < go_bbox_right) &&
                    (go0_bbox_right > go_bbox_left)  &&
                    (go0_bbox_bot   > go_bbox_top)   &&
                    (go0_bbox_top   < go_bbox_bot))
                {
                    go->Receive(msg);
                    go0->Receive(msg);
                }
            }
        }
    }
};

#endif /* turtle_h */
