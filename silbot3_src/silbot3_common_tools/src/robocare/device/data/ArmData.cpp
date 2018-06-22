#include "ArmData.h"

CArmData::CArmData() //: CInputDeviceData( 0, 0 )
{
}

CArmData::CArmData( int status, unsigned int time, float* data )
//    : CInputDeviceData( status, time )
{
    for ( int inx = 0; inx < CArmData::NUMBER_OF_DATA; inx++ )
    {
        this->data[inx] = data[inx];
    }
}

CArmData::~CArmData()
{
}

float* CArmData::getData()
{
    return this->data;
}

ostream& operator<<( ostream& os, const CArmData& armData )
{
    os << "[CArmData] :"
       << " status = [" << armData.status << "], "
       << " time = [" << armData.time << "]" << endl;

    for ( int inx = 0; inx < CArmData::NUMBER_OF_DATA; inx++ )
        os << "data[" << inx << "]=[" << armData.data[inx] << "], ";

    return os;
}
