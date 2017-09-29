/*
** textOpusCodec.cpp for PortAudio in /home/marco/Bureau/rendu/PortAudio
**
** Made by Marco
** Login   <marco@epitech.net>
**
** Started on  Thu Sep 28 17:07:30 2017 Marco
** Last update Fri Sep 29 20:06:35 2017 Marco
*/

#include "Audio.hpp"
#include "Encoder.hpp"

void     				decode(OpusDecoder *decoder, EncodedData& encoded, std::vector<float> *);
EncodedData			encode(OpusEncoder *encoder, std::vector<float> encryptMe);

int					recordMeEncode()
{
	babel::Audio	audio;
	int				i = 0;
	FILE			*fd;
	EncodedData	encodeData;
	float			*buff;
	int													err;

	OpusEncoder *encoder = opus_encoder_create(SAMPLE_RATE, NUM_CHANNELS, OPUS_APPLICATION_VOIP, &err);
	if (err < 0)
		std::cerr << "error encoder create: " << opus_strerror(err) << std::endl;
	opus_encoder_ctl(encoder, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));


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
			std::vector<float>	buffer;
			int it;
			for (it = 0; it < audio.getRecordSize(); it++)
				buffer.push_back(buff[it]);
			encodeData = encode(encoder, buffer);

			if (fd != NULL)
			{
				std::cout << encodeData.lenght << " ";
				fwrite(&encodeData, sizeof(EncodedData), 1, fd);
			}
			i++;
		}
		std::cout << "\n=========== STOP ============\n" << std::endl;
		if (fd != NULL)
			fclose(fd);
		audio.stopStream();
	}
	audio.terminate();
	opus_encoder_destroy(encoder);
	return (0);
}

int						playBackMeDecode()
{
	babel::Audio	audio;
	bool			run = true;
	FILE			*fd;
	int				read = 0;
	int				size = 0;
	int				err;
	OpusDecoder *decoder;
	EncodedData	data;
	std::vector<float>	buff;

	decoder = opus_decoder_create(SAMPLE_RATE, NUM_CHANNELS, &err);
	if (err < 0)
		std::cout << "error decoder create: " << opus_strerror(err) << '\n';


	if (audio.initialize())
		return (1);
	if (audio.startStream(false, true) == 0)
	{
		std::cout << "\n====== STARTING PlayBACK ======\n" << std::endl;
		fd = fopen("recorded.raw", "rb");
		while (run)
		{
			read = fread(&data, sizeof(EncodedData), 1, fd);
//			std::cout << "read = " << read << '\n';
			if (read <= 0)
				run = false;
			else
			{
				std::cout << data.lenght << " ";
				buff.clear();
				decode(decoder, data, &buff);
				audio.setPlayBack(buff.data(), buff.size());
				audio.run();
				if (read <= 0)
					run = false;
			}
		}
		std::cout << "\n=========== STOP ============\n" << std::endl;
		if (fd != NULL)
			fclose(fd);
		audio.stopStream();
	}
	audio.terminate();

std::cout << "a" << '\n';
	opus_decoder_destroy(decoder);
std::cout << "b" << '\n';
	return (0);
}
