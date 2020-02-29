#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "avancezlib.h"
#include "object_pool.h"

#include <set>

const unsigned int  SCREEN_WIDTH         = 448;
const unsigned int  SCREEN_HEIGHT        = 512;
const unsigned int  CELL_S               = 32;

const unsigned int	NUM_LIVES            = 5;

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

const float GAME_TIMER               = 60.f;

const float SLOW_PLATFORM_SPEED      = 60.f;
const float FAST_PLATFORM_SPEED      = 120.f;

const float SLOW_CAR_SPEED           = 60.f;
const float MEDIUM_CAR_SPEED         = 90.f;
const float FAST_CAR_SPEED           = 200.f;



float game_speed = 1.f;		// speed of the game; it is increased each time all the aliens are hit
							// it is also the score multiplier

#include "util.h"
#include "component.h"
#include "game_object.h"

#include "player.h"
#include "turtle.h"
#include "log.h"
#include "car.h"
#include "pocket.h"
#include "animation.h"

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



