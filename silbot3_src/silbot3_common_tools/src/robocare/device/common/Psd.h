#ifndef __ROSCIR_COMMON_PSD_H__
#define __ROSCIR_COMMON_PSD_H__


#include <robocare/device/data/PsdData.h>

class CIRAPI CPSD {
public:
	virtual ~CPSD(void) {}


	/** \brief
		read psd data
	\remarks
		after funtion call, it return psd data
	\return
		return CPSDData
	\author
		cspark
	*/
	virtual int readData(int returnMethod, CPSDData& data) = 0;
};


#endif /* __ROSCIR_COMMON_PSD_H__ */ 
