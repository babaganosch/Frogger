#pragma once
#include <iostream>

const float move_timer_start = 0.25f;
const float jump_distance    = 32.f;
const float move_speed       = 240.f;
const int ground_check_timer = 8;

// the main player
class Player : public GameObject
{
    bool  on_ground;
    float platform_speed;
    
public:

    int lives;    // it's game over when goes below zero
    int ground_timer;

    virtual ~Player()    {        SDL_Log("Player::~Player");    }

    virtual void Init()
    {
        SDL_Log("Player::Init");
        GameObject::Init();
        lives = NUM_LIVES;
        on_ground = true;
        ground_timer = ground_check_timer;
    }
    
    void Die() {
        if (verticalPosition <= TURTLE_ROW_0) {
            Send(PLAYER_DROWN);
        } else {
            Send(PLAYER_ROADKILL);
        }
        GameObject::Init();
        RemoveLife();
        if (lives < 0)
            Send(GAME_OVER);
    }
    
    bool GetOnGround() {
        return on_ground;
    }
    
    virtual void Update(float dt)
    {
        GameObject::Update(dt);
        if (verticalPosition < TURTLE_ROW_0+32) {
            ground_timer--;
            if (ground_timer <= 0) {
                on_ground = false;
            }
        } else {
            on_ground = true;
        }
    }

    virtual void Receive(Message m)
    {
        if (m == HIT)
        {
            SDL_Log("Player::Hit!");
            Die();
        }
        if (m == ON_GROUND)
        {
            //SDL_Log("Player::On Ground!");
            ground_timer = ground_check_timer;
            on_ground = true;
        }
    }

    void RemoveLife()
    {
        lives--;
        SDL_Log("remaining lives %d", lives);
    }
};

class PlayerBehaviourComponent : public Component
{
    double move_distance;
    double move_cooldown;
    int    top_y;
    
    DIRECTION moving;
    
    enum SPRITE_INDEX {
        IDLE_VERTICAL     = 0,
        MOVING_VERTICAL   = 1,
        IDLE_HORIZONTAL   = 2,
        MOVING_HORIZONTAL = 3
    };

public:
	virtual ~PlayerBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects)
	{
		Component::Create(engine, go, game_objects);
	}

	virtual void Init()
	{
        /* Place the frog at the starting position */
		go->horizontalPosition = SCREEN_WIDTH / 2;
		go->verticalPosition = SCREEN_HEIGHT - 64;
        top_y = go->verticalPosition;
        
        /* Initialize the frog */
        move_cooldown = move_timer_start;
        moving = DIRECTION::NONE;
        move_distance = 0.f;
        
        /* Setup frog bounding box */
        go->bbox_left  = 4;
        go->bbox_top   = 4;
        go->bbox_right = go->width  -4;
        go->bbox_bot   = go->height -4;
        
        /* Make sure the frog faces upwards */
        RenderComponent* rendererComponent = go->GetComponent<RenderComponent*>();
        rendererComponent->SetImageIndex(IDLE_VERTICAL);
        rendererComponent->SetImageFlip(SDL_FLIP_NONE);
	}

	virtual void Update(float dt)
	{
		AvancezLib::KeyStatus keys;
		engine->getKeyStatus(keys);
        move_cooldown -= dt;
        if (move_cooldown <= 0.f && dt > 0.f) {
            if (keys.right) {
                moving = DIRECTION::RIGHT;
                move_cooldown = move_timer_start / game_speed;
                move_distance = jump_distance;
            }
            if (keys.left) {
                moving = DIRECTION::LEFT;
                move_cooldown = move_timer_start / game_speed;
                move_distance = jump_distance;
            }
            if (keys.up) {
                moving = DIRECTION::UP;
                move_cooldown = move_timer_start / game_speed;
                move_distance = jump_distance;
            }
            if (keys.down) {
                moving = DIRECTION::DOWN;
                move_cooldown = move_timer_start / game_speed;
                move_distance = jump_distance;
            }
        }
        Move(dt);
        OnGround();
        move_cooldown = clamp(move_cooldown, 0.f, 100.f);
        
	}

	// move the player left or right
	// param move depends on the time, so the player moves always at the same speed on any computer
	void Move(float dt)
	{
        if (move_distance > 0.f) {
            /* AYY, MOVE! */
            RenderComponent* rendererComponent = go->GetComponent<RenderComponent*>();
            double dist = clamp(dt * move_speed, 0.f, move_distance);
            switch (moving) {
                case DIRECTION::RIGHT:
                    rendererComponent->SetImageIndex(MOVING_HORIZONTAL);
                    rendererComponent->SetImageFlip(SDL_FLIP_HORIZONTAL);
                    go->horizontalPosition += dist;
                    move_distance -= dist;
                    break;
                case DIRECTION::LEFT:
                    rendererComponent->SetImageIndex(MOVING_HORIZONTAL);
                    rendererComponent->SetImageFlip(SDL_FLIP_NONE);
                    go->horizontalPosition -= dist;
                    move_distance -= dist;
                    break;
                case DIRECTION::UP:
                    rendererComponent->SetImageIndex(MOVING_VERTICAL);
                    rendererComponent->SetImageFlip(SDL_FLIP_NONE);
                    go->verticalPosition  -= dist;
                    move_distance -= dist;
                    break;
                case DIRECTION::DOWN:
                    rendererComponent->SetImageIndex(MOVING_VERTICAL);
                    rendererComponent->SetImageFlip(SDL_FLIP_VERTICAL);
                    go->verticalPosition  += dist;
                    move_distance -= dist;
                    break;
                case DIRECTION::NONE: break;
            }
        } else {
            /* STOP MOVING */
            RenderComponent* rendererComponent = go->GetComponent<RenderComponent*>();
            switch (moving) {
                case DIRECTION::RIGHT:
                case DIRECTION::LEFT:
                    rendererComponent->SetImageIndex(IDLE_HORIZONTAL);
                    break;
                case DIRECTION::UP:
                case DIRECTION::DOWN:
                    rendererComponent->SetImageIndex(IDLE_VERTICAL);
                    break;
                case DIRECTION::NONE: break;
            }
            moving = DIRECTION::NONE;
            move_distance = 0.f;
            
            /* MOVE WITH THE PLATFORMS */
            int pos = go->verticalPosition;
            if (pos <= LOG_ROW_2) {
                /* On the medium log layer */
                go->horizontalPosition += FAST_PLATFORM_SPEED * dt;
            } else if (pos <= TURTLE_ROW_1) {
                /* On the top turtle layer */
                go->horizontalPosition -= SLOW_PLATFORM_SPEED * dt;
            } else if (pos <= LOG_ROW_1) {
                /* On the large log layer */
                go->horizontalPosition += FAST_PLATFORM_SPEED * dt;
            } else if (pos <= LOG_ROW_0) {
                /* On the small log layer */
                go->horizontalPosition += SLOW_PLATFORM_SPEED * dt;
            } else if (pos <= TURTLE_ROW_0) {
                /* On the bottom turtle layer */
                go->horizontalPosition -= FAST_PLATFORM_SPEED * dt;
            } else {
                /* Clamp the frog to a nice value */
                go->horizontalPosition = floor(go->horizontalPosition);
                go->verticalPosition = floor(go->verticalPosition);
            }
            
            if (go->verticalPosition < top_y) {
                top_y = go->verticalPosition;
                go->Send(VERTICAL_ADVANCEMENT);
            }
        }
        
        /* Keep the player within the world */
        go->horizontalPosition = clamp(go->horizontalPosition, 0.f, SCREEN_WIDTH-go->width);
        go->verticalPosition   = clamp(go->verticalPosition,   0.f, SCREEN_HEIGHT-go->height);
	}
    
    void OnGround() {
        if (moving == DIRECTION::NONE) {
            
            if (!((Player*)go)->GetOnGround()) {
                SDL_Log("On the river, ouch!");
                ((Player*)go)->Die();
            }
            
        }
    }
};



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
