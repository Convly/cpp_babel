/*
** IEncoder.hpp for PortAudio in /home/marco/Bureau/rendu/PortAudio
**
** Made by Marco
** Login   <marco@epitech.net>
**
** Started on  Thu Sep 28 16:46:46 2017 Marco
** Last update Fri Sep 29 21:23:56 2017 Marco
*/

#ifndef I_ENCODER_HPP
# define I_ENCODER_HPP

#include <vector>

struct                        EncodedData
{
  opus_int32                  lenght;
  std::vector<unsigned char>  data;

  EncodedData(){};
  EncodedData(opus_int32 l, std::vector<unsigned char> &d): lenght(l), data(d){};
  EncodedData &operator=(const EncodedData &other)
  {
    if (this != &other)
    {
      this->lenght = other.lenght;
      this->data.clear();
      for (int i = 0; i < other.data.size(); ++i)
        this->data.push_back(other.data.at(i));
    }
  }
};

class ICodec
{
public:
  virtual EncodedData const   &Encode(std::vector<float> const &) = 0;
  virtual void                Decode(EncodedData&, std::vector<float> &) const = 0;
};

#endif
