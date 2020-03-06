#pragma once

/* Draw colors */
const SDL_Color c_white  = { 255, 255, 255, 255 };
const SDL_Color c_black  = { 0,   0,   0,   255 };
const SDL_Color c_transp = { 0,   0,   0,   128 };
const SDL_Color c_red    = { 255, 0,   0,   255 };
const SDL_Color c_yellow = { 255, 255, 0,   255 };
const SDL_Color c_green  = { 33,  222, 0,   255 };
const SDL_Color c_river  = { 0,   0,   71,  255 };

class Game : public GameObject
{
	std::set<GameObject*> game_objects;	// http://www.cplusplus.com/reference/set/set/
	
	AvancezLib* engine;
	Player * player;
    Grudge * grudge;
    PlayerDeath * player_drown;
    PlayerDeath * player_roadkill;
	Sprite * life_sprite;
    Sprite * grass_purple;
    Sprite * grass_top;
    Sprite * level_sprite;
    
    /* Goal */
    bool pocket0, pocket1, pocket2, pocket3, pocket4;
    bool victory;
    int  victory_time;
    int  extra_frog_counter;
    float wink_timer;
    Sprite * victory_frog;
    Sprite * victory_frog_wink;
    ObjectPool<Pocket> goal_pool;
    
    /* Score pools */
    ObjectPool<ScoreAnimation>    score100_pool;
    ObjectPool<ScoreAnimation>    score200_pool;
    
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
    
    /* Enemy pools */
    ObjectPool<Snake>  snake_pool;
    
    /* Sounds */
    Mix_Music * m_music_menu;
    Mix_Music * m_music_0;
    Mix_Music * m_music_1;
    Mix_Music * m_music_2;
    Mix_Music * m_music_3;
    Mix_Music * m_music_4;
    Mix_Chunk * s_frog_jump;
    Mix_Chunk * s_frog_drown;
    Mix_Chunk * s_frog_roadkill;
    Mix_Chunk * s_time_low;
    Mix_Chunk * s_level_win;
    Mix_Chunk * s_game_over;
    Mix_Chunk * s_extra_frog;
    Mix_Chunk * s_goal;
    Mix_Chunk * s_glitched;

    /* Global */
	unsigned int score;
    unsigned int hiScore;
    bool         game_over;
    bool         paused;
    bool         glitched;
    float        show_victory_timer;
    float        button_bounce_timer;
    int          level;
    
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
    
    float        snake_timer;

public:

	virtual void Create(AvancezLib* engine)
	{
		SDL_Log("Game::Create");

		this->engine = engine;
        
        // Grudge
        grudge = new Grudge();
        GrudgeBehaviourComponent * grudge_behaviour = new GrudgeBehaviourComponent();
        grudge_behaviour->Create(engine, grudge, &game_objects);
        RenderComponent * grudge_render = new RenderComponent();
        grudge_render->Create(engine, grudge, &game_objects, "/Users/larsa/Chalmers/TDA572/Data/misc/grudge0.bmp", 4.f);
        grudge_render->AddSprite("/Users/larsa/Chalmers/TDA572/Data/misc/grudge1.bmp");
        grudge->Create();
        grudge->AddComponent(grudge_behaviour);
        grudge->AddComponent(grudge_render);
        grudge->AddReceiver(this);
        game_objects.insert(grudge);
        
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
        
        /* Enemy collider */
        CollideComponent * snake_collider = new CollideComponent();
        snake_collider->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) &snake_pool, HIT);
        
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
        player->AddComponent(snake_collider);
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
        
        
        car_pool_4.Create(8);
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
        
        car_pool_3.Create(5);
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
        
        car_pool_2.Create(8);
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
        
        car_pool_1.Create(8);
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
        
        car_pool_0.Create(8);
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
        
        score100_pool.Create(3);
        for (auto scr = score100_pool.pool.begin(); scr != score100_pool.pool.end(); scr++)
        {
            RenderComponent * scr_renderer = new RenderComponent();
            scr_renderer->Create(engine, *scr, &game_objects, "/Users/larsa/Chalmers/TDA572/Data/misc/score_100.bmp", 1.f);
            (*scr)->Create();
            (*scr)->AddComponent(scr_renderer);
            (*scr)->AddReceiver(this);
            game_objects.insert(*scr);
        }
        score200_pool.Create(3);
        for (auto scr = score200_pool.pool.begin(); scr != score200_pool.pool.end(); scr++)
        {
            RenderComponent * scr_renderer = new RenderComponent();
            scr_renderer->Create(engine, *scr, &game_objects, "/Users/larsa/Chalmers/TDA572/Data/misc/score_200.bmp", 1.f);
            (*scr)->Create();
            (*scr)->AddComponent(scr_renderer);
            (*scr)->AddReceiver(this);
            game_objects.insert(*scr);
        }
        
        snake_pool.Create(2);
        for (auto snake = snake_pool.pool.begin(); snake != snake_pool.pool.end(); snake++)
        {
            SnakeBehaviourComponent * snake_behaviour = new SnakeBehaviourComponent();
            snake_behaviour->Create(engine, *snake, &game_objects);
            RenderComponent * snake_renderer = new RenderComponent();
            snake_renderer->Create(engine, *snake, &game_objects, "/Users/larsa/Chalmers/TDA572/Data/snake/snake0.bmp", 7.f);
            snake_renderer->AddSprite("/Users/larsa/Chalmers/TDA572/Data/snake/snake1.bmp");
            snake_renderer->AddSprite("/Users/larsa/Chalmers/TDA572/Data/snake/snake2.bmp");
            snake_renderer->AddSprite("/Users/larsa/Chalmers/TDA572/Data/snake/snake1.bmp");
            (*snake)->Create();
            (*snake)->AddComponent(snake_behaviour);
            (*snake)->AddComponent(snake_renderer);
            (*snake)->AddReceiver(this);
            game_objects.insert(*snake);
        }
        
        /* Load solo sprites */
		life_sprite  = engine->createSprite("/Users/larsa/Chalmers/TDA572/Data/misc/frog_life.bmp");
        grass_purple = engine->createSprite("/Users/larsa/Chalmers/TDA572/Data/bg/grass_purple.bmp");
        grass_top    = engine->createSprite("/Users/larsa/Chalmers/TDA572/Data/bg/grass_top.bmp");
        level_sprite = engine->createSprite("/Users/larsa/Chalmers/TDA572/Data/misc/level.bmp");
        victory_frog = engine->createSprite("/Users/larsa/Chalmers/TDA572/Data/misc/victory_frog0.bmp");
        victory_frog_wink = engine->createSprite("/Users/larsa/Chalmers/TDA572/Data/misc/victory_frog1.bmp");
        
        /* Load sounds */
        s_frog_jump     = Mix_LoadWAV( "/Users/larsa/Chalmers/TDA572/Data/sounds/frog_jump.wav" );
        s_frog_drown    = Mix_LoadWAV( "/Users/larsa/Chalmers/TDA572/Data/sounds/frog_drown.wav" );
        s_frog_roadkill = Mix_LoadWAV( "/Users/larsa/Chalmers/TDA572/Data/sounds/frog_roadkill.wav" );
        s_time_low      = Mix_LoadWAV( "/Users/larsa/Chalmers/TDA572/Data/sounds/time_low.wav" );
        s_level_win     = Mix_LoadWAV( "/Users/larsa/Chalmers/TDA572/Data/sounds/level_win.wav" );
        s_game_over     = Mix_LoadWAV( "/Users/larsa/Chalmers/TDA572/Data/sounds/game_over.wav" );
        s_extra_frog    = Mix_LoadWAV( "/Users/larsa/Chalmers/TDA572/Data/sounds/extra_frog.wav" );
        s_goal          = Mix_LoadWAV( "/Users/larsa/Chalmers/TDA572/Data/sounds/score.wav" );
        s_glitched      = Mix_LoadWAV( "/Users/larsa/Chalmers/TDA572/Data/sounds/glitched.wav" );
        m_music_menu    = Mix_LoadMUS( "/Users/larsa/Chalmers/TDA572/Data/sounds/theme_menu.wav" );
        m_music_0       = Mix_LoadMUS( "/Users/larsa/Chalmers/TDA572/Data/sounds/theme0.wav" );
        m_music_1       = Mix_LoadMUS( "/Users/larsa/Chalmers/TDA572/Data/sounds/theme1.wav" );
        m_music_2       = Mix_LoadMUS( "/Users/larsa/Chalmers/TDA572/Data/sounds/theme2.wav" );
        m_music_3       = Mix_LoadMUS( "/Users/larsa/Chalmers/TDA572/Data/sounds/theme3.wav" );
        m_music_4       = Mix_LoadMUS( "/Users/larsa/Chalmers/TDA572/Data/sounds/theme4.wav" );
	}

	virtual void Init()
	{
		player->Init();
        int grudge_start_x = percentChance(50) ? SCREEN_WIDTH : -32;
        grudge->Init(grudge_start_x, irandom(SCREEN_HEIGHT));
        grudge->enabled = false;

        /* Globals */
		enabled     = true;
        game_over   = false;
        paused      = false;
        glitched    = false;
        score       = 0;
        hiScore     = 0;
        level       = 0;
        game_timer  = GAME_TIMER;
        extra_frog_counter = 0;
        victory_time       = 0;
        show_victory_timer = 0.f;
        button_bounce_timer = 0.f;
        
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
        snake_timer = 3.f;
        
        /* Pockets */
        pocket0 = false;
        pocket1 = false;
        pocket2 = false;
        pocket3 = false;
        pocket4 = false;
        victory = false;
        wink_timer = 0.f;
        ResetPockets();
        engine->playMusic(m_music_0);
	}
    
    void ResetPockets() {
        pocket0 = pocket1 = pocket2 = pocket3 = pocket4 = 0;
        for (auto pocket = goal_pool.pool.begin(); pocket != goal_pool.pool.end(); pocket++)
        {
            (*pocket)->enabled = false;
        }
        /* Init 5 pockets */
        for (int x = 16; x < SCREEN_WIDTH; x += 96) {
            goal_pool.FirstAvailable()->Init(x, POCKET_ROW);
        }
    }
    
    void NextLevel() {
        player->Reset();
        victory     = false;
        wink_timer  = 0.f;
        game_timer  = GAME_TIMER;
        game_speed *= 1.1;
        level      += 1;
        pocket0 = pocket1 = pocket2 = pocket3 = pocket4 = 0;
        
        ResetPockets();
        
        /* Change music to corresponding level */
        engine->stopMusic();
        switch(level) {
            case 1:
                engine->playMusic(m_music_1);
                break;
            case 2:
                engine->playMusic(m_music_2);
                break;
            case 3:
                engine->playMusic(m_music_3);
                break;
            case 4:
                engine->playMusic(m_music_4);
                break;
            default:
                engine->playMusic(m_music_0);
                break;
        }
    }
    
    void TogglePause() {
        if (button_bounce_timer <= 0.f) {
            paused = !paused;
            if (paused) {
                engine->pauseMixer();
                SDL_Log("Game Paused");
            } else {
                engine->resumeMixer();
                SDL_Log("Game UnPaused");
            }
            button_bounce_timer = .25f;
        }
    }
    
    void Restart() {
        if (button_bounce_timer <= 0.f) {
            player->Init();
            game_over   = false;
            victory     = false;
            score       = 0;
            level       = 0;
            game_timer  = GAME_TIMER;
            game_speed  = 1.f;
            button_bounce_timer = .25f;
            extra_frog_counter  = 0;
            ResetPockets();
            engine->playMusic(m_music_0);
        }
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
        if (keys.pause) {
            TogglePause();
        }
        if (keys.opt0) {
            if (button_bounce_timer <= 0.f) {
                engine->togglePostProcessing();
                button_bounce_timer = .25f;
            }
        }
        
        /* Draw the BG */
        {
            engine->drawRect(0, 0, SCREEN_WIDTH, GRASS_GREEN_ROW_TOP+8, c_river, false);
            int width0 = SCREEN_WIDTH/96;
            int width1 = SCREEN_WIDTH/CELL_S;
            /* Top Grass */
            for (int x = 0; x <= width0; x++) {
                grass_top->draw(x*96, CELL_S + 16);
            }
            /* Purple Grass */
            for (int x = 0; x < width1; x++) {
                grass_purple->draw(x*CELL_S, GRASS_GREEN_ROW_TOP);
                grass_purple->draw(x*CELL_S, GRASS_GREEN_ROW_BOT);
            }
            /* Victory Frogs */
            if (pocket0) {
                if (wink_timer > .5f) victory_frog_wink->draw(16, POCKET_ROW);
                else victory_frog->draw(16, POCKET_ROW);
            }
            if (pocket1) {
                if (wink_timer > 1.f) victory_frog_wink->draw(112, POCKET_ROW);
                else victory_frog->draw(112, POCKET_ROW);
            }
            if (pocket2) {
                if (wink_timer > 1.5f) victory_frog_wink->draw(208, POCKET_ROW);
                else victory_frog->draw(208, POCKET_ROW);
            }
            if (pocket3) {
                if (wink_timer > 2.f) victory_frog_wink->draw(304, POCKET_ROW);
                else victory_frog->draw(304, POCKET_ROW);
            }
            if (pocket4) {
                if (wink_timer > 2.5f) victory_frog_wink->draw(400, POCKET_ROW);
                else victory_frog->draw(400, POCKET_ROW);
            }
        }

        /* Freeze the objects when game over OR Victory OR Paused */
        float running = game_over ? 0.0f : 1.0f;
        if (victory || paused) running = 0.f;
        for (auto go = game_objects.begin(); go != game_objects.end(); go++)
            (*go)->Update(dt * running);
        
        /* Render the snakes above logs and such */
        for (auto snake = snake_pool.pool.begin(); snake != snake_pool.pool.end(); snake++)
        {
            if((*snake)->enabled) (*snake)->Update(0);
        }
        
        /* Make sure player, grudge and animations are rendered last */
        grudge->Update(0);
        player->Update(0);
        player_drown->Update(dt);
        player_roadkill->Update(dt);
        
        if (victory) {
            player->enabled = false;
            wink_timer += (dt/game_speed);
            if (wink_timer > 6.f) NextLevel();
        }
        
        /* Update highscore and victory/button mechanics */
        if (score > hiScore) hiScore = score;
        if (show_victory_timer > 0.f) show_victory_timer -= (dt/game_speed);
        button_bounce_timer = clamp(button_bounce_timer - (dt/game_speed), 0.f, 1.f);
        
        /* Award the player with an extra life each 10,000 pts */
        int old_v = extra_frog_counter;
        extra_frog_counter = score % 10000;
        if (extra_frog_counter < old_v) {
            player->lives++;
            engine->playSound( s_extra_frog );
        }
        
        /* Game Mechanics */
        if (running > 0.f) {
            /* GLITCH */
            bool was_glitched = glitched;
            glitched = engine->getGlitch();
            if (glitched && !was_glitched) {
                engine->pauseMixer();
                engine->playSound( s_glitched );
                grudge->enabled = true;
            } else if (was_glitched && !glitched) {
                engine->resumeMixer();
                grudge->enabled = false;
            }
            if (glitched) grudge->SetTarget(player->horizontalPosition, player->verticalPosition);
            
            
            if (game_timer <= 0.f) {
                if (!game_over) {
                    engine->stopMusic();
                    engine->playSound(s_game_over);
                }
                game_over = true;
            }
            float play_time_low = game_timer;
            game_timer = clamp(game_timer - dt * running, 0.f, 100.f);
            if (play_time_low > 10.f && game_timer <= 10.f) {
                engine->playSound(s_time_low);
            }
            
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
                if (percentChance(10)) {
                    /* SPAWN SNAKE ON LOG */
                    Snake* snake = snake_pool.FirstAvailable();
                    if (snake != NULL) {
                        if (percentChance(50)) snake->Init(-200+random(64), LOG_ROW_1, (SNAKE_SPEED / 2) + FAST_PLATFORM_SPEED, SDL_FLIP_HORIZONTAL);
                        else  snake->Init(-58, LOG_ROW_1, -(SNAKE_SPEED / 2) + FAST_PLATFORM_SPEED, SDL_FLIP_NONE);
                    }
                }
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
                if (percentChance(40)) car4_timer = 4.f;
                car_pool_4.FirstAvailable()->Init(SCREEN_WIDTH, CAR_LANE_4+6, -SLOW_CAR_SPEED);
            }
            if (car3_timer <= 0.f) {
                car3_timer = 2.5f;
                car_pool_3.FirstAvailable()->Init(-32, CAR_LANE_3+2, FAST_CAR_SPEED);
                if (percentChance(25)) car_pool_3.FirstAvailable()->Init(-96, CAR_LANE_3+2, FAST_CAR_SPEED);
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
                if (percentChance(33)) car0_timer = 1.25f;
                car_pool_0.FirstAvailable()->Init(SCREEN_WIDTH, CAR_LANE_0+2, -MEDIUM_CAR_SPEED);
            }
            
            /* Enemies */
            snake_timer -= dt;
            
            if (snake_timer <= 0.f) {
                snake_timer = 15.f;
                if (percentChance(20)) {
                    Snake* snake = snake_pool.FirstAvailable();
                    if (snake != NULL) {
                        if (percentChance(50)) snake->Init(-64, GRASS_GREEN_ROW_TOP, SNAKE_SPEED, SDL_FLIP_HORIZONTAL);
                        else snake->Init(SCREEN_WIDTH, GRASS_GREEN_ROW_TOP, -SNAKE_SPEED, SDL_FLIP_NONE);
                    }
                }
            }
        }
	}
    
	virtual void Draw()
	{
        char text[256];
        int draw_highscore = glitched ? 6660 : hiScore;
        /* Score */
        snprintf(text, 256, "HI-SCORE");
        engine->drawText(SCREEN_WIDTH / 2, 1, text, H_ALIGN::CENTER, V_ALIGN::TOP, c_white);
        snprintf(text, 256, "%05d", draw_highscore);
        engine->drawText(SCREEN_WIDTH / 2, 17, text, H_ALIGN::CENTER, V_ALIGN::TOP, c_red);
        snprintf(text, 256, "1-UP");
        engine->drawText(SCREEN_WIDTH / 4, 1, text, H_ALIGN::RIGHT, V_ALIGN::TOP, c_white);
        if (!glitched) {
            snprintf(text, 256, "%05d", score);
            engine->drawText(SCREEN_WIDTH / 4, 17, text, H_ALIGN::RIGHT, V_ALIGN::TOP, c_red);
        } else {
            snprintf(text, 256, "xxxxxxxx");
            engine->drawText((SCREEN_WIDTH / 3)*2, 64, text, H_ALIGN::LEFT, V_ALIGN::TOP, c_white);
            snprintf(text, 256, "natas");
            engine->drawText(SCREEN_WIDTH / 4, 17, text, H_ALIGN::RIGHT, V_ALIGN::TOP, c_red);
        }
        
        
        /* Time */
        {
            int x0 = SCREEN_WIDTH  - 68 - (4*game_timer);
            int y0 = SCREEN_HEIGHT - 16;
            int x1 = SCREEN_WIDTH  - 68;
            int y1 = SCREEN_HEIGHT;
            snprintf(text, 256, "TIME");
            engine->drawText(SCREEN_WIDTH - 2, SCREEN_HEIGHT - 16, text, H_ALIGN::RIGHT, V_ALIGN::TOP, c_yellow);
            if (game_timer > 10) engine->drawRect(x0, y0, x1, y1, c_green, false);
            else engine->drawRect(x0, y0, x1, y1, c_red, false);
        }
        
        /* Lives */
        {
            int spr_w = life_sprite->getImageWidth() + 2;
            int spr_h = life_sprite->getImageHeight();
            for (int i = 0; i < player->lives; i++) {
                life_sprite->draw(2 + (spr_w * i), SCREEN_HEIGHT - (spr_h*2));
            }
        }
        
        /* Levels */
        {
            int spr_w = level_sprite->getImageWidth() + 2;
            int spr_h = level_sprite->getImageHeight();
            int x = SCREEN_WIDTH - (spr_w*2);
            for (int i = 0; i <= level; i++) {
                level_sprite->draw(x - (spr_w * i), SCREEN_HEIGHT - (spr_h*2));
            }
        }
        
        /* Victory Time */
        if (show_victory_timer > 0.f) {
            snprintf(text, 256, "TIME %d", 60 - victory_time);
            engine->drawText(SCREEN_WIDTH/2, SCREEN_HEIGHT/2+16, text, H_ALIGN::CENTER, V_ALIGN::TOP, c_red, c_black);
        }
        
        /* Game Over */
        if (game_over) {
            snprintf(text, 256, "*** G A M E   O V E R ***");
            engine->drawText(SCREEN_WIDTH / 2, SCREEN_HEIGHT/3, text, H_ALIGN::CENTER, V_ALIGN::TOP, c_red, c_black);
        }
        
        /* Paused */
        if (paused) {
            engine->drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, c_transp, false);
            snprintf(text, 256, "Game Paused");
            engine->drawText(SCREEN_WIDTH / 2, SCREEN_HEIGHT/2.5, text, H_ALIGN::CENTER, V_ALIGN::TOP, c_red);
        }
        
        engine->postProcessing();
		engine->swapBuffers();
		engine->clearWindow();
	}

	virtual void Receive(Message m)
	{
        if (m == FROG_JUMP)
        {
            engine->playSound(s_frog_jump);
        }
		if (m == GAME_OVER)
		{
            SDL_Log("Game Over!");
            game_over = true;
            player->enabled = false;
            engine->stopMusic();
            engine->playSound(s_game_over);
		}
        
        else if (m == PLAYER_DROWN)
        {
            engine->playSound(s_frog_drown);
            SDL_Log("Player Drowned");
            player_drown->Init(player->horizontalPosition, player->verticalPosition);
            
        }
        
        else if (m == PLAYER_ROADKILL)
        {
            engine->playSound(s_frog_roadkill);
            SDL_Log("Player died on land");
            player_roadkill->Init(player->horizontalPosition, player->verticalPosition);
        }
        
        else if (m == VERTICAL_ADVANCEMENT)
        {
            score += 10;
        }
        
        else if (m == POCKET_REACHED)
        {
            engine->playSound( s_goal );
            score += 50;
            score += game_timer * 10;
            victory_time = game_timer;
            show_victory_timer = 4.f;
            game_timer = GAME_TIMER;
            int x = player->horizontalPosition;
            if (x >= 368)      pocket4 = true;
            else if (x >= 272) pocket3 = true;
            else if (x >= 176) pocket2 = true;
            else if (x >= 80)  pocket1 = true;
            else               pocket0 = true;
            
            if (pocket0 && pocket1 && pocket2 && pocket3 && pocket4) {
                /* Victory! */
                victory = true;
                score += 1000;
                engine->stopMusic();
                engine->playSound(s_level_win);
            }
        }
	}

	virtual void Destroy()
	{
		SDL_Log("Game::Destroy");

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Destroy();

        /* Destroy single sprites */
		life_sprite->destroy();
        grass_purple->destroy();
        grass_top->destroy();
        victory_frog->destroy();
        victory_frog_wink->destroy();
        level_sprite->destroy();
        
        /* Free sounds */
        Mix_FreeMusic( m_music_menu );
        Mix_FreeMusic( m_music_0 );
        Mix_FreeMusic( m_music_1 );
        Mix_FreeMusic( m_music_2 );
        Mix_FreeMusic( m_music_3 );
        Mix_FreeMusic( m_music_4 );
        Mix_FreeChunk( s_frog_jump );
        Mix_FreeChunk( s_frog_drown );
        Mix_FreeChunk( s_frog_roadkill );
        Mix_FreeChunk( s_time_low );
        Mix_FreeChunk( s_level_win );
        Mix_FreeChunk( s_game_over );
        Mix_FreeChunk( s_extra_frog );
        Mix_FreeChunk( s_goal );
        Mix_FreeChunk( s_glitched );
        
        Mix_CloseAudio();
        
        delete grudge;
		delete player;
        delete player_drown;
        delete player_roadkill;
	}
};
