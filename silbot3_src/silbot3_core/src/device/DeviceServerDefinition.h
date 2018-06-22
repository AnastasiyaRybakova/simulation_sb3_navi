#ifndef __DEVICE_SERVER_DEFINITION_H__
#define __DEVICE_SERVER_DEFINITION_H__

//ROBOT_TYPE
#define ROBOT_TYPE_TROT					1
#define ROBOT_TYPE_CIROS				2
#define ROBOT_TYPE_FRI					3
#define ROBOT_TYPE_KIBO					4
#define ROBOT_TYPE_ENGKEY				5
#define ROBOT_TYPE_MERO					7
#define ROBOT_TYPE_MERO_S				8
#define ROBOT_TYPE_EROBOT				11

//SERVER_DEVICE_TYPE
#define SERVER_DEVICE_REAL				1
#define SERVER_DEVICE_SIMULATOR		2
#define SERVER_DEVICE_BOTH				3
#define SERVER_DEVICE_MOCK				6


//HEAD_FACE_TYPE
#define REAL_HARDWARE			0
#define AVATAR				1


// Message
#define NO_MATCHING_SERVER_TYPE			"no ServerDevice Type (1=Real, 2=3DSimulator  6=MOCK) " 
#define NO_MATCHING_ROBOT_TYPE			"no Robot Type (1=Trot, 2=Ciros, 3=Fri, 4=Kibo, 5=Engkey) "








// limit of input values
#define PAN_ANGLE_MAX					100
#define PAN_ANGLE_MIN					0
#define TILT_ANGLE_MAX					90
#define TILT_ANGLE_MIN					-90
#define PANTILT_SPEED_MAX				50
#define WHEEL_VELOCITY_MAX				1000
#define WHEEL_VELOCITY_MIN				-1000
#define WHEEL_ANGLE_MAX					360
#define WHEEL_ANGLE_MIN					-360
#define ARM_0_MAX						180 // shoulder pitch
#define ARM_0_MIN						-2
#define ARM_1_MAX						90 // shoulder roll
#define ARM_1_MIN						0
#define ARM_2_MAX						40 // elbow pitch
#define ARM_2_MIN						-20
#define SPEED_MIN						0
#define SPEED_MAX						20


#define WHEEL_ON 1
#define WHEEL_OFF 2
#define WHEEL_VELOCITY_CONTROL 3
#define WHEEL_STOP 4
#define M_PI 3.14159265358979323846

#define MIN_TRANSLATIONAL_VELOCITY -1000
#define MAX_TRANSLATIONAL_VELOCITY 1000

// (-800~800 mm/sec
#define MIN_ROTATIONAL_VELOCITY -1000
#define MAX_ROTATIONAL_VELOCITY 1000

// Base
#define ROBOT_BASE_SIZE 10

//  : 4.88, : 5.06
#define GAIN_VALUE 5.06

#endif /* __DEVICE_SERVER_DEFINITION_H__ */
