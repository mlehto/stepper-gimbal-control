#include <AccelStepper.h>

AccelStepper stepperH(1, 13, 12);
AccelStepper stepperV(1, 11, 10);

// Define our maximum and minimum speed in steps per second (scale pot to these)
#define  MAX_SPEED 2000
#define  MIN_SPEED 0.1
int h_deadband_low = 510;
int h_deadband_high = 517;
int v_deadband_low = 515;
int v_deadband_high = 525;

void setup() {
  stepperH.setMaxSpeed(MAX_SPEED);
  stepperH.setAcceleration(400);
  stepperV.setMaxSpeed(MAX_SPEED);
  stepperV.setAcceleration(400);
}

void loop() {
  static int analog_read_counter = 1000;    // Counts down to 0 to fire analog read
  static float h_required_speed = 0.0;
  static float h_current_speed = 0.0;
  static char h_required_direction = 0;
  static char h_current_direction = 0; 
  static int h_joystic_value = 0;
  static float v_required_speed = 0.0;
  static float v_current_speed = 0.0;
  static char v_required_direction = 0;
  static char v_current_direction = 0; 
  static int v_joystic_value = 0;
  
  if (analog_read_counter > 0) {
    analog_read_counter--;
  }
  else {
    analog_read_counter = 3000;
    h_joystic_value = analogRead(A0);
    v_joystic_value = analogRead(A1);
    
    //We are in deadband
    if (h_joystic_value > h_deadband_low && h_joystic_value < h_deadband_high) {
      h_required_speed = 0;
      h_required_direction = 0;
    }
    stepperH.runSpeed();
    if (v_joystic_value > v_deadband_low && v_joystic_value < v_deadband_high) {
      v_required_speed = 0;
      v_required_direction = 0;
    }
    stepperV.runSpeed();

    //Low end
    if (h_joystic_value < h_deadband_low) {
      h_required_speed = map(h_joystic_value, h_deadband_low, 0, MIN_SPEED, MAX_SPEED);
      h_required_direction = -1;
    }
    if (v_joystic_value < v_deadband_low) {
      v_required_speed = map(v_joystic_value, v_deadband_low, 0, MIN_SPEED, MAX_SPEED);
      v_required_direction = -1;
    }
    stepperH.runSpeed();
    
    //High end
    if (h_joystic_value > h_deadband_high) {
      h_required_speed = map(h_joystic_value, h_deadband_high, 1023, MIN_SPEED, MAX_SPEED);
      h_required_direction = 1;
    }
    if (v_joystic_value > v_deadband_high) {
      v_required_speed = map(v_joystic_value, v_deadband_high, 1023, MIN_SPEED, MAX_SPEED);
      v_required_direction = 1;
    }
    stepperV.runSpeed();
    
    //Changing directions if needed
    if (h_required_direction != h_current_direction) {
      if (h_current_speed < 10) {
        h_current_direction = h_required_direction;
      }
      else {
        h_required_speed = 0;
      }
    }
    stepperH.runSpeed();
    if (v_required_direction != v_current_direction) {
      if (v_current_speed < 10) {
        v_current_direction = v_required_direction;
      }
      else {
        v_required_speed = 0;
      }
    }
    stepperV.runSpeed();
    
    //Accelerating
    if (h_current_speed < h_required_speed) {
      h_current_speed = h_current_speed + min(100, h_required_speed - h_current_speed);
    }
    stepperH.runSpeed();
    if (v_current_speed < v_required_speed) {
      v_current_speed = v_current_speed + min(100, v_required_speed - v_current_speed);
    }
    stepperV.runSpeed();
    
    //Decelerating
    if (h_current_speed > h_required_speed) {
      h_current_speed = h_current_speed - min(200, h_current_speed - h_required_speed);
    }
    stepperH.runSpeed();
    if (v_current_speed > v_required_speed) {
      v_current_speed = v_current_speed - min(200, v_current_speed - v_required_speed);
    }
    stepperV.runSpeed();

    stepperH.setSpeed(h_current_direction * h_current_speed);
    stepperV.setSpeed(v_current_direction * v_current_speed);
  }

  stepperH.runSpeed();
  stepperV.runSpeed();
}
