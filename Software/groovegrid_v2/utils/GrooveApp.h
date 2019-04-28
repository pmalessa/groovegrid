

#ifndef UTILS_GROOVEAPP_H_
#define UTILS_GROOVEAPP_H_

#include "../PLATFORM.h"
#include "Task.h"
#include "../driver/COMM.h"
#include "../driver/GridTile.h"
#include "../driver/DeltaTimer.h"

class GrooveApp : public Task
{
public:
	GrooveApp(GridTile* tile){this->tile = tile; frameTimer.setTimeStep(FRAMERATE_MS);};
	virtual ~GrooveApp(){};
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void run() = 0;					//has to be overridden by AppLoop
protected:
	GridTile* tile;							//has to be initialized
	DeltaTimer frameTimer;
};


#endif /* UTILS_GROOVEAPP_H_ */
