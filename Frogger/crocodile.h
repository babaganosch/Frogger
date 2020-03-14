//
//  crocodile.h
//  Frogger
//
//  Created by Lars Andersson on 2020-03-09.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef crocodile_h
#define crocodile_h

class Crocodile : public GameObject
{
public:

    int speed;
    int open_mouth;
    
    virtual ~Crocodile()    {        SDL_Log("Crocodile::~Crocodile");    }

    virtual void Init(int x, int y, int spd)
    {
        GameObject::Init();
        horizontalPosition = x;
        verticalPosition   = y;
        speed = spd;
        bbox_left = 2;
        bbox_right = width - 6;
        open_mouth = false;
    }
    
    void setMouth(bool mouth) {
        open_mouth = mouth;
    }

    virtual void Receive(Message m)
    {
    }
};

class CrocodileBehaviourComponent : public Component
{
    
    RenderComponent * render_component;

public:
    virtual ~CrocodileBehaviourComponent() {}

    virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects)
    {
        Component::Create(engine, go, game_objects);
    }

    virtual void Init() {
        render_component = go->GetComponent<RenderComponent*>();
    }

    virtual void Update(float dt) {
        go->horizontalPosition += ((Crocodile*)go)->speed * dt;
        if (go->horizontalPosition > SCREEN_WIDTH) {
            go->enabled = false;
        }
        /* Activate collision when on mouth open frame */
        ((Crocodile*)go)->setMouth(render_component->GetImageIndex() >= 1.f);
    }
};

class CrocodileHeadCollideComponent : public Component
{
    ObjectPool<GameObject> * coll_objects;
    Message msg;

public:
    virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects, Message messageType) {
        
        Component::Create(engine, go, game_objects);
        this->coll_objects = coll_objects;
        this->msg = messageType;
        
    }
    virtual void Update(float dt) {
        if (go->verticalPosition <= TOP_HALF) {
            for (auto i = 0; i < coll_objects->pool.size(); i++)
            {
                GameObject * go0 = coll_objects->pool[i];
                if (go0->enabled && ((Crocodile*)go0)->open_mouth)
                {
                    /* Other bounding box */
                    int go0_bbox_left  = go0->horizontalPosition + go0->bbox_left + 74;
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
    }
};

#endif /* crocodile_h */
