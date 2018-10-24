// for chip info see https://www.analog.com/en/products/ad9833.html


const uint8_t _dataPin  = 11;
const uint8_t _clkPin   = 13;
const uint8_t _fsyncPin = 10;

// send raw 16-bit word
void spiSend(const uint16_t data)
{
  digitalWrite(_fsyncPin, LOW);

  Serial.println(data, HEX);

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

  Serial.print("Set f: ");
  Serial.println(f);

  spiSend(b28);
  spiSend(f_low  | freq);
  spiSend(f_high | freq);
}

void setup() {
  pinMode(_clkPin,   OUTPUT);
  pinMode(_fsyncPin, OUTPUT);
  pinMode(_dataPin,  OUTPUT);

  digitalWrite(_fsyncPin, HIGH);
  digitalWrite(_clkPin,   LOW);
  // put your setup code here, to run once:

  Serial.begin(9600);
}

void loop() {
  setFreq(800.0);
  delay(50);
  setFreq(1600.0);
  delay(50);
 

}

