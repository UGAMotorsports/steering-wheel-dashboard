/*
 * rpiSceneBuilderUser.cpp
 *
 *  Created on: May 2, 2025
 *      Author: bhargav
 */

#include "rpiSceneBuilderUser.h"

#include "rpiDisplaySceneBuilder.h"
#include "FreeSans18pt7b.h"
#include "FreeMonoBold24pt7b.h"
#include "splashimage.h"
#include <cstring>

char rpmresult[17] = "rpm: ";
char tempresult[18] = "temp: ";
char gearresult[20] = "GEAR: ";
DisplayObject* otherobjects[3] = {
		new StringObject(240, 160, 0xFFFF, FREE_MONO_BOLD_24PT7B, CENTER_OBJECT, rpmresult, 1),
		new StringObject(240, 100, 0xFFFF, FREE_SANS_18PT7B, CENTER_OBJECT, tempresult, 3),
		new StringObject(240, 200, 0xFFFF, FREE_MONO_BOLD_24PT7B, CENTER_OBJECT, gearresult, 2)
};
Scene myScene2(otherobjects, 3);

DisplayObject* splashobjects[3] = {
		new ImageObject(epd_bitmap_Screenshot_from_2025_05_03_23_43_00, SPLASH_LENGTH, SPLASH_HEIGHT, 240, 160, CENTER_OBJECT, 2)
};
Scene splashScene(splashobjects, 1);

void setrpmdata(char *rpmvalue) {
	char newrpmresult[17] = "rpm: ";
	strncpy(rpmresult, newrpmresult, 16);
	strncat(rpmresult, rpmvalue, 10);
	((StringObject*)otherobjects[0])->updateString(rpmresult, CENTER_OBJECT, 0xFFFF, FREE_MONO_BOLD_24PT7B, 240, 160, 1);
}

void settempdata(char *tempvalue) {
	char newtempresult[18] = "temp: ";
	strncpy(tempresult, newtempresult, 17);
	strncat(tempresult, tempvalue, 10);
	((StringObject*)otherobjects[1])->updateString(tempresult, CENTER_OBJECT, 0xFFFF, FREE_SANS_18PT7B, 240, 100, 3);

}

void setgeardata(char *gearvalue) {
	char newgearresult[20] = "GEAR: ";
	strncpy(gearresult, newgearresult, 20);
	strncat(gearresult, gearvalue, 10);
	((StringObject*)otherobjects[2])->updateString(gearresult, CENTER_OBJECT, 0xFFFF, FREE_MONO_BOLD_24PT7B, 240, 200, 2);
}

void domainscreen() {
	myScene2.drawScene();
	myScene2.setScene(otherobjects, 3);
}

void dosplashscene() {
	splashScene.drawScene();
	splashScene.setScene(splashobjects, 1);
}

