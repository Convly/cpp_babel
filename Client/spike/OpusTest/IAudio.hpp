//
// Made by marco
//

#ifndef I_AUDIO_HPP_
# define I_AUDIO_HPP_

namespace babel
{
	class IAudio
	{
	public:
		virtual int		initialize(void) = 0;
		virtual int		terminate(void) = 0;

	public:
		virtual int		startStream(bool record = true, bool difuse = true) = 0;
		virtual int		run(void) = 0;
		virtual int		stopStream(void) = 0;

	public:
		virtual float	*getRecord(void) const = 0;
		virtual int		getRecordSize(void) const = 0;
		virtual int		getPlayBackSize(void) const = 0;

	public:
		virtual void	setPlayBack(float *playMe, int size) = 0;

	public:
		virtual void	cleanRecord(void) = 0;
	};
}

#endif