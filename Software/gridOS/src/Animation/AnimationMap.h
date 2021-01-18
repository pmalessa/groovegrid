
#ifndef ANIMATIONMAP_H_
#define ANIMATIONMAP_H_

#include "PLATFORM.h"

#include "RandomLineAnimation.h"
#include "RandomPixelAnimation.h"
#include "RandomRectAnimation.h"
#include "RandomCircleAnimation.h"
#include "ColorPaletteAnimation.h"
#include "NFSAnimation.h"
#include "SimplyRedAnimation.h"
#include "MatrixAnimation.h"
#include "SpectrumAnimation.h"
#include "ShotAnimation.h"
#include "WalkingDotAnimation.h"
#include "UnicolorAnimation.h"
//balou elisa gonta björn magareto pip kk kobaanwa
inline std::map<std::string, std::function<GrooveAnimation*(GridTile*)>> animationMap = {
	{"Color Palette"		,[](GridTile *tile){return new ColorPaletteAnimation(tile);}},
	{"Simply Red"			,[](GridTile *tile){return new SimplyRedAnimation(tile);}},
	{"Dancefloor"			,[](GridTile *tile){return new RandomCircleAnimation(tile);}},
	{"Matrix"				,[](GridTile *tile){return new MatrixAnimation(tile);}},
	{"Need for Speed"		,[](GridTile *tile){return new NFSAnimation(tile);}},
	{"Spectrum"				,[](GridTile *tile){return new SpectrumAnimation(tile);}},
	{"Shot"					,[](GridTile *tile){return new ShotAnimation(tile);}},
	{"Walking Dot"			,[](GridTile *tile){return new WalkingDotAnimation(tile);}},
	{"Unicolor"				,[](GridTile *tile){return new UnicolorAnimation(tile);}}
};

#endif //ANIMATIONMAP_H_