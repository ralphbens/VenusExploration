#ifndef Driving_h
#define Driving_h

// The class interface
class Driving
{
private:
  static Servo leftWheel;
  static Servo rightWheel;

  static volatile int leftPulses;
  static volatile int rightPulses;
  static volatile int rotatePulses;

  static int callbackPulses;
  static void (*callbackFunc)();

public:
  static double relativeXPosition;
  static double relativeYPosition;
  static double relativeOrientation;

  static void initialize();
  static void trigger(byte pin);
  static void rotate(double degree);
  static void drive(int dir);

  static void addCallback(int pulses, void (*callback)());
  static void runCallback();

  static void mapAndReset();
  static void calculateNewPosition(double degreeTurned, double pulsesDriven);
  static double* calculateBaseDirection();
  static void resetPosition();
};

#endif // Driving_h
