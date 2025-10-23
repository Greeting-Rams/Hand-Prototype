#include <Servo.h>
#include <AUnit.h>
#include <ArduinoUnit.h>


// ==== Pin mapping (change to match your wiring) ====
const uint8_t PIN_THUMB = 6;
const uint8_t PIN_INDEX = 5;
const uint8_t PIN_MIDDLE = 4;
const uint8_t PIN_RING  = 3;
const uint8_t PIN_PINKY = 2;

// If some servos are reversed mechanically, set these to true
bool REVERSE_THUMB = false;
bool REVERSE_INDEX = false;
bool REVERSE_MIDDLE= false;
bool REVERSE_RING  = false;
bool REVERSE_PINKY = false;

// Optional per-servo trim (mechanical neutral offset in degrees)
int8_t TRIM_THUMB = 0;
int8_t TRIM_INDEX = 0;
int8_t TRIM_MIDDLE= 0;
int8_t TRIM_RING  = 0;
int8_t TRIM_PINKY = 0;

Servo sThumb, sIndex, sMiddle, sRing, sPinky;

// Current positions we track in software
int curThumb=90, curIndex=90, curMiddle=90, curRing=90, curPinky=90;

// --- helpers ---
int applyReverseAndTrim(int deg, bool reverse, int8_t trim) {
  deg = constrain(deg + trim, 0, 180);
  return reverse ? (180 - deg) : deg;
}

void writeServosRaw(int t, int i, int m, int r, int p) {
  sThumb.write(applyReverseAndTrim(t, REVERSE_THUMB, TRIM_THUMB));
  sIndex.write(applyReverseAndTrim(i, REVERSE_INDEX, TRIM_INDEX));
  sMiddle.write(applyReverseAndTrim(m, REVERSE_MIDDLE, TRIM_MIDDLE));
  sRing .write(applyReverseAndTrim(r, REVERSE_RING,  TRIM_RING));
  sPinky.write(applyReverseAndTrim(p, REVERSE_PINKY, TRIM_PINKY));
}

// Smoothly move all fingers to target angles
void setHand(int t, int i, int m, int r, int p, int step=2, int msPerStep=12) {
  t = constrain(t,0,180); i=constrain(i,0,180); m=constrain(m,0,180);
  r = constrain(r,0,180); p=constrain(p,0,180);

  // Move in small steps so they arrive together
  bool moving = true;
  while (moving) {
    moving = false;

    if (curThumb < t) { curThumb = min(curThumb+step, t); moving = true; }
    else if (curThumb > t){ curThumb = max(curThumb-step, t); moving = true; }

    if (curIndex < i) { curIndex = min(curIndex+step, i); moving = true; }
    else if (curIndex > i){ curIndex = max(curIndex-step, i); moving = true; }

    if (curMiddle < m) { curMiddle = min(curMiddle+step, m); moving = true; }
    else if (curMiddle > m){ curMiddle = max(curMiddle-step, m); moving = true; }

    if (curRing < r) { curRing = min(curRing+step, r); moving = true; }
    else if (curRing > r){ curRing = max(curRing-step, r); moving = true; }

    if (curPinky < p) { curPinky = min(curPinky+step, p); moving = true; }
    else if (curPinky > p){ curPinky = max(curPinky-step, p); moving = true; }

    writeServosRaw(curThumb, curIndex, curMiddle, curRing, curPinky);
    delay(msPerStep);
  }
}

//r, p, m, i, t
void Open(){setHand(0,70,30,80,0,3,10);}
void Fist(){setHand(180, 180, 180, 0, 180, 3, 12);}
void Peace()       { setHand(180,180,30,80,180, 3, 12); }
void ThumbsUp()    { setHand(180,180,180,0,0,3,12); }
void WaveOpen()    { setHand(0,90,50,100,20,2,10); }
void WaveClosed()  { setHand(120,140,120,0,0,2,10); }

void test_open(){
  int pinky = 70;
  int ring = 0;
  int mid = 30;
  int index = 80;
  int thumb = 0;
  int openstuff[] ={0,70,30,80,0};
  //pinky
  assertEqual(70, openstuff[1]);
}
void test_peace(){
  int pinky = 180;
  int ring = 180;
  int mid = 30;
  int index = 80;
  int thumb = 180;
  int openstuff[] ={180,180,30,80,180};
  //pinky
  assertEqual(70, openstuff[1]);
}
void test_fist(){
  int pinky = 180;
  int ring = 180;
  int mid = 180;
  int index = 0;
  int thumb = 180;
  int openstuff[] ={180, 180, 180, 0, 180};
  //pinky
  assertEqual(180, openstuff[1]);
}


void wave(int cycles=3) {
  for (int k=0; k<cycles; ++k) {
    WaveOpen();
    delay(160);
    WaveClosed();
    delay(160);
  }
}

void setup() {
  Serial.begin(9600);
  sThumb.attach(PIN_THUMB);
  sIndex.attach(PIN_INDEX);
  sMiddle.attach(PIN_MIDDLE);
  sRing.attach(PIN_RING);
  sPinky.attach(PIN_PINKY);
  writeServosRaw(curThumb, curIndex, curMiddle, curRing, curPinky);
  delay(10);
 // aunit::TestRunner::begin();
}


void loop() {
  aunit::TestRunner::run();

}
