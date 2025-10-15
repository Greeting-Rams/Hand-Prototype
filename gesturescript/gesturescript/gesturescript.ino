#include <Servo.h>

// ==== Pin mapping (change to match your wiring) ====
const uint8_t PIN_THUMB = 6;
const uint8_t PIN_INDEX = 5;
const uint8_t PIN_MIDDLE = 4;
const uint8_t PIN_RING  = 3;
const uint8_t PIN_PINKY = 2;

// If some servos are reversed mechanically, set these to true
bool REVERSE_THUMB = false;
bool REVERSE_INDEX = false;
bool REVERSE_MIDDLE= true;
bool REVERSE_RING  = false;
bool REVERSE_PINKY = true;

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

// === Preset gestures (t, i, m, r, p) ===
// Angles are examples; adjust to fit your mechanics:
// 0 = fully extended, 180 = fully curled (if not reversed)
void gestureOpen()        { setHand(  0,   0,   0,   0,   0, 3, 10); }
void gestureFist()        { setHand(180, 180, 180, 180, 180, 3, 12); }
void gesturePeace()       { setHand(170,   0,   0, 170, 170, 3, 12); }
void gestureThumbsUp()    { setHand(  0, 170, 170, 170, 170, 3, 12); }
void gestureWaveOpen()    { setHand(  0,  20,  20,  20,  20, 2, 10); }
void gestureWaveClosed()  { setHand(  0, 120, 120, 120, 120, 2, 10); }

void wave(int cycles=3) {
  for (int k=0; k<cycles; ++k) {
    gestureWaveOpen();
    delay(160);
    gestureWaveClosed();
    delay(160);
  }
}

void setup() {
  Serial.begin(9600);
  // Attach; if your servos need different pulse range, use attach(pin, minUs, maxUs)
  sThumb.attach(PIN_THUMB);
  sIndex.attach(PIN_INDEX);
  sMiddle.attach(PIN_MIDDLE);
  sRing.attach(PIN_RING);
  sPinky.attach(PIN_PINKY);

  // Start neutral
  writeServosRaw(curThumb, curIndex, curMiddle, curRing, curPinky);
  delay(10);
}
//Updated 10/15, hasn't been tested.
void loop() {
  Serial.println("Open");
  gestureOpen();
  delay(400);
  Serial.println("Fist");
  gestureFist();
  delay(500);
  /*
  Serial.println("Open");
  gestureOpen();
  delay(300);*/
  Serial.println("Peace");
  gesturePeace();
  delay(700);
  /*
  Serial.println("Open");
  gestureOpen();
  delay(300);
  
  Serial.println("ThumbsUp");
  gestureThumbsUp();
  delay(700);
  Serial.println("Wave");
  wave(4);
  delay(600);*/
  Serial.println("done");

}
