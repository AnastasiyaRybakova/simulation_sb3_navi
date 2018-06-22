#ifndef __ROSCIR_COMMON_BUMPER_H__
#define __ROSCIR_COMMON_BUMPER_H__

#include <robocare/device/data/BumperData.h>

struct BUMPER_DATA_TYPE
{
	char data[4];
};

class CIRAPI CBumper
{
    public :
        virtual ~CBumper() {}

		/** \brief
			read bumper data
		\remarks
			after funtion call, it return bumper data
		\return
			BumperData
		\author
			cspark
		*/
        virtual CBumperData readData( int index )=0;
};

#endif /* __ROSCIR_COMMON_BUMPER_H__ */
