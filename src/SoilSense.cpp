/*
 * SoilSense.cpp
 */
#include <Arduino.h>
#include "SoilSense.h"

SoilSense::SoilSense(uint8_t pin) {
  _pin = pin;
  _value = 0;
  pinMode(pin, INPUT_PULLUP);
}

int SoilSense::init(int min, int max, byte lowpass) {
  _min = convert(min);
  _max = convert(max);
  _lowpass = lowpass;
}

int SoilSense::measure(int samples) {
  int val = 1023 * samples;
  for (size_t i = 0; i < samples; i++) {
    val -= analogRead(_pin);
  }
  return val / 10;
}

byte SoilSense::value() {
  long rawVal = convert(measure());
  rawVal -= _min;
  if(rawVal < 0) rawVal = 0;
  rawVal = rawVal * 100 / (_max - _min);
  byte val = 100;
  if(rawVal < 100) val = (byte)rawVal;
  val = val * (100 - _lowpass) / 100 + val * _lowpass / 100;
  return val;
}

long SoilSense::convert(int value) {
  return 0x100000 - 0x100000 / ((long)value + 1);
}
