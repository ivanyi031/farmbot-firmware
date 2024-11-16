#ifndef SPI_ENCODER_H
#define SPI_ENCODER_H
#include "pin.h"
#include <SPI.h>


const int spi_encoder_offset = 4;
//extern const int NSS_PIN;  // 使用 extern 声明 NSS_PIN


enum SpiEncoders
{
  _X1 = 0b0001,
  _X2 = 0b0010,
  _Y  = 0b0100,
  _Z  = 0b1000,
};


void ResetEncoder(SpiEncoders encoders);
long ReadEncoder(SpiEncoders encoder);

#endif
