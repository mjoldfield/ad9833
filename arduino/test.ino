/*
  Toy code for simple control of the AD9833 DDS chip

  Cut down heavily from https://github.com/MajicDesigns/MD_AD9833
    - The SPI code is lifted almost verbatim

  Copyright (C) 2018 M J Oldfield
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


// for chip info see https://www.analog.com/en/products/ad9833.html

const uint8_t _dataPin  = 11;
const uint8_t _clkPin   = 13;
const uint8_t _fsyncPin = 10;

// send raw 16-bit word
void spiSend(const uint16_t data)
{
  digitalWrite(_fsyncPin, LOW);

  uint16_t m = 1UL << 15;
  for (uint8_t i = 0; i < 16; i++)
    {
      digitalWrite(_dataPin, data & m ? HIGH : LOW);
      digitalWrite(_clkPin, LOW); //data is valid on falling edge
      digitalWrite(_clkPin, HIGH);
      m >>= 1;
    }

  digitalWrite(_dataPin, LOW); //idle low
  digitalWrite(_fsyncPin, HIGH);
}

void setFreq(double f)
{ 
  const uint16_t b28  = (1UL << 13);
  const uint16_t freq = (1UL << 14);
  
  const double   f_clk = 25e6;
  const double   scale = 1UL << 28;
  const uint32_t n_reg = f * scale / f_clk;

  const uint16_t f_low  = n_reg         & 0x3fffUL;
  const uint16_t f_high = (n_reg >> 14) & 0x3fffUL; 

  spiSend(b28);
  spiSend(f_low  | freq);
  spiSend(f_high | freq);
}

void setup()
{
  pinMode(_clkPin,   OUTPUT);
  pinMode(_fsyncPin, OUTPUT);
  pinMode(_dataPin,  OUTPUT);

  digitalWrite(_fsyncPin, HIGH);
  digitalWrite(_clkPin,   LOW);
}

void loop()
{
  setFreq(800.0);
  delay(50);
  setFreq(1600.0);
  delay(50);
}

