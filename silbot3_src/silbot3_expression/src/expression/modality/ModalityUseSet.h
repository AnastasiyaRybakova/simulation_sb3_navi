#ifndef __EXPRESSION_MODALITY_MODALITYUSESET_H__
#define __EXPRESSION_MODALITY_MODALITYUSESET_H__

namespace expression {
	namespace modality {

		class ModalityUseSet {
		public:
			ModalityUseSet(void);
			~ModalityUseSet(void);

			ModalityUseSet(bool arm1, bool arm2, bool neck, bool wheel, bool face, bool led, bool sound);

			bool m_arm1;
			bool getArm1() const;
			void setArm1(bool val);

			bool m_arm2;
			bool getArm2() const;
			void setArm2(bool val);

			bool m_neck;
			bool getNeck() const;
			void setNeck(bool val);

			bool m_wheel;
			bool getWheel() const;
			void setWheel(bool val);

			bool m_face;
			bool getFace() const;
			void setFace(bool val);

			bool m_led;
			bool getLed() const;
			void setLed(bool val);

			bool m_sound;
			bool getSound() const;
			void setSound(bool val);

			void copyFrom(const ModalityUseSet& source);

			void dump();

		};

	} /* namespace modality */
} /* namespace expression */

#endif /* __EXPRESSION_MODALITY_MODALITYUSESET_H__ */
