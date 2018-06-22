#ifndef __EXPRESSION_STATUS_H__
#define __EXPRESSION_STATUS_H__

namespace expression {
namespace status {

	enum StatusType {
		// INIT_STATE = 1,
		// EXPRESSION_WITH_SENTENCE_STATE,
		// EXPRESSION_ONLY_LIP_STATE,
		// SONG_STATE,
		// SPEAK_FINISH_STATE,
		// MOTION_FINISH_STATE
		EXPRESSION_STATUS_INIT,
		EXPRESSION_STATUS_WITH_MOTION,
		EXPRESSION_STATUS_WITHOUT_MOTION,
		EXPRESSION_STATUS_WITH_SONG,
		EXPRESSION_STATUS_WAIT_FOR_MOTION_FINISH,
		EXPRESSION_STATUS_WAIT_FOR_SOUND_FINISH
	} ;

	class Status {

	public:
		Status();
		virtual ~Status();

		const StatusType& getStatus() const;
	};

} /* namespace status */
} /* namespace expression */

#endif /* __EXPRESSION_STATUS_H__ */
