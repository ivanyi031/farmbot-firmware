#include "SpiEncoder.h"



void ResetEncoder(SpiEncoders encoders)
{
  const byte reset_cmd = 0x00;

  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
  digitalWrite(NSS_PIN, LOW);
  delayMicroseconds(2);
  SPI.transfer(reset_cmd | (encoders << spi_encoder_offset));
  delayMicroseconds(5);
  digitalWrite(NSS_PIN, HIGH);
  SPI.endTransaction();
}

long ReadEncoder(SpiEncoders encoder)
{
  const byte read_cmd  = 0x0F;
  int readSize = 4;
  long encoderVal = 0;

  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
  digitalWrite(NSS_PIN, LOW);
  delayMicroseconds(2);
  SPI.transfer(read_cmd | (encoder << spi_encoder_offset));
  delayMicroseconds(5);

  for (int i = 0; i < readSize; ++i)
  {
    encoderVal <<= 8;
    encoderVal |= SPI.transfer(0x01);
  }
  
  digitalWrite(NSS_PIN, HIGH);
  SPI.endTransaction();
  return encoderVal;
}
