

#ifndef UTILS_GROOVEAPP_H_
#define UTILS_GROOVEAPP_H_

#include "../PLATFORM.h"
#include "Task.h"
#include "../driver/COMM.h"
#include "../driver/GridTile.h"

class GrooveApp : public Task
{
public:
	GrooveApp(GridTile* tile){this->tile = tile;};
	virtual ~GrooveApp(){};
	virtual void run() = 0;					//has to be overridden by AppLoop
protected:
	GridTile* tile;							//has to be initialized
};


#endif /* UTILS_GROOVEAPP_H_ */
