#pragma once

/* Draw colors */
const SDL_Color c_white  = { 255, 255, 255 };
const SDL_Color c_red    = { 255, 0,   0   };
const SDL_Color c_yellow = { 255, 255, 0   };
const SDL_Color c_green  = { 33,  222, 0   };
const SDL_Color c_river  = { 0,   0,   71  };

class Game : public GameObject
{
	std::set<GameObject*> game_objects;	// http://www.cplusplus.com/reference/set/set/
	
	AvancezLib* engine;
	Player * player;
    PlayerDeath * player_drown;
    PlayerDeath * player_roadkill;
	Sprite * life_sprite;
    Sprite * grass_purple;
    Sprite * grass_top;
    
    /* Goal */
    bool pocket0, pocket1, pocket2, pocket3, pocket4;
    bool victory;
    float wink_timer;
    Sprite * victory_frog;
    Sprite * victory_frog_wink;
    ObjectPool<Pocket> goal_pool;
    
    /* Platform pools */
    ObjectPool<Turtle> turtle_pool;
    ObjectPool<Log>    log_pool_small;
    ObjectPool<Log>    log_pool_medium;
    ObjectPool<Log>    log_pool_large;
    
    /* Vehicle pools */
    ObjectPool<Car>    car_pool_0;
    ObjectPool<Car>    car_pool_1;
    ObjectPool<Car>    car_pool_2;
    ObjectPool<Car>    car_pool_3;
    ObjectPool<Car>    car_pool_4;

    /* Global */
	unsigned int score;
    unsigned int hiScore;
    bool         game_over;
    
    /* Timers */
    float        game_timer;
    float        log_timer_top;
    float        log_timer_mid;
    float        log_timer_bot;
    float        turtle_timer_bot;
    float        turtle_timer_top;
    
    float        car0_timer;
    float        car1_timer;
    float        car2_timer;
    float        car3_timer;
    float        car4_timer;

public:

