

#ifndef UTILS_GROOVEAPP_H_
#define UTILS_GROOVEAPP_H_

#include "../PLATFORM.h"
#include "Task.h"
#include "InputListener.h"
#include "../driver/GridTile.h"

class GrooveApp : public Task,public InputListener
{
public:
	virtual ~GrooveApp(){};
	virtual void run() = 0;					//has to be overridden by AppLoop
protected:
	GridTile* tile;							//has to be initialized
};


#endif /* UTILS_GROOVEAPP_H_ */
