

#ifndef UTILS_GROOVEAPP_H_
#define UTILS_GROOVEAPP_H_

#include "../driver/BluetoothService.h"
#include "../PLATFORM.h"
#include "Task.h"
#include "../driver/GridTile.h"
#include "../driver/DeltaTimer.h"

class GrooveApp : public Task, CommInterface
{
public:
	GrooveApp(GridTile* tile){this->tile = tile; frameTimer.setTimeStep(FRAMERATE_MS);};
	virtual ~GrooveApp(){};
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void run() = 0;					//has to be overridden by AppLoop
    virtual GrooveApp* new_instance(GridTile *tile) = 0;
    void onCommand(DynamicJsonDocument doc, uint8_t userID){};	//can be overridden
    void load(DynamicJsonDocument *doc){};								//can be overridden
    void save(DynamicJsonDocument *doc){doc->createNestedObject("savegame");};	//create empty savegame object, can be overridden
protected:
	GridTile* tile;							//has to be initialized
	DeltaTimer frameTimer;
};


#endif /* UTILS_GROOVEAPP_H_ */
