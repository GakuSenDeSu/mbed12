#include "mbed.h"

#define CENTER_BASE 1500

Serial pc(USBTX, USBRX);
DigitalIn encoder(D10);
PwmOut servo(D11);

Timer t;
Timer t1;
Ticker encoder_ticker;

volatile int steps;
volatile int last;

void servo_control(int speed) {
  if (speed > 200)       speed = 200;
  else if (speed < -200) speed = -200;

  servo = (CENTER_BASE + speed) / 20000.0f;
}

void encoder_control() {
  int value = encoder;
  if (!last && value) steps++;
  last = value;
}

int main() {
  pc.baud(9600);
  servo_control(0);
  encoder_ticker.attach(&encoder_control, .001);
  servo.period(.02);
  //speed
  t1.start();
  while (t1.read() <= 5) {
    servo_control(25.138850477515426);

    steps = 0;
    t.reset();
    t.start();

    wait(1);

    float time = t.read();
    pc.printf("%1.3f\r\n", (float)steps * 6.5 * 3.14 / 32 / time);
  }
  servo_control(0);
  t1.reset();
  //counterwise
  t1.start();
  while (t1.read() <= 5) {
    servo_control(-44.46909569791206);

    steps = 0;
    t.reset();
    t.start();

    wait(1);

    float time = t.read();
    pc.printf("%1.3f\r\n", (float)steps * 6.5 * 3.14 / 32 / time);
  }
  servo_control(0);

  while (1);
}