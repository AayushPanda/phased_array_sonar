struct pinMask {volatile uint8_t bank; uint8_t mask;};
uint8_t* ports[] = {&PORTB, &PORTC, &PORTD};

void fastWrite(uint8_t pin, uint8_t state);

const pinMask unoPins[] = {
  // {[0,1,2]->[B,C,D], pin address}
  // Consider A0 to be pin 14.. A5 as pin 19

  // D BANK
  {2, B00000001}, // D0 (2)
  {2, B00000010}, // D1 (3)
  {2, B00000100}, // D2 (4)
  {2, B00001000}, // D3 (5)
  {2, B00010000}, // D4 (6)
  {2, B00100000}, // D5 (11)
  {2, B01000000}, // D6 (12)
  {2, B10000000}, // D7 (13)

  // B BANK
  {0, B00000001}, // D8  (14)
  {0, B00000010}, // D9  (15)
  {0, B00000100}, // D10 (16)
  {0, B00001000}, // D11 (17)
  {0, B00010000}, // D12 (18)
  {0, B00100000}, // D13 (19)

  // C BANK
  {1, B00000001}, // A0 (23)
  {1, B00000010}, // A1 (24)
  {1, B00000100}, // A2 (25)
  {1, B00001000}, // A3 (26)
  {1, B00010000}, // A4 (27)
  {1, B00100000}, // A5 (28)
};

void setup() {
  pinMode(2, OUTPUT);
}

void loop() {
  fastWrite(2, 1);
  delayMicroseconds(3);
  fastWrite(2, 0);
  delayMicroseconds(3);
}

void fastWrite(uint8_t pin, uint8_t state){
  uint8_t* port = ports[unoPins[pin].bank];
  uint8_t mask = unoPins[pin].mask;

  if (state) {
    *port |= mask;
  } else {
    *port &= ~mask;
  }
}

