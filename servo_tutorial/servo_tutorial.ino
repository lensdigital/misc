/* =============================================================
--- Advanced Servo Tutorial by lensdigital.com ---
This sketch uses standard Arduino library
It shows how to use non-blocking functions to control servos
and adjust servo rotation speed.
It also uses Random to randomise servo movements
 =============================================================== */
#include <Servo.h>

#define MAX_SERVO_NUM 2  // Current number of servos
#define PAN_SERVO_PIN 4  // Arduino Pin connected to servo
#define TILT_SERVO_PIN 5 // Arduino Pin connected to servo

#define PAN_MIN 10   // Minimum position (in degrees)
#define PAN_MAX 170  // Minimum position (in degrees)
#define TILT_MIN 10  // Minimum position (in degrees)
#define TILT_MAX 170 // Maximum position (in degrees)
#define INIT_POS 90  // Initial servo position (in degrees)

#define PAN_SERVO 0 // Gives meaningful name to array index element
#define TILT_SERVO 1 // Gives meaningful name to array index element


bool activeServo[MAX_SERVO_NUM]; // Keeps tracking of servoes that need to change
int currDeg[MAX_SERVO_NUM]; // Keep track of current degree locaiton for each servo
int targetDeg[MAX_SERVO_NUM]; //Target rotation degree fore each servo
int servoLimit[MAX_SERVO_NUM][2]={ {PAN_MIN, PAN_MAX}, {TILT_MIN,TILT_MAX},}; // Servos Max Min limits
volatile unsigned long lastUpdate = 0; // Keeps time of last time servo moved
volatile unsigned long lastRandomMove = 0; // Keeps time of last time servo moved randomly
int servoSpeed=0; // Controls how frequently servo function runs, thus controlling speed of servo (ms)
int timeoutB4NextMove = 1000; // Wait between moving servos again (ms)

Servo myservo[MAX_SERVO_NUM];  // create servo object array to control a servos

void setup() {
  Serial.begin(115200);\
  // Initialize servos
  myservo[PAN_SERVO].attach(PAN_SERVO_PIN);  // attaches the servo on pin 9 to the servo object
  myservo[TILT_SERVO].attach(TILT_SERVO_PIN);  // attaches the servo on pin 9 to the servo object
  // Initialize servos and variable arrays
  for (int i=0; i < MAX_SERVO_NUM; i++) { 
    activeServo[i] = false; // Fill array with "deactivate" values
    currDeg[i] = INIT_POS; // Fill array with initial positoin
    targetDeg[i] = INIT_POS; // Fill array with initial positoin
    myservo[i].write(currDeg[i]); // Rotate servos to initial position
    delay (50);
  }
  Serial.println ("Servos Initialized");
  delay (2000);
}

void loop() {
  updateServo(PAN_SERVO);
  updateServo(TILT_SERVO);
  randomMove(TILT_SERVO,45,120);
  randomMove(PAN_SERVO,45,120);
}

// =========================================================
// --- Sets servos to random position between Min and Max
// =========================================================
void randomMove(int servoID, int mMin, int mMax) {
  if (activeServo[servoID]) return; // Servo is currently updating
  if ((unsigned long)millis()-lastRandomMove > timeoutB4NextMove ) { // It's time to execute
    lastRandomMove=millis(); // Remember the time we executed
    int rPos = random (mMin,mMax); // Generate random number in specified range
    Serial.println (rPos); // Debug
    moveServo (servoID, rPos, 10); // Enable servo movement
  }
}
// =========================================================
// --- Enabes movement of servo to target degree and speed 
// ========================================================
void moveServo (char servoID, int newTarget, int newSpeed) {
  if (activeServo[servoID]) return; // Servo is currently updating
  activeServo[servoID]=true;
  if (newTarget < servoLimit[servoID][0]) targetDeg[servoID]=servoLimit[servoID][0]; // Servo taraget is below MIN, override
  else
  if (newTarget > servoLimit[servoID][1]) targetDeg[servoID]=servoLimit[servoID][1]; // Servo Target is above MAX, override
  else  targetDeg[servoID]=newTarget; // Servo Target is within limits. No need to override
  servoSpeed = newSpeed;
}
// ===============================================================================
// --- Moves servos when needed. Constantly executes via main loop. Non blocking
// ===============================================================================
void updateServo(char servoID) {
  if (!activeServo[servoID]) return; // No servos need updating, exiting.
  if (currDeg[servoID]==targetDeg[servoID]) { // Reached destination
    activeServo[servoID]=false;
    Serial.println ("Reached destination");
    // You can perform some action here (i.e. call a function to take pictures, fire laser, etc.)
    return;
  } // MAIN PART
  if ((unsigned long)millis()-lastUpdate > servoSpeed ) {// It's time to execute
    lastUpdate=millis();
    myservo[servoID].write(currDeg[servoID]);// Move servo to current location
    if (currDeg[servoID] > targetDeg[servoID]) 
      currDeg[servoID]--;    // Rotate CW (decrement position)
    else currDeg[servoID]++; // Rotate CCW (increment position)
  }
}





