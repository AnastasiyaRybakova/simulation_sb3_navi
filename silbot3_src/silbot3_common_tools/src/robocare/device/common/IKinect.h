/*
* IKinect.h
*
*  Created on: 2011. 12. 26.
*      Author: root
*/

#ifndef IKINECT_H_
#define IKINECT_H_

#include <CirAPI/device/client-stub/message/Packet.h>
#include "IKinectCallback.h"
#include "KinectTypes.h"

class IKinect {
public:
	static const int __NUMBER_OF_DATA = 640 * 480 * 3;
	static const int MAX_DEPTH = 10000;
	virtual ~IKinect() {}

public:

	/** \brief
		readData
	\remarks
		deprecated
	\return
		bool
	\author
		cspark
	*/	
	virtual bool readData(OniDepthInfo& info) = 0;


	/** \brief
		getCom
	\remarks
		deprecated
	\return
		bool
	\author
		cspark
	*/	
	virtual bool getCom(const OniUserid& uid, OniPoint3D& point) = 0;


	/** \brief
		convertRealWorldToProjective
	\remarks
		deprecated
	\return
		bool
	\author
		cspark
	*/	
	virtual bool convertRealWorldToProjective(const OniPoint3D& realworld, OniPoint3D& aProjective) = 0;


	/** \brief
		isTracking
	\remarks
		deprecated
	\return
		bool
	\author
		cspark
	*/	
	virtual bool isTracking(const OniUserid& users) = 0;


	/** \brief
		getSkeletonJointPosition
	\remarks
		deprecated
	\return
		bool
	\author
		cspark
	*/	
	virtual bool getSkeletonJointPosition(const OniUserid& uid, const int& joint, const int& position) = 0;


	/** \brief
		waitOneUpdateAll
	\remarks
		deprecated
	\return
		bool
	\author
		cspark
	*/	
	virtual bool waitOneUpdateAll() = 0;


public:

	/** \brief
		setCallback
	\remarks
		deprecated
	\return
		bool
	\author
		cspark
	*/
	virtual void setCallback(IKinectCallback* callback) = 0;

	/** \brief
		startConsumer
	\remarks
		deprecated
	\return
		bool
	\author
		cspark
	*/
	virtual void startConsumer() = 0;

	/** \brief
		draw
	\remarks
		deprecated
	\return
		bool
	\author
		cspark
	*/
	virtual void draw() = 0;
};

#endif /* IKINECT_H_ */
