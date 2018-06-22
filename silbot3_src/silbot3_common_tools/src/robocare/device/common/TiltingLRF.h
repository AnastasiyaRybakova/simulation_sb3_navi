pragma once

#include <CirAPI/device/devicedata/TiltingLRFData.h>

class CTiltingLRF
{
public :
	
	virtual ~CTiltingLRF() {}

	virtual int scan( int ANGst, int ANGend, int interval, int speed)=0;

	/** \brief
		read tilting lrf data
	\remarks
		after funtion call, it return tilting lrf data
	\return
		return CTiltingLRFData
	\author
		cspark
	*/
	virtual CTiltingLRFData readData()=0;
};
