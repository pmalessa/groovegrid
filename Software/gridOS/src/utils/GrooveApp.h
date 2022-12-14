

#ifndef UTILS_GROOVEAPP_H_
#define UTILS_GROOVEAPP_H_

#include "../PLATFORM.h"
#include "../driver/BluetoothService.h"
#include "../driver/GridTile.h"
#include "../driver/DeltaTimer.h"

class GrooveApp : public CommInterface
{
public:
	GrooveApp(GridTile* tile){this->tile = tile; frameTimer.setTimeStep(FRAMERATE_MS);};
	virtual ~GrooveApp(){};
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void run() = 0;					//has to be overridden by AppLoop
    void onCommand(CommandMsg *msg){};	//can be overridden
    void load(DynamicJsonDocument *doc){};								//can be overridden
    void save(DynamicJsonDocument *doc){doc->createNestedObject("savegame");};	//create empty savegame object, can be overridden
protected:
	GridTile* tile;							//has to be initialized
	DeltaTimer frameTimer;
};


#endif /* UTILS_GROOVEAPP_H_ */
