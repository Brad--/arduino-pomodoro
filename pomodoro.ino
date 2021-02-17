/*
  Simple Pomodoro clock controller
*/
#include <arduino-timer.h>
#include "song.h"

int switchState = 0;
bool alarmOn = false;

// off, work, break
String stage = "off";

auto timer = timer_create_default();

const int WORK_DURATION_MINUTES  = 45;
const int BREAK_DURATION_MINUTES = 10;

void ringAlarm() {
  playSong();
}

void handleSwitch() {
  // Handle switch based on timer stage
  if (stage == "off") {
    startWorkTimer();
  } else if (stage == "work") {
    startBreakTimer();
  } else {
    stopTimer();
  }
  
  // Disable alarm if switch is pressed while alarm is on
  if (alarmOn) {
    alarmOn = false;
  }
  
  delay(1000);
}

void setLEDs(bool red, bool green) {
  writeLED(4, red);
  writeLED(5, green);
}

void writeLED(int pin, bool on) {
  if (on) {
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }
}

void stopTimer() {
  if (timer.size() > 0) {
    timer.cancel();
  }
  setLEDs(false, false);
  stage = "off";
}

void startWorkTimer() {
  startTimer(WORK_DURATION_MINUTES);
  setLEDs(false, true);
  stage = "work";
}

void startBreakTimer() {
  startTimer(BREAK_DURATION_MINUTES);
  setLEDs(true, false);
  stage = "break";
}

void triggerAlarm() {
  resetSong();
  alarmOn = true;
  return false; // stop Timer task
}

void startTimer(int minutes) {
  timer.in(minutes * 60 * 1000UL, triggerAlarm);
}

void setup() {
  // Switch
  pinMode(2, INPUT);
  
  // Red LED
  pinMode(4, OUTPUT);
  // Green LED
  pinMode(5, OUTPUT);
}

void loop() {
  timer.tick();
  
  if (alarmOn) {
    ringAlarm();
  }
  
  // switch is pressed
  switchState = digitalRead(2);
  if (switchState == HIGH) {
    handleSwitch();
  }
}
