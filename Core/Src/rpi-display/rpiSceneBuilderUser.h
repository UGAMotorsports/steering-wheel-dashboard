/*
 * rpiSceneBuilderUser.h
 *
 *  Created on: May 2, 2025
 *      Author: bhargav
 */

#ifndef SRC_RPI_DISPLAY_RPISCENEBUILDERUSER_H_
#define SRC_RPI_DISPLAY_RPISCENEBUILDERUSER_H_


#ifdef __cplusplus
extern "C" {
#endif
	void domainscreen();
	void setrpmdata(char *rpmvalue);
	void settempdata(char *tempvalue);
	void setgeardata(char *gearvalue);
	void setbattdata(char *battvalue);
	void dosplashscene();
#ifdef __cplusplus
}
#endif


#endif /* SRC_RPI_DISPLAY_RPISCENEBUILDERUSER_H_ */
