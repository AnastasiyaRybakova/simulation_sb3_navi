/*
 * HeadFace.h
 *
 *  Created on: 2014. 2. 11.
 *      Author: msyoo
 */

#ifndef HEADFACE_H_
#define HEADFACE_H_

#include <robocare/device/CirApi.h>
#include <string>

using namespace std;

class CIRAPI CHeadFace
{
public:
	virtual ~CHeadFace(void) {}
	virtual int on()=0;
	virtual int off()=0;
	virtual int stop()=0;
   virtual int express( string emotion, double intensity, int duration )=0;
	virtual int moveAbsolutePosition( int motorID, int speed, int position )=0;
	virtual int moveAbsolutePosition( int motorID, double degree, int speed )=0;
	virtual int moveLip( string visemeTime )=0;
	virtual int moveLipViseme(int visemeID)=0;
	virtual int changeAvatar(int avatarType)=0;
};


#endif /* HEADFACE_H_ */
