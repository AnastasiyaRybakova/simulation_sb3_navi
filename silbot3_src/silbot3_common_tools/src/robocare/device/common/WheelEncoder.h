#ifndef __ROSCIR_COMMON_WHEELENCODER_H__
#define __ROSCIR_COMMON_WHEELENCODER_H__

#include <robocare/device/data/WheelEncoderData.h>

class CIRAPI CWheelEncoder
{
public :
	virtual ~CWheelEncoder() {}


	/** \brief
		read wheel encoder data
	\remarks
		after funtion call, it return wheel encoder data
	\return
		return CWheelEncoderData
	\author
		cspark
	*/
	virtual CWheelEncoderData readData( int index )=0;
};

#endif /* __ROSCIR_COMMON_WHEELENCODER_H__ */