	virtual void Create(AvancezLib* engine)
	{
		SDL_Log("Game::Create");

		this->engine = engine;

        // Player
		player = new Player();
		PlayerBehaviourComponent * player_behaviour = new PlayerBehaviourComponent();
		player_behaviour->Create(engine, player, &game_objects);
        
        /* Platform colliders */
        CollideComponent * large_log_collider = new CollideComponent();
        large_log_collider->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) &log_pool_large, ON_GROUND);
        CollideComponent * medium_log_collider = new CollideComponent();
        medium_log_collider->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) &log_pool_medium, ON_GROUND);
        CollideComponent * small_log_collider = new CollideComponent();
        small_log_collider->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) &log_pool_small, ON_GROUND);
        CollideComponent * turtle_collider = new CollideComponent();
        turtle_collider->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) &turtle_pool, ON_GROUND);
        
        /* Vehicle colliders */
        CollideComponent * car0_collider = new CollideComponent();
        car0_collider->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) &car_pool_0, HIT);
        CollideComponent * car1_collider = new CollideComponent();
        car1_collider->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) &car_pool_1, HIT);
        CollideComponent * car2_collider = new CollideComponent();
        car2_collider->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) &car_pool_2, HIT);
        CollideComponent * car3_collider = new CollideComponent();
        car3_collider->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) &car_pool_3, HIT);
        CollideComponent * car4_collider = new CollideComponent();
        car4_collider->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) &car_pool_4, HIT);
        
        /* Goal colliders */
        CollideComponent * pocket_collider = new CollideComponent();
        pocket_collider->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) &goal_pool, POCKET_REACHED);
        
        /* Renderer */
		RenderComponent * player_render = new RenderComponent();
        player_render->Create(engine, player, &game_objects, "/Users/larsa/Chalmers/TDA572/Data/frog/frog0.bmp", 0.f);
        player_render->AddSprite("/Users/larsa/Chalmers/TDA572/Data/frog/frog1.bmp");
        player_render->AddSprite("/Users/larsa/Chalmers/TDA572/Data/frog/frog2.bmp");
        player_render->AddSprite("/Users/larsa/Chalmers/TDA572/Data/frog/frog3.bmp");
        
        player->Create();
        player->AddComponent(player_behaviour);
        player->AddComponent(player_render);
        player->AddReceiver(this);
        game_objects.insert(player);
        /* Player collision components */
        player->AddComponent(large_log_collider);
        player->AddComponent(medium_log_collider);
        player->AddComponent(small_log_collider);
        player->AddComponent(turtle_collider);
        player->AddComponent(car0_collider);
        player->AddComponent(car1_collider);
        player->AddComponent(car2_collider);
        player->AddComponent(car3_collider);
        player->AddComponent(car4_collider);
        player->AddComponent(pocket_collider);
        
        /* Player death animation */
        player_drown = new PlayerDeath();
        RenderComponent * player_drown_render = new RenderComponent();
        player_drown_render->Create(engine, player_drown, &game_objects, "/Users/larsa/Chalmers/TDA572/Data/death/frog_drown0.bmp", 4.f);
        player_drown_render->AddSprite("/Users/larsa/Chalmers/TDA572/Data/death/frog_drown1.bmp");
        player_drown_render->AddSprite("/Users/larsa/Chalmers/TDA572/Data/death/frog_drown2.bmp");
        player_drown_render->AddSprite("/Users/larsa/Chalmers/TDA572/Data/death/skull.bmp");
        player_drown_render->AddSprite("/Users/larsa/Chalmers/TDA572/Data/death/skull.bmp");
        player_drown_render->AddSprite("/Users/larsa/Chalmers/TDA572/Data/death/skull.bmp");
        player_drown->AddComponent(player_drown_render);
        
        player_roadkill = new PlayerDeath();
        RenderComponent * player_roadkill_render = new RenderComponent();
        player_roadkill_render->Create(engine, player_roadkill, &game_objects, "/Users/larsa/Chalmers/TDA572/Data/death/frog_roadkill0.bmp", 4.f);
        player_roadkill_render->AddSprite("/Users/larsa/Chalmers/TDA572/Data/death/frog_roadkill1.bmp");
        player_roadkill_render->AddSprite("/Users/larsa/Chalmers/TDA572/Data/death/frog_roadkill2.bmp");
        player_roadkill_render->AddSprite("/Users/larsa/Chalmers/TDA572/Data/death/skull.bmp");
        player_roadkill_render->AddSprite("/Users/larsa/Chalmers/TDA572/Data/death/skull.bmp");
        player_roadkill_render->AddSprite("/Users/larsa/Chalmers/TDA572/Data/death/skull.bmp");
        player_roadkill->AddComponent(player_roadkill_render);
        
        goal_pool.Create(5);
        for (auto pocket = goal_pool.pool.begin(); pocket != goal_pool.pool.end(); pocket++)
        {
            PocketBehaviourComponent * pocket_behaviour = new PocketBehaviourComponent();
            pocket_behaviour->Create(engine, *pocket, &game_objects);
            (*pocket)->Create();
            (*pocket)->AddComponent(pocket_behaviour);
            (*pocket)->AddReceiver(this);
            
            game_objects.insert(*pocket);
        }
        
        turtle_pool.Create(30);
        for (auto turtle = turtle_pool.pool.begin(); turtle != turtle_pool.pool.end(); turtle++)
        {
            TurtleBehaviourComponent * turtle_behaviour = new TurtleBehaviourComponent();
            turtle_behaviour->Create(engine, *turtle, &game_objects);
            RenderComponent * turtle_render = new RenderComponent();
            turtle_render->Create(engine, *turtle, &game_objects, "/Users/larsa/Chalmers/TDA572/Data/turtle/turtle2.bmp", 4.f);
            turtle_render->AddSprite("/Users/larsa/Chalmers/TDA572/Data/turtle/turtle1.bmp");
            turtle_render->AddSprite("/Users/larsa/Chalmers/TDA572/Data/turtle/turtle0.bmp");
            (*turtle)->Create();
            (*turtle)->AddComponent(turtle_behaviour);
            (*turtle)->AddComponent(turtle_render);
            (*turtle)->AddReceiver(this);
            
            game_objects.insert(*turtle);
        }
        
        log_pool_small.Create(6);
        for (auto log = log_pool_small.pool.begin(); log != log_pool_small.pool.end(); log++)
        {
            LogBehaviourComponent * log_behaviour = new LogBehaviourComponent();
            log_behaviour->Create(engine, *log, &game_objects);
            RenderComponent * log_renderer = new RenderComponent();
            log_renderer->Create(engine, *log, &game_objects, "/Users/larsa/Chalmers/TDA572/Data/logs/log_s.bmp", 0.f);
            (*log)->Create();
            (*log)->AddComponent(log_behaviour);
            (*log)->AddComponent(log_renderer);
            (*log)->AddReceiver(this);
            
            game_objects.insert(*log);
        }
        
        log_pool_medium.Create(4);
        for (auto log = log_pool_medium.pool.begin(); log != log_pool_medium.pool.end(); log++)
        {
            LogBehaviourComponent * log_behaviour = new LogBehaviourComponent();
            log_behaviour->Create(engine, *log, &game_objects);
            RenderComponent * log_renderer = new RenderComponent();
            log_renderer->Create(engine, *log, &game_objects, "/Users/larsa/Chalmers/TDA572/Data/logs/log_m.bmp", 0.f);
            (*log)->Create();
            (*log)->AddComponent(log_behaviour);
            (*log)->AddComponent(log_renderer);
            (*log)->AddReceiver(this);
            
            game_objects.insert(*log);
        }
        
        log_pool_large.Create(3);
        for (auto log = log_pool_large.pool.begin(); log != log_pool_large.pool.end(); log++)
        {
            LogBehaviourComponent * log_behaviour = new LogBehaviourComponent();
            log_behaviour->Create(engine, *log, &game_objects);
            RenderComponent * log_renderer = new RenderComponent();
            log_renderer->Create(engine, *log, &game_objects, "/Users/larsa/Chalmers/TDA572/Data/logs/log_l.bmp", 0.f);
            (*log)->Create();
            (*log)->AddComponent(log_behaviour);
            (*log)->AddComponent(log_renderer);
            (*log)->AddReceiver(this);
            
            game_objects.insert(*log);
        }
        
        
        car_pool_4.Create(6);
        for (auto car = car_pool_4.pool.begin(); car != car_pool_4.pool.end(); car++)
        {
            CarBehaviourComponent * car_behaviour = new CarBehaviourComponent();
            car_behaviour->Create(engine, *car, &game_objects);
            RenderComponent * car_renderer = new RenderComponent();
            car_renderer->Create(engine, *car, &game_objects, "/Users/larsa/Chalmers/TDA572/Data/cars/car4.bmp", 0.f);
            (*car)->Create();
            (*car)->AddComponent(car_behaviour);
            (*car)->AddComponent(car_renderer);
            (*car)->AddReceiver(this);
            
            game_objects.insert(*car);
        }
        
        car_pool_3.Create(4);
        for (auto car = car_pool_3.pool.begin(); car != car_pool_3.pool.end(); car++)
        {
            CarBehaviourComponent * car_behaviour = new CarBehaviourComponent();
            car_behaviour->Create(engine, *car, &game_objects);
            RenderComponent * car_renderer = new RenderComponent();
            car_renderer->Create(engine, *car, &game_objects, "/Users/larsa/Chalmers/TDA572/Data/cars/car3.bmp", 0.f);
            (*car)->Create();
            (*car)->AddComponent(car_behaviour);
            (*car)->AddComponent(car_renderer);
            (*car)->AddReceiver(this);
            
            game_objects.insert(*car);
        }
        
        car_pool_2.Create(6);
        for (auto car = car_pool_2.pool.begin(); car != car_pool_2.pool.end(); car++)
        {
            CarBehaviourComponent * car_behaviour = new CarBehaviourComponent();
            car_behaviour->Create(engine, *car, &game_objects);
            RenderComponent * car_renderer = new RenderComponent();
            car_renderer->Create(engine, *car, &game_objects, "/Users/larsa/Chalmers/TDA572/Data/cars/car2.bmp", 0.f);
            (*car)->Create();
            (*car)->AddComponent(car_behaviour);
            (*car)->AddComponent(car_renderer);
            (*car)->AddReceiver(this);
            
            game_objects.insert(*car);
        }
        
        car_pool_1.Create(6);
        for (auto car = car_pool_1.pool.begin(); car != car_pool_1.pool.end(); car++)
        {
            CarBehaviourComponent * car_behaviour = new CarBehaviourComponent();
            car_behaviour->Create(engine, *car, &game_objects);
            RenderComponent * car_renderer = new RenderComponent();
            car_renderer->Create(engine, *car, &game_objects, "/Users/larsa/Chalmers/TDA572/Data/cars/car1.bmp", 0.f);
            (*car)->Create();
            (*car)->AddComponent(car_behaviour);
            (*car)->AddComponent(car_renderer);
            (*car)->AddReceiver(this);
            
            game_objects.insert(*car);
        }
        
        car_pool_0.Create(6);
        for (auto car = car_pool_0.pool.begin(); car != car_pool_0.pool.end(); car++)
        {
            CarBehaviourComponent * car_behaviour = new CarBehaviourComponent();
            car_behaviour->Create(engine, *car, &game_objects);
            RenderComponent * car_renderer = new RenderComponent();
            car_renderer->Create(engine, *car, &game_objects, "/Users/larsa/Chalmers/TDA572/Data/cars/car0.bmp", 0.f);
            (*car)->Create();
            (*car)->AddComponent(car_behaviour);
            (*car)->AddComponent(car_renderer);
            (*car)->AddReceiver(this);
            
            game_objects.insert(*car);
        }
        
		life_sprite  = engine->createSprite("/Users/larsa/Chalmers/TDA572/Data/misc/frog_life.bmp");
        grass_purple = engine->createSprite("/Users/larsa/Chalmers/TDA572/Data/bg/grass_purple.bmp");
        grass_top    = engine->createSprite("/Users/larsa/Chalmers/TDA572/Data/bg/grass_top.bmp");
        victory_frog = engine->createSprite("/Users/larsa/Chalmers/TDA572/Data/misc/victory_frog0.bmp");
        victory_frog_wink = engine->createSprite("/Users/larsa/Chalmers/TDA572/Data/misc/victory_frog1.bmp");
	}

	virtual void Init()
	{
		player->Init();

        /* Globals */
		enabled     = true;
        game_over   = false;
        score       = 0;
        hiScore     = 0;
        game_timer  = GAME_TIMER;
        
        /* Timers */
        log_timer_top    = 1.5f;
        turtle_timer_top = 0.f;
        log_timer_mid    = 0.f;
        log_timer_bot    = 0.f;
        turtle_timer_bot = 0.f;
        car4_timer = 0.f;
        car3_timer = 0.f;
        car2_timer = 0.f;
        car1_timer = 0.f;
        car0_timer = 0.f;
        
        /* Pockets */
        pocket0 = false;
        pocket1 = false;
        pocket2 = false;
        pocket3 = false;
        pocket4 = false;
        victory = false;
        wink_timer = 0.f;
        /* Init 5 pockets */
        for (int x = 16; x < SCREEN_WIDTH; x += 96) {
            goal_pool.FirstAvailable()->Init(x, 96);
        }
	}
    
    void ResetPockets() {
        pocket0 = pocket1 = pocket2 = pocket3 = pocket4 = 0;
        for (auto pocket = goal_pool.pool.begin(); pocket != goal_pool.pool.end(); pocket++)
        {
            (*pocket)->enabled = false;
        }
        /* Init 5 pockets */
        for (int x = 16; x < SCREEN_WIDTH; x += 96) {
            goal_pool.FirstAvailable()->Init(x, 96);
        }
    }
    
    void NextLevel() {
        victory     = false;
        wink_timer  = 0.f;
        game_timer  = GAME_TIMER;
        game_speed *= 1.1;
        pocket0 = pocket1 = pocket2 = pocket3 = pocket4 = 0;
        
        ResetPockets();
    }
    
    void Restart() {
        player->Init();
        game_over   = false;
        victory     = false;
        score       = 0;
        game_timer  = GAME_TIMER;
        game_speed  = 1.f;
        
        ResetPockets();
    }

	virtual void Update(float dt)
	{
		AvancezLib::KeyStatus keys;
		engine->getKeyStatus(keys);
		if (keys.esc) {
			Destroy();
			engine->quit();
		}
        
        if (keys.restart) {
            Restart();
        }
        
        /* Draw the BG */
        {
            engine->drawRect(0, 0, SCREEN_WIDTH, GRASS_GREEN_ROW_TOP+8, c_river, false);
            int width0 = SCREEN_WIDTH/96;
            int width1 = SCREEN_WIDTH/CELL_S;
            /* Top Grass */
            for (int x = 0; x <= width0; x++) {
                grass_top->draw(x*96, CELL_S + 48);
            }
            /* Purple Grass */
            for (int x = 0; x < width1; x++) {
                grass_purple->draw(x*CELL_S, GRASS_GREEN_ROW_TOP);
                grass_purple->draw(x*CELL_S, GRASS_GREEN_ROW_BOT);
            }
            /* Victory Frogs */
            if (pocket0) {
                if (wink_timer > .5f) victory_frog_wink->draw(16, 96);
                else victory_frog->draw(16, 96);
            }
            if (pocket1) {
                if (wink_timer > 1.f) victory_frog_wink->draw(112, 96);
                else victory_frog->draw(112, 96);
            }
            if (pocket2) {
                if (wink_timer > 1.5f) victory_frog_wink->draw(208, 96);
                else victory_frog->draw(208, 96);
            }
            if (pocket3) {
                if (wink_timer > 2.f) victory_frog_wink->draw(304, 96);
                else victory_frog->draw(304, 96);
            }
            if (pocket4) {
                if (wink_timer > 2.5f) victory_frog_wink->draw(400, 96);
                else victory_frog->draw(400, 96);
            }
        }

        /* Freeze the objects when game over OR Victory */
        float running = game_over ? 0.0f : 1.0f;
        if (victory) running = 0.f;
        for (auto go = game_objects.begin(); go != game_objects.end(); go++)
            (*go)->Update(dt * running);
        
        /* Make sure player is rendered last */
        player->Update(0);
        player_drown->Update(dt);
        player_roadkill->Update(dt);
        
        if (victory) {
            wink_timer += (dt/game_speed);
            if (wink_timer > 4.f) NextLevel();
        }
        
        if (score > hiScore) hiScore = score;
        
        /* Spawn new entities */
        if (running > 0.f) {
            if (game_timer <= 0.f) {
                game_over = true;
            }
            game_timer = clamp(game_timer - dt * running, 0.f, 100.f);
            
            /* Platforms */
            log_timer_top    -= dt;
            turtle_timer_top -= dt;
            log_timer_mid    -= dt;
            log_timer_bot    -= dt;
            turtle_timer_bot -= dt;
            
            if (log_timer_top <= 0.f) {
                /* SPAWN MEDIUM LOG */
                log_timer_top = 3.f;
                log_pool_medium.FirstAvailable()->Init(-114, LOG_ROW_2+6, FAST_PLATFORM_SPEED);
            }
            if (turtle_timer_top <= 0.f) {
                /* SPAWN TURTLES */
                turtle_timer_top = 2.f;
                turtle_pool.FirstAvailable()->Init(SCREEN_WIDTH,    TURTLE_ROW_1, -SLOW_PLATFORM_SPEED);
                turtle_pool.FirstAvailable()->Init(SCREEN_WIDTH+32, TURTLE_ROW_1, -SLOW_PLATFORM_SPEED);
            }
            if (log_timer_mid <= 0.f) {
                /* SPAWN LARGE LOG */
                log_timer_mid = 3.f;
                log_pool_large.FirstAvailable()->Init(-174, LOG_ROW_1+6, FAST_PLATFORM_SPEED);
            }
            if (log_timer_bot <= 0.f) {
                /* SPAWN SMALL LOG */
                log_timer_bot = 3.f;
                log_pool_small.FirstAvailable()->Init(-84, LOG_ROW_0+6, SLOW_PLATFORM_SPEED);
            }
            if (turtle_timer_bot <= 0.f) {
                /* SPAWN TURTLES */
                turtle_timer_bot = 3.f;
                turtle_pool.FirstAvailable()->Init(SCREEN_WIDTH,    TURTLE_ROW_0, -FAST_PLATFORM_SPEED);
                turtle_pool.FirstAvailable()->Init(SCREEN_WIDTH+32, TURTLE_ROW_0, -FAST_PLATFORM_SPEED);
                turtle_pool.FirstAvailable()->Init(SCREEN_WIDTH+64, TURTLE_ROW_0, -FAST_PLATFORM_SPEED);
            }
            
            
            /* Vehicles */
            car4_timer -= dt;
            car3_timer -= dt;
            car2_timer -= dt;
            car1_timer -= dt;
            car0_timer -= dt;
            
            if (car4_timer <= 0.f) {
                car4_timer = 2.5f;
                car_pool_4.FirstAvailable()->Init(SCREEN_WIDTH, CAR_LANE_4+6, -SLOW_CAR_SPEED);
            }
            if (car3_timer <= 0.f) {
                car3_timer = 2.5f;
                car_pool_3.FirstAvailable()->Init(-32, CAR_LANE_3+2, FAST_CAR_SPEED);
            }
            if (car2_timer <= 0.f) {
                car2_timer = 2.f;
                car_pool_2.FirstAvailable()->Init(SCREEN_WIDTH, CAR_LANE_2+6, -MEDIUM_CAR_SPEED);
            }
            if (car1_timer <= 0.f) {
                car1_timer = 2.f;
                car_pool_1.FirstAvailable()->Init(-32, CAR_LANE_1+4, MEDIUM_CAR_SPEED);
            }
            if (car0_timer <= 0.f) {
                car0_timer = 2.5f;
                car_pool_0.FirstAvailable()->Init(SCREEN_WIDTH, CAR_LANE_0+2, -MEDIUM_CAR_SPEED);
            }
            
        }
	}
    
	virtual void Draw()
	{
        char text[256];
        
        /* Score */
        snprintf(text, 256, "HI-SCORE");
        engine->drawText(SCREEN_WIDTH / 2, 0, text, H_ALIGN::CENTER, V_ALIGN::TOP, c_white);
        snprintf(text, 256, "%05d", hiScore);
        engine->drawText(SCREEN_WIDTH / 2, 16, text, H_ALIGN::CENTER, V_ALIGN::TOP, c_red);
        snprintf(text, 256, "1-UP");
        engine->drawText(SCREEN_WIDTH / 5, 0, text, H_ALIGN::CENTER, V_ALIGN::TOP, c_white);
        snprintf(text, 256, "%05d", score);
        engine->drawText(SCREEN_WIDTH / 5, 16, text, H_ALIGN::CENTER, V_ALIGN::TOP, c_red);
        
        /* Time */
        {
            int x0 = SCREEN_WIDTH  - 68 - (4*game_timer);
            int y0 = SCREEN_HEIGHT - 16;
            int x1 = SCREEN_WIDTH  - 68;
            int y1 = SCREEN_HEIGHT;
            snprintf(text, 256, "TIME");
            engine->drawText(SCREEN_WIDTH - 2, SCREEN_HEIGHT - 16, text, H_ALIGN::RIGHT, V_ALIGN::TOP, c_yellow);
            engine->drawRect(x0, y0, x1, y1, c_green, false);
        }
        
        /* Lives */
        {
            int spr_w = life_sprite->getImageWidth() + 2;
            int spr_h = life_sprite->getImageHeight();
            for (int i = 0; i < player->lives; i++) {
                life_sprite->draw(2 + (spr_w * i), SCREEN_HEIGHT - (spr_h*2));
            }
        }
        
        /* Game Over */
        if (game_over) {
            snprintf(text, 256, "*** G A M E   O V E R ***");
            engine->drawText(SCREEN_WIDTH / 2, 48, text, H_ALIGN::CENTER, V_ALIGN::TOP, c_white);
        }

		engine->swapBuffers();
		engine->clearWindow();
	}

	virtual void Receive(Message m)
	{
		if (m == GAME_OVER)
		{
            SDL_Log("Game Over!");
            game_over = true;
		}
        
        else if (m == PLAYER_DROWN)
        {
            SDL_Log("Player Drowned");
            player_drown->Init(player->horizontalPosition, player->verticalPosition);
        }
        
        else if (m == PLAYER_ROADKILL)
        {
            SDL_Log("Player died on land");
            player_roadkill->Init(player->horizontalPosition, player->verticalPosition);
        }
        
        else if (m == VERTICAL_ADVANCEMENT)
        {
            score += 10;
        }
        
        else if (m == POCKET_REACHED)
        {
            game_timer = GAME_TIMER;
            score += 100;
            int x = player->horizontalPosition;
            if (x >= 368) {
                pocket4 = true;
            } else if (x >= 272) {
                pocket3 = true;
            } else if (x >= 176) {
                pocket2 = true;
            } else if (x >= 80) {
                pocket1 = true;
            } else {
                pocket0 = true;
            }
            
            if (pocket0 && pocket1 && pocket2 && pocket3 && pocket4) {
                /* Victory! */
                victory = true;
            }
        }
	}

	virtual void Destroy()
	{
		SDL_Log("Game::Destroy");

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Destroy();

		life_sprite->destroy();
        grass_purple->destroy();
        grass_top->destroy();
        victory_frog->destroy();
        victory_frog_wink->destroy();
        
		delete player;
        delete player_drown;
        delete player_roadkill;
	}
};
