#include "UltrasonicData.h"

CUltrasonicData::CUltrasonicData()
{
}


CUltrasonicData::CUltrasonicData( int status, unsigned int time, unsigned short* data )
{
	for ( int inx = 0; inx < CUltrasonicData::NUMBER_OF_DATA; inx++ )
	{
		this->data[inx] = data[inx];
	}
}


CUltrasonicData::~CUltrasonicData()
{
}

/**********************************************************************/
/*                                                                    */
/*  get	 Methods                                                   */
/*                                                                    */
/**********************************************************************/
unsigned short* CUltrasonicData::getData()
{
	return this->data;
}