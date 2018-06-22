#include "TouchButtonData.h"
#include <iostream>
using namespace std;

CHeadTouchButtonData::CHeadTouchButtonData() //: CInputDeviceData( 0, 0 )
{
	for( int i = 0; i < NUMBER_OF_DATA; i++ )
	{
		for( int j = 0; j < NUMBER_OF_TOUCHBUTTON; j++ )
		{
			this->data[i][j] = 0;
		}
	}

}

CHeadTouchButtonData::CHeadTouchButtonData( int status, unsigned int time, unsigned int (*data)[18] )
//    : CInputDeviceData( status, time )
{
	cout << "CHeadTouchButtonData Create.." << endl;

    //memcpy( this->data, data, sizeof(double)*( NUMBER_OF_DATA * NUMBER_OF_TOUCHBUTTON ) );

	for( int i = 0; i < NUMBER_OF_DATA; i++ )
	{
		for( int j = 0; j < NUMBER_OF_TOUCHBUTTON; j++ )
		{
			this->data[i][j] = data[i][j];
		}
	}

//    this->status = status;
//    this->time = time;

}

CTouchButtonData::CTouchButtonData(unsigned short* data, int dataCount) {
	this->numberOfData = dataCount;
	for ( int inx = 0; inx < this->numberOfData; inx++ )
	{
		this->data[inx] = data[inx];
	}
}

CHeadTouchButtonData::~CHeadTouchButtonData()
{
	cout << "CHeadTouchButtonData Destructor.." << endl;
}


unsigned int* CHeadTouchButtonData::getData()
{
	  return ( unsigned int* )( this->data );
}


CTouchButtonData::CTouchButtonData() //: CInputDeviceData( 0, 0 )
{
    for ( int inx = 0; inx < CTouchButtonData::NUMBER_OF_DATA; inx++ )
    {
        this->data[inx] = inx;
    }
}


CTouchButtonData::CTouchButtonData( int status, unsigned int time, unsigned int* data )
//    : CInputDeviceData( status, time )
{
    for ( int inx = 0; inx < CTouchButtonData::NUMBER_OF_DATA; inx++ )
    {
        this->data[inx] = data[inx];
    }
}


CTouchButtonData::~CTouchButtonData()
{
}


unsigned int* CTouchButtonData::getData()
{
    return this->data;
}

