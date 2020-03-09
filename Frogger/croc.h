//
//  croc.h
//  Frogger
//
//  Created by Lars Andersson on 2020-03-09.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef croc_h
#define croc_h

class Croc : public GameObject
{
public:
    
    bool bite;
    
    virtual ~Croc()    {        SDL_Log("Croc::~Croc");    }

    virtual void Init(int x, int y)
    {
        GameObject::Init();
        horizontalPosition = x;
        verticalPosition   = y;
        bite = false;
    }
    
    void SetBite() {
        bite = true;
    }

    virtual void Receive(Message m) {
        if (m == ANIMATION_END)
        {
            enabled = false;
        }
    }
};

class CrocBehaviourComponent : public Component
{
    RenderComponent * render_component;
public:
    virtual ~CrocBehaviourComponent() {}

    virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects)
    {
        Component::Create(engine, go, game_objects);
    }

    virtual void Init() {
        /* Reset animation */
        render_component = go->GetComponent<RenderComponent*>();
        render_component->SetImageIndex(0);
    }

    virtual void Update(float dt) {
        if (render_component->GetImageIndex() >= 1.f) ((Croc*)go)->SetBite();
    }
};

class CrocCollideComponent : public Component
{
    GameObject * coll_object; // collision will be tested with these objects
    Message msg;

public:
    virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, GameObject * coll_object, Message messageType) {
        
        Component::Create(engine, go, game_objects);
        this->coll_object = coll_object;
        this->msg = messageType;
        
    }
    virtual void Update(float dt) {
        GameObject * go0 = coll_object;
        if (go0->enabled && ((Croc*)go0)->bite)
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
};

#endif /* croc_h */
