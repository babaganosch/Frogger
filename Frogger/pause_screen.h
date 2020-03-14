//
//  pause_screen.h
//  Frogger
//
//  Created by Lars Andersson on 2020-03-10.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef pause_screen_h
#define pause_screen_h

class PauseScreen : public GameObject
{
public:

    bool showing;
    
    virtual ~PauseScreen()    {        SDL_Log("PauseScreen::~PauseScreen");    }

    virtual void Init()
    {
        GameObject::Init();
        showing = false;
    }

    virtual void Receive(Message m)
    {
        if (m == TOGGLE_PAUSE)
        {
            GameObject::Init();
            showing = !showing;
        }
    }
};

class PauseScreenBehaviourComponent : public Component
{

    int option;
    float bounce_timer;
    
public:
    virtual ~PauseScreenBehaviourComponent() {}

    virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects)
    {
        Component::Create(engine, go, game_objects);
    }

    virtual void Init() {
        option = 0;
        bounce_timer = 0.f;
    }

    virtual void Update(float dt) {
        if (((PauseScreen*)go)->showing) {
            bounce_timer = clamp(bounce_timer - .5f, 0.f, 30.f);
            AvancezLib::KeyStatus keys;
            engine->getKeyStatus(keys);
            if (keys.down && bounce_timer <= 0.f) {
                option++;
                option = option % 3;
                bounce_timer = 2.5f;
            }
            if (keys.up && bounce_timer <= 0.f) {
                option--;
                if (option < 0) option = 2;
                bounce_timer = 2.5f;
            }
            if (keys.fire && bounce_timer <= 0.f) {
                switch (option) {
                    case 0:
                        go->Send(BTN_RESUME);
                        break;
                    case 1:
                        go->Send(BTN_RETRY);
                        break;
                    case 2:
                        go->Send(BTN_EXIT);
                        break;
                }
                bounce_timer = 2.f;
            }
            
            int center_x = SCREEN_WIDTH  / 2;
            int center_y = SCREEN_HEIGHT / 2;
            int box_width  = SCREEN_WIDTH  / 3;
            int box_height = SCREEN_HEIGHT / 6;
            int top_y = SCREEN_HEIGHT / 2.5;
            int x = center_x - 38;
            
            /* Semi-black Background */
            engine->drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, c_transp, false);
            
            /* Paused Text */
            engine->drawText(32, 16, "PAUSED", H_ALIGN::LEFT, V_ALIGN::TOP, c_white);
            
            /* Blue Square */
            engine->drawRect(center_x-box_width, center_y-box_height, center_x+box_width, center_y+box_height, c_blue, false);
            
            if (option == 0) {
                engine->drawRect(x-16, top_y-2, (SCREEN_WIDTH/2)+82, top_y+16, c_white, false);
                engine->drawText(x-64, top_y, ">", H_ALIGN::LEFT, V_ALIGN::TOP, c_white);
                engine->drawText(x, top_y, "RESUME", H_ALIGN::LEFT, V_ALIGN::TOP, c_blue);
            } else {
                engine->drawText(x, top_y, "RESUME", H_ALIGN::LEFT, V_ALIGN::TOP, c_white);
            }
            top_y+=24;
            if (option == 1) {
                engine->drawRect(x-16, top_y-2, (SCREEN_WIDTH/2)+82, top_y+16, c_white, false);
                engine->drawText(x-64, top_y, ">", H_ALIGN::LEFT, V_ALIGN::TOP, c_white);
                engine->drawText(x, top_y, "REWIND", H_ALIGN::LEFT, V_ALIGN::TOP, c_blue);
            } else {
                engine->drawText(x, top_y, "REWIND",  H_ALIGN::LEFT, V_ALIGN::TOP, c_white);
            }
            top_y+=48;
            if (option == 2) {
                engine->drawRect(x-16, top_y-2, (SCREEN_WIDTH/2)+82, top_y+16, c_white, false);
                engine->drawText(x-64, top_y, ">", H_ALIGN::LEFT, V_ALIGN::TOP, c_white);
                engine->drawText(x, top_y, "EXIT", H_ALIGN::LEFT, V_ALIGN::TOP, c_blue);
            } else {
                engine->drawText(x, top_y, "EXIT",  H_ALIGN::LEFT, V_ALIGN::TOP, c_white);
            }
        }
    }
};

#endif /* pause_screen_h */
