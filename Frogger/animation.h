//
//  animation.h
//  Frogger
//
//  Created by Lars Andersson on 2020-02-29.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef animation_h
#define animation_h

class PlayerDeath : public GameObject
{
    
public:

    virtual ~PlayerDeath()    {        SDL_Log("PlayerDeath::~PlayerDeath");    }

    virtual void Init(int x, int y)
    {
        SDL_Log("PlayerDeath::Init");
        GameObject::Init();
        horizontalPosition = x;
        verticalPosition = y;
        /* Restart the animation */
        RenderComponent* rendererComponent = GetComponent<RenderComponent*>();
        rendererComponent->SetImageIndex(0);
        bbox_right = 0; bbox_top = 0; bbox_left = 0; bbox_bot = 0;
    }
    
    virtual void Update(float dt)
    {
        GameObject::Update(dt);
    }

    virtual void Receive(Message m)
    {
        if (m == ANIMATION_END) {
            enabled = false;
        }
    }
};

class ScoreAnimation : public GameObject
{
    
public:

    virtual ~ScoreAnimation()    {        SDL_Log("ScoreAnimation::~ScoreAnimation");    }

    virtual void Init(int x, int y)
    {
        SDL_Log("ScoreAnimation::Init");
        GameObject::Init();
        horizontalPosition = x;
        verticalPosition = y-CELL_S;
        /* Restart the animation */
        RenderComponent* rendererComponent = GetComponent<RenderComponent*>();
        rendererComponent->SetImageIndex(0);
        bbox_right = 0; bbox_top = 0; bbox_left = 0; bbox_bot = 0;
    }
    
    virtual void Update(float dt)
    {
        GameObject::Update(dt);
    }

    virtual void Receive(Message m)
    {
        if (m == ANIMATION_END) {
            enabled = false;
        }
    }
};



#endif /* animation_h */
