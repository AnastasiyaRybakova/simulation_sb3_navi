#include "PanTiltPosition.h"


CPanTiltPosition::CPanTiltPosition()
{
	this->degreeLeftRight = 0.0;
	this->degreeUpDown = 0.0;
}

CPanTiltPosition::~CPanTiltPosition()
{
}
/**********************************************************************/
/*                                                                    */
/*  get/set Methods                                                   */
/*                                                                    */
/**********************************************************************/
double CPanTiltPosition::getDegreeLeftRight()   { return this->degreeLeftRight; }
double CPanTiltPosition::getDegreeUpDown()      { return this->degreeUpDown; }
int CPanTiltPosition::getEncoderLeft()          { return this->encoderLeft; }
int CPanTiltPosition::getEncoderRight()         { return this->encoderRight; }
int CPanTiltPosition::getXtionTilt()				{ return this->xtionTilt; }

void CPanTiltPosition::setDegreeLeftRight( double degreeLeftRight )     { this->degreeLeftRight = degreeLeftRight; }
void CPanTiltPosition::setDegreeUpDown( double degreeUpDown )           { this->degreeUpDown = degreeUpDown; }
void CPanTiltPosition::setEncoderLeft( int encoderLeft )                { this->encoderLeft = encoderLeft; }
void CPanTiltPosition::setEncoderRight( int encoderRight )              { this->encoderRight = encoderRight; }
void CPanTiltPosition::setXtionTilt(int tiltAngle)								{ this->xtionTilt = tiltAngle; }


/**********************************************************************/
/*                                                                    */
/*  Method Name     : operator<<                                      */
/*                                                                    */
/*  Description     : operator overloading                            */
/*                                                                    */
/*  Parameters      : ostream& - ostream                              */
/*                    const CPanTiltPosition& - CPanTiltPosition      */
/*                                                                    */
/*  Return Value    : ostream& - ostream                              */
/*                                                                    */
/**********************************************************************/
ostream& operator<<( ostream& os, const CPanTiltPosition& position )
{
	os << " CPanTiltPosition ( degreeLR, degreeUD, encL, encR ) = ( "
		<< position.degreeLeftRight << ", "
		<< position.degreeUpDown << ", "
		<< position.encoderLeft << ", "
		<< position.encoderRight << ","
		<< position.xtionTilt << " )" << endl;

	return os;
}

