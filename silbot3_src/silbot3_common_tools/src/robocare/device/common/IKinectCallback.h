/*
* IKinectCallback.h
*
*  Created on: 2011. 12. 26.
*      Author: root
*/

#ifndef IKINECTCALLBACK_H_
#define IKINECTCALLBACK_H_

#include "KinectTypes.h"
#include <string>

using namespace std;

class IKinectCallback {
public:

	/** \brief
		onNewUser
	\remarks
		deprecated
	\return
		bool
	\author
		cspark
	*/
	virtual void onNewUser(unsigned int uid, void* cookie)  = 0;

	/** \brief
		onLostUser
	\remarks
		deprecated
	\return
		bool
	\author
		cspark
	*/
	virtual void onLostUser(unsigned int uid, void* cookie)  = 0;
	
	/** \brief
		onCalibratioinInProgress
	\remarks
		deprecated
	\return
		bool
	\author
		cspark
	*/
	virtual void onCalibratioinInProgress(unsigned int uid, void* cookie) = 0;

	/** \brief
		onUserPoseDetected
	\remarks
		deprecated
	\return
		bool
	\author
		cspark
	*/
	virtual void onUserPoseDetected(const string& pose, unsigned int id, void* cookie) = 0;

	/** \brief
		onCalibrationStart
	\remarks
		deprecated
	\return
		bool
	\author
		cspark
	*/
	virtual void onCalibrationStart(unsigned int uid, void* cookie) = 0;

	/** \brief
		onCalibrationEnd
	\remarks
		deprecated
	\return
		bool
	\author
		cspark
	*/
	virtual void onCalibrationEnd(unsigned int uid, void* cookie) = 0;

	/** \brief
		onCalibrationComplete
	\remarks
		deprecated
	\return
		bool
	\author
		cspark
	*/
	virtual void onCalibrationComplete(unsigned int uid, void* cookie) = 0;

public:
	virtual ~IKinectCallback(){}

};


#endif /* IKINECTCALLBACK_H_ */
