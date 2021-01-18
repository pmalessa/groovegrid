#ifndef APPMAP_H_
#define APPMAP_H_

#include "PLATFORM.h"

#include "Animation/AnimationRunner.h"
#include "Game/Game_2048.h"
#include "Game/FlappyGroove.h"
#include "Game/Snake.h"
#include "Game/SwipeMaster.h"
#include "Game/Battleship.h"

inline std::map<std::string, std::function<GrooveApp*(GridTile*)>> appMap = {
	{"AnimationRunner"	,[](GridTile *tile){return new AnimationRunner(tile);}},
	{"2048"				,[](GridTile *tile){return new Game_2048(tile);}},
	{"Flappy Groove"	,[](GridTile *tile){return new FlappyGroove(tile);}},
	{"Snake"			,[](GridTile *tile){return new SnakeGame(tile);}},
	{"Battleship"		,[](GridTile *tile){return new Battleship(tile);}},
	{"Swipe Master"		,[](GridTile *tile){return new SwipeMaster(tile);}}
};

#endif //APPMAP_H_