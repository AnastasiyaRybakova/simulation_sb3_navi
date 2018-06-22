#ifndef __ROSCIR_COMMON_LED_H__
#define __ROSCIR_COMMON_LED_H__

#include <robocare/device/CirApi.h>

class CIRAPI CLightEmittingDiode
{
public:

	/** \brief
		establishing connection 
	\remarks
		try to connect device-server
	\return
		1:success  0:fail
	\author
		cspark
	*/
	virtual int on( int id, int color )=0;

	/** \brief
		establishing connection 
	\remarks
		try to connect device-server
	\return
		1:success  0:fail
	\author
		cspark
	*/
	virtual int on(int id, unsigned char bright, unsigned char red, unsigned char green, unsigned char blue) = 0;
	virtual ~CLightEmittingDiode(void) {}
};

#endif /* __ROSCIR_COMMON_LED_H__ */
