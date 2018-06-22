#ifndef __ROSCIR_DATA_ARM_H__
#define __ROSCIR_DATA_ARM_H__

#include <ostream>
#include <robocare/device/CirApi.h>

using namespace std;

class CIRAPI CArmData //: public CInputDeviceData
{

    public :
        int status;
        unsigned int time;

    public :
        static const int NUMBER_OF_DATA = 14;

    private :
        float data[ NUMBER_OF_DATA ];


    public :
        CArmData();
        CArmData( int status, unsigned int time, float* data );
        ~CArmData();

        float* getData();
        friend ostream& operator<<( ostream& os, const CArmData& armData );
};

#endif /* __ROSCIR_DATA_ARM_H__ */