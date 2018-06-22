/*
 * CirApi.h
 *
 *  Created on: 2011. 12. 2.
 *      Author: ijpark
 *
 *  Modified on: 2011. 12. 15
 *  	Author: skkang
 */

/** \mainpage CirAPI

\section developer Developer
- cspark
- skkang
- ijpark

\section info Information
- Cir-Robot's framework api 
*/

#ifndef CIRAPI_H_
#define CIRAPI_H_

#pragma warning (disable : 4819)


#ifdef __GNUC__
	#define CIRAPI
#else
#ifdef _WINDOWS
        #define CIRAPI __declspec(dllexport)
#else
        #define CIRAPI __declspec(dllimport)
#endif

#endif /* __GNUC__ */


#endif /* CIRAPI_H_ */
