//
// Made by Marco
//

#include "Audio.hpp"

int					recordMeNow()
{
	babel::Audio	audio;
	int				i = 0;
	FILE			*fd;
	float			*buff;

	if (audio.initialize())
		return (1);
	if (audio.startStream(true, false) == 0)
	{
		std::cout << "\n====== STARTING STREAM ======\n" << std::endl;
		fd = fopen("recorded.raw", "wb");
		while (i < 160)
		{
			audio.run();
			buff = audio.getRecord();
			if (fd != NULL)
				std::cout << fwrite(buff, sizeof(float), audio.getRecordSize(), fd) << std::endl;
			i++;
		}
		std::cout << "\n=========== STOP ============\n" << std::endl;
		if (fd != NULL)
			fclose(fd);
		audio.stopStream();
	}
	audio.terminate();
	return (0);
}

int						playBackMeNow()
{
	babel::Audio	audio;
	bool			run = true;
	FILE			*fd;
	float			*buff;
	int				read = 0;

	if ((buff = new float[NUM_SECONDS * SAMPLE_RATE]) == NULL)
		return (1);

	if (audio.initialize())
		return (1);
	if (audio.startStream(false, true) == 0)
	{
		std::cout << "\n====== STARTING PlayBACK ======\n" << std::endl;
		fd = fopen("recorded.raw", "rb");
		while (run)
		{
			read = fread(buff, sizeof(float), FRAME_SIZE, fd);
			audio.setPlayBack(buff, FRAME_SIZE);
			audio.run();
			if (read <= 0)
				run = false;
		}
		std::cout << "\n=========== STOP ============\n" << std::endl;
		if (fd != NULL)
			fclose(fd);
		audio.stopStream();
	}
	audio.terminate();
	return (0);
}
