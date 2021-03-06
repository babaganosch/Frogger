#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "avancezlib.h"
#include "object_pool.h"
#include <string>
#include <set>

const std::string data_path = "/Users/larsa/Chalmers/TDA572/Data/";

const unsigned int  SCREEN_WIDTH     = 448;
const unsigned int  SCREEN_HEIGHT    = 512;
const unsigned int  CELL_S           = 32;

const unsigned int  POCKET_ROW       = CELL_S*2;

const unsigned int  LOG_ROW_2        = SCREEN_HEIGHT-(CELL_S*13);
const unsigned int  TURTLE_ROW_1     = SCREEN_HEIGHT-(CELL_S*12);
const unsigned int  LOG_ROW_1        = SCREEN_HEIGHT-(CELL_S*11);
const unsigned int  LOG_ROW_0        = SCREEN_HEIGHT-(CELL_S*10);
const unsigned int  TURTLE_ROW_0     = SCREEN_HEIGHT-(CELL_S*9);

const unsigned int  CAR_LANE_4       = SCREEN_HEIGHT-(CELL_S*7);
const unsigned int  CAR_LANE_3       = SCREEN_HEIGHT-(CELL_S*6);
const unsigned int  CAR_LANE_2       = SCREEN_HEIGHT-(CELL_S*5);
const unsigned int  CAR_LANE_1       = SCREEN_HEIGHT-(CELL_S*4);
const unsigned int  CAR_LANE_0       = SCREEN_HEIGHT-(CELL_S*3);

const unsigned int  GRASS_GREEN_ROW_TOP = SCREEN_HEIGHT-(CELL_S*8);
const unsigned int  GRASS_GREEN_ROW_BOT = SCREEN_HEIGHT-(CELL_S*2);

const unsigned int  NUM_LIVES        = 5;
const float GAME_TIMER               = 60.f;

const float SLOW_PLATFORM_SPEED      = 60.f;
const float FAST_PLATFORM_SPEED      = 120.f;

const float SLOW_CAR_SPEED           = 60.f;
const float MEDIUM_CAR_SPEED         = 90.f;
const float FAST_CAR_SPEED           = 200.f;

const float SNAKE_SPEED              = 40.f;

/* Draw colors */
const SDL_Color c_white  = { 255, 255, 255, 255 };
const SDL_Color c_black  = { 0,   0,   0,   255 };
const SDL_Color c_transp = { 0,   0,   0,   128 };
const SDL_Color c_red    = { 255, 0,   0,   255 };
const SDL_Color c_yellow = { 255, 255, 0,   255 };
const SDL_Color c_blue   = { 0,   0,   255, 255 };
const SDL_Color c_green  = { 33,  222, 0,   255 };
const SDL_Color c_river  = { 0,   0,   71,  255 };
const SDL_Color c_hell   = { 151, 0,   247, 255 }; /* Purple, but gets shifted to about RED */




float game_speed = 1.f; // Game speed is increased each level (5 delivered frogs)


#include "util.h"
#include "component.h"
#include "game_object.h"

#include "player.h"
#include "turtle.h"
#include "log.h"
#include "car.h"
#include "pocket.h"
#include "animation.h"
#include "snake.h"
#include "grudge.h"
#include "bug.h"
#include "croc.h"
#include "crocodile.h"
#include "pause_screen.h"

#include "game.h"

int main(int argc, char** argv)
{

	AvancezLib engine;

	engine.init(SCREEN_WIDTH, SCREEN_HEIGHT);

	Game game;
	game.Create(&engine);
	game.Init();

	float lastTime = engine.getElapsedTime();
	while (true)
	{
		float newTime = engine.getElapsedTime();
		float dt = newTime - lastTime;
		dt = dt * game_speed;
		lastTime = newTime;

		engine.processInput();
		game.Update(dt);
		game.Draw();
	}

	return 0;
}



