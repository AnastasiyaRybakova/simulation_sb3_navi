#include "ModalityUseSet.h"
#include <ros/ros.h>

namespace expression {
	namespace modality {

		ModalityUseSet::ModalityUseSet(void)
		{
			m_arm1 = true;
			m_arm2 = true;
			m_neck = true;
			m_wheel = true;
			m_face = true;
			m_led = true;
			m_sound = true;

		}

		ModalityUseSet::ModalityUseSet( bool arm1, bool arm2, bool neck, bool wheel, bool face, bool led, bool sound)
			: m_arm1(arm1), m_arm2(arm2), m_neck(neck), m_wheel(wheel), m_face(face), m_led(led), m_sound(sound)
		{

		}

		ModalityUseSet::~ModalityUseSet(void)
		{
		}

		bool ModalityUseSet::getArm1() const
		{
			return m_arm1;
		}

		void ModalityUseSet::setArm1( bool val )
		{
			m_arm1 = val;
		}

		bool ModalityUseSet::getArm2() const
		{
			return m_arm2;
		}

		void ModalityUseSet::setArm2( bool val )
		{
			m_arm2 = val;
		}

		bool ModalityUseSet::getNeck() const
		{
			return m_neck;
		}

		void ModalityUseSet::setNeck( bool val )
		{
			m_neck = val;
		}

		bool ModalityUseSet::getWheel() const
		{
			return m_wheel;
		}

		void ModalityUseSet::setWheel( bool val )
		{
			m_wheel = val;
		}

		bool ModalityUseSet::getFace() const
		{
			return m_face;
		}
		void ModalityUseSet::setFace( bool val )
		{
			m_face = val;
		}

		bool ModalityUseSet::getLed() const
		{
			return m_led;
		}

		void ModalityUseSet::setLed( bool val )
		{
			m_led = val;
		}

		bool ModalityUseSet::getSound() const
		{
			return m_sound;
		}

		void ModalityUseSet::setSound( bool val )
		{
			m_sound = val;
		}

		void ModalityUseSet::copyFrom(const ModalityUseSet& source) {
			m_arm1 = source.m_arm1;
			m_arm2 = source.m_arm2;

			m_neck = source.m_neck;
			m_wheel = source.m_wheel;

			m_face = source.m_face;
			m_led = source.m_led;

			m_sound = source.m_sound;
		}

		void ModalityUseSet::dump() {
			ROS_DEBUG("left_arm_enabled : %d", m_arm1);
			ROS_DEBUG("right_arm_enabled : %d", m_arm2);
			ROS_DEBUG("neck_enabled : %d", m_neck);
			ROS_DEBUG("wheel_enabled : %d", m_wheel);
			ROS_DEBUG("face_enabled : %d", m_face);
			ROS_DEBUG("led_enabled : %d", m_led);
			ROS_DEBUG("sound_enabled : %d", m_sound);
		}


	} /* namespace modality */
} /* namespace behavior */
