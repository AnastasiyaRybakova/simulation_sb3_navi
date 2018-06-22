/**********************************************************************/
/*                                                                    */
/*  Class name      : CInputDeviceType.hpp                            */
/*                                                                    */
/*  Description     : Input Device Type                               */
/*                                                                    */
/**********************************************************************/
/*								History                               */
/*                                                                    */
/*      DATE           AUTHOR                    DESCRIPTION          */
/*   ----------   ----------------   ------------------------------   */
/*   2005/05/26    JH. KIM           Initial Release                  */
/*                                                                    */
/**********************************************************************/
#ifndef CINPUTDEVICETYPE_H_
#define CINPUTDEVICETYPE_H_
#include <device/CirApi.h>
class CIRAPI CInputDeviceType
{
    public :
        // RTAI.
        static const int RTAI           = 1;

        // SIMULATOR
        static const int SIMULATOR      = 2;

        // TCP
        static const int TCP_STUB       = 3;

        // UPnP Control Point
        static const int UPNP_CP        = 4;

		static const int RPC			= 5;
};

#endif
