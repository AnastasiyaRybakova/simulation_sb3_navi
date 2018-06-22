/**********************************************************************/
/*                                                                    */
/*  Class name      : COutputDeviceType.hpp                           */
/*                                                                    */
/*  Description     : Output Device Type                              */
/*                                                                    */
/**********************************************************************/
/*                        변경 History                                */
/*                                                                    */
/*      DATE           AUTHOR                    DESCRIPTION          */
/*   ----------   ----------------   ------------------------------   */
/*   2005/05/26    JH. KIM           Initial Release                  */
/*                                                                    */
/**********************************************************************/
#ifndef COUTPUTDEVICETYPE_H_
#define COUTPUTDEVICETYPE_H_

#include <robocare/device/CirApi.h>
class CIRAPI COutputDeviceType
{
    public :
        // RTAI.
        static const int RTAI           = 1;

        // 3DSIMULATOR
        static const int SIMULATOR      = 2;

        // STUB(원격 객체 호출을 위함)
        static const int TCP_STUB       = 3;

        // UPnP Device(원격 UPnP Device를 호출하기 위함)
        static const int UPNP_CP        = 4;

		static const int RPC			= 5;
};

#endif
