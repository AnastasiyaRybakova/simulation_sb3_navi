#ifndef __ROSCIR_COMMON_TOUCHBUTTON_H__
#define __ROSCIR_COMMON_TOUCHBUTTON_H__

#include <robocare/device/data/TouchButtonData.h>

struct TOUCH_BUTTON_DATA_TYPE 
{
	unsigned int data[6];
};

class CIRAPI CTouchButton
{

public:
	virtual ~CTouchButton(void) {}

	/** \brief
		read touch button data
	\remarks
		after funtion call, it return touch button data
	\return
		return CTouchButtonData
	\author
		cspark
	*/
    virtual CTouchButtonData readData( int index )=0;

	

	/** \brief
		read head touch data
	\remarks
		after funtion call, it return head touch data
	\return
		return CHeadTouchButtonData
	\author
		cspark
	*/
	virtual CHeadTouchButtonData readHeadData( int index )=0;
	virtual int detectingError(int touchID)=0;

};

#endif /* __ROSCIR_COMMON_TOUCHBUTTON_H__ */
