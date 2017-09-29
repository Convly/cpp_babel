//
// Made by Marco
//

#include <iostream>
#include "Encoder.hpp"
#include "Audio.hpp"

EncodedData                   encode(OpusEncoder *encoder, std::vector<float> encryptMe)
{
  EncodedData                 encoded;

  encoded.data.resize(MAX_PACKET);
  if ((encoded.lenght = opus_encode_float(encoder, encryptMe.data(), encryptMe.size() / NUM_CHANNELS, encoded.data.data(), MAX_PACKET)) < 0)
    std::cout << "error encode: " << encoded.lenght << " " << opus_strerror(encoded.lenght) << '\n';
  std::cout << "{" << encryptMe.size() << "}";
  return (encoded);
}

void    decode(OpusDecoder *decoder, EncodedData& encoded, std::vector<float> *decoded)
{
  int   frameSize;

  decoded->resize(MAX_FRAME_SIZE * NUM_CHANNELS);
  if ((frameSize = opus_decode_float(decoder, encoded.data.data(), encoded.lenght, decoded->data(), MAX_FRAME_SIZE, 0)) < 0)
    std::cout << "error decode: " << frameSize << " " << opus_strerror(frameSize) << '\n';
  std::cout << "{" << frameSize << "}";
  decoded->resize(frameSize * NUM_CHANNELS);
}

int recordMeEncode();
int playBackMeDecode();
int recordMeNow();
int playBackMeNow();

/*
int main(void)
{
//	recordMeEncode();
//	playBackMeDecode();
  recordMeNow();
	playBackMeNow();
	return (0);
}
*/
