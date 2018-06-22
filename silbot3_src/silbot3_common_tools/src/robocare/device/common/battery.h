#ifndef __ROSCIR_COMMON_BATTERY_H__
#define __ROSCIR_COMMON_BATTERY_H__

#include <robocare/device/data/BatteryData.h>

class CBattery
{
public :
	virtual ~CBattery() {}
public:

	/** \brief
		read battery data
	\remarks
		after funtion call, it return battery data
	\return
		BatteryData
	\author
		cspark
	*/
	virtual CBatteryData readData( int index )=0;
	

	/** \brief
		check docked
	\remarks
		check robot is docked
	\return
		true / false
	\author
		cspark
	*/
	virtual bool isDocked(const int& id) = 0;
	
	
	/** \brief
		check charge
	\remarks
		check robot do charge
	\return
		true / false
	\author
		cspark
	*/
	virtual bool doCharge(const int& id) = 0;
};

#endif /* __ROSCIR_COMMON_BATTERY_H__ */