#ifndef __ROSCIR_DATA_TOUCHBUTTON_H__
#define __ROSCIR_DATA_TOUCHBUTTON_H__


#include <robocare/device/CirApi.h>

using namespace std;

class CIRAPI CTouchButtonData
{
    public :
        static const int NUMBER_OF_DATA = 6;
		unsigned int data[ NUMBER_OF_DATA ];
		int numberOfData;

    public :
        CTouchButtonData();
        CTouchButtonData( int status, unsigned int time, unsigned int* data );
		CTouchButtonData(unsigned short* data, int numbeOfData);
        ~CTouchButtonData();

        unsigned int* getData();
        //friend ostream& operator<<( ostream& os, const CTouchButtonData& touchButtonData );
};


class CIRAPI CHeadTouchButtonData //: public CInputDeviceData
{
    public :
        static const int NUMBER_OF_DATA = 10;
        static const int NUMBER_OF_TOUCHBUTTON = 18;
		unsigned int data[ NUMBER_OF_DATA ][ NUMBER_OF_TOUCHBUTTON ];
    
    public :
        CHeadTouchButtonData();
        CHeadTouchButtonData( int status, unsigned int time, unsigned int (*data)[18] );
        ~CHeadTouchButtonData();

        unsigned int* getData();
        //friend ostream& operator<<( ostream& os, const CHeadTouchButtonData& headTouchButtonData );
};

#endif /* __ROSCIR_DATA_TOUCHBUTTON_H__ */
