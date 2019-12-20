#include <WS2812FX.h>

#define LED_COUNT 4
#define LED_PIN 7

#define TIMER_MS 5000

long debouncing_time = 30; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;

unsigned long last_trigger = 0;
unsigned long triggerRate = 10; // rate to triger effect in ms

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

unsigned long last_change = 0;
unsigned long now = 0;

byte fxs[] = {0,1,2,9,10,11,12,15,16,17,19,20,21,22,23,24,25,29,46,49,48,50};
int speeds[] = {};
byte fxI = 10;

void setup() {
  Serial.begin(9600);
  
  ws2812fx.init();
  ws2812fx.setBrightness(128);
  ws2812fx.setSpeed(10000);
  ws2812fx.setColor(BLUE);
  ws2812fx.setMode(fxs[fxI]);
  ws2812fx.start();

  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), debounceInterrupt1, RISING);

  Serial.println(sizeof(fxs));
}

void loop() {
  now = millis();

  ws2812fx.service();

  // Set Dial control to trigger rate
  //triggerRate = map(analogRead(A3),0,1023,5,250);

  // trigger on a regular basis
  if(now - last_trigger > triggerRate) {
    ws2812fx.trigger();
    last_trigger = now;
  }
}

void debounceInterrupt1() {
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    Interrupt1();
    last_micros = micros();
  }
}

void Interrupt1() {
  //next mode
  fxI = ((fxI + 1) % sizeof(fxs));
  ws2812fx.setMode(fxs[fxI]);
  Serial.println(ws2812fx.getMode());
}

