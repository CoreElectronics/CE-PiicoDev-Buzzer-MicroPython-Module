#include <stdint.h>
#define NUM(a) (sizeof(a) / sizeof(*a))

const uint16_t buzzerPin = PIN_PA3;

const uint16_t ScaleCmaj[] = {262, 294, 330, 349, 392, 440, 493, 523};

void setup() {
  // put your setup code here, to run once:
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < NUM(ScaleCmaj) ; i++) {
    tone(buzzerPin, ScaleCmaj[i], 400);
    delay(500);  
  }
  
  
}
