#ifndef __ROSCIR_COMMON_PSDBUMPER_H__
#define __ROSCIR_COMMON_PSDBUMPER_H__

#include <robocare/device/CirApi.h>

class CIRAPI CPSDBumper {
public:
	static const int __MEASURE_DISTANCE = 30;
	static const int __NUMBER_OF_DATA = 3;
public:
	virtual ~CPSDBumper() {}
public:


	/** \brief
		read psdbumper data
	\remarks
		after funtion call, it return psdbumper data
	\return
		return distance array
	\author
		cspark
	*/
	virtual int readData(int distance, int v[3]) = 0;
};


#endif /* __ROSCIR_COMMON_PSDBUMPER_H__ */
