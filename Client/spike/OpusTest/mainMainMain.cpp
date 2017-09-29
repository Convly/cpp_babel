/*
** mainMainMain.cpp for PortAudio in /home/marco/Bureau/rendu/PortAudio
**
** Made by Marco
** Login   <marco@epitech.net>
**
** Started on  Fri Sep 29 16:11:10 2017 Marco
** Last update Fri Sep 29 23:31:38 2017 Marco
*/

#include "Encoder.hpp"
#include "Audio.hpp"

EncodedData                   encode(OpusEncoder *encoder, std::vector<float> encryptMe);

void    decode(OpusDecoder *decoder, EncodedData& encoded, std::vector<float> *decoded);

int recordMeEncode();
int playBackMeDecode();
int recordMeNow();
int playBackMeNow();

class Codec
{
private:
  OpusEncoder *_encoder;
  OpusDecoder *_decoder;
  int         _err;

public:
  explicit Codec (void)
  {
    this->_encoder = opus_encoder_create(SAMPLE_RATE, NUM_CHANNELS, OPUS_APPLICATION_VOIP, &this->_err);
    if (this->_err < 0)
      std::cout << "error encoder create: " << opus_strerror(this->_err) << std::endl;
    this->_decoder = opus_decoder_create(SAMPLE_RATE, NUM_CHANNELS, &this->_err);
    if (this->_err < 0)
        std::cout << "error decoder create: " << opus_strerror(this->_err) << std::endl;
    opus_encoder_ctl(this->_encoder, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
  }

  virtual ~Codec (void)
  {
    opus_encoder_destroy(this->_encoder);
    opus_decoder_destroy(this->_decoder);
  }

public:
  virtual EncodedData const   Encode(std::vector<float> const &encryptMe)
  {
    EncodedData               back;

    back.data.resize(MAX_PACKET);
    if ((back.lenght = opus_encode_float(this->_encoder, encryptMe.data(), encryptMe.size() / NUM_CHANNELS, back.data.data(), MAX_PACKET)) < 0)
      {
        std::cerr << "Error: " << opus_strerror(back.lenght) << std::endl;
        back.lenght = 0;
      }
    return (back);
  }

  virtual void                Decode(EncodedData decryptMe, std::vector<float> &sample) const
  {
    int                       frameSize;

    sample.resize(MAX_FRAME_SIZE * NUM_CHANNELS);
    if ((frameSize = opus_decode_float(this->_decoder, decryptMe.data.data(), decryptMe.lenght, sample.data(), MAX_FRAME_SIZE, 0)) < 0)
      std::cerr << "Error: " << frameSize << std::endl;
    else
      sample.resize(frameSize * NUM_CHANNELS);
  }
};

int main()
{
  bool          run = true;
  babel::Audio  audio;
  Codec         cot;
  int				    i = 0;
  float			    *buff = nullptr;
  std::vector<float>  in;
  std::vector<float>  out;
  int						err;
  int           it;

  if (audio.initialize())
    return (1);
  if (audio.startStream(true, true) == 0)
  {
    while (run)
    {
      if (buff != nullptr)
      {
        out.clear();
        cot.Decode(cot.Encode(in), out);
        audio.setPlayBack(out.data(), out.size());
      }
      audio.run();
      buff = nullptr;
      buff = audio.getRecord();
      in.clear();
      for (int i = 0; i <audio.getRecordSize(); ++i)
        in.push_back(buff[i]);
    }
    audio.stopStream();
  }
  audio.terminate();
  return (0);
}
