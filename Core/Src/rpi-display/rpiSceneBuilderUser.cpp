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
#include "gearFont.h"
#include "iconFont.h"
#include "splashimage.h"
#include <cstring>

char rpmresult[17] = "";
char tempresult[18] = "";
char speedresult[20] = "";
char gearresult[20] = "8";
char battresult[20] = "";
char tempicon[3] = " ";
char batticon[3] = "\"";
char rpmicon[3] = "#";
char speedicon[3] = "%";
DisplayObject* otherobjects[10] = {
		new StringObject(200, 260, 0xFFFF, FREE_MONO_BOLD_24PT7B, NO_CENTER_OBJECT, rpmresult, 1),
		new StringObject(350, 100, 0xFFFF, FREE_SANS_18PT7B, CENTER_OBJECT, tempresult, 3),
		new StringObject(350, 60, 0xFFFF, FREE_SANS_18PT7B, CENTER_OBJECT, battresult, 6),
		new StringObject(100, 240, 0xFFFF, GEARFONT, CENTER_OBJECT, gearresult, 2),
		new StringObject(395, 130, 0xFFFF, ICONFONT, CENTER_OBJECT, tempicon, 4),
		new StringObject(400, 90, 0xFFFF, ICONFONT, CENTER_OBJECT, batticon, 5),
		new StringObject(375, 185, 0xFFFF, FREE_SANS_18PT7B, LEFTDRAW_OBJECT, speedresult, 7),
		new StringObject(440, 288, 0xFFFF, ICONFONT, LEFTDRAW_OBJECT, rpmicon, 8),
		new OutlineRectObject(190, 250, 260, 55, 0xFFFF, NO_CENTER_OBJECT, 9),
		new StringObject(405, 210, 0xFFFF, ICONFONT, CENTER_OBJECT, speedicon, 10)
};
Scene myScene2(otherobjects, 6);

char *image = "splash.bin";
DisplayObject* splashobjects[1] = {
		new ImageObject(image, SPLASH_LENGTH, SPLASH_HEIGHT, 240, 160, CENTER_OBJECT, 2)
};
Scene splashScene(splashobjects, 1);

void setrpmdata(char *rpmvalue) {
	strncpy(rpmresult, "", 10);
	strncat(rpmresult, rpmvalue, 10);
	((StringObject*)otherobjects[0])->updateString(rpmresult, NO_CENTER_OBJECT, 0xFFFF, FREE_MONO_BOLD_24PT7B, 200, 260, 1);
}

void settempdata(char *tempvalue) {
	strncpy(tempresult, "", 10);
	strncat(tempresult, tempvalue, 10);
	((StringObject*)otherobjects[1])->updateString(tempresult, CENTER_OBJECT, 0xFFFF, FREE_SANS_18PT7B, 350, 100, 3);

}

void setgeardata(char *gearvalue) {
	strncpy(gearresult, "", 10);
	strncat(gearresult, gearvalue, 10);
	((StringObject*)otherobjects[3])->updateString(gearresult, CENTER_OBJECT, 0xFFFF, GEARFONT, 100, 240, 2);
}

void setbattdata(char *battvalue) {
	strncpy(battresult, "", 10);
	strncat(battresult, battvalue, 10);
	((StringObject*)otherobjects[2])->updateString(battresult, CENTER_OBJECT, 0xFFFF, FREE_SANS_18PT7B, 350, 60, 6);
}

void setspeeddata(char *speedvalue) {
	strncpy(speedresult, "", 10);
	strncat(speedresult, speedvalue, 10);
	((StringObject*)otherobjects[6])->updateString(speedresult, LEFTDRAW_OBJECT, 0xFFFF, FREE_SANS_18PT7B, 375, 185, 7);
}

void domainscreen() {
	myScene2.drawScene();
	myScene2.setScene(otherobjects, 10);
}

void dosplashscene() {
	splashScene.drawScene();
	splashScene.setScene(splashobjects, 1);
}

