#include "Definitions.h"
#ifdef __DEBUG
#include "DebugSerial.h"

// Variable initialization
byte DebugSerial::writePointer;
byte DebugSerial::debugItem;
char DebugSerial::debugStr[16];

// Initialization function
void DebugSerial::open()
{
  // Open Serial communications
  Serial.begin(9600);

  // Print all functions
  Serial.println("Debugging commands");
  Serial.println("- rot [degrees]");
  Serial.println("- drive [direction]");
  Serial.println("- dist [degree]");
  Serial.println("- sweep [0/1/2]");
  Serial.println("- sample [test]");
}

// Serial reading function
void DebugSerial::read()
{
  // Check if we have serial data
  if (Serial.available() > 0)
  {
    handle(Serial.read());
  }
}

// Serial input handler
void DebugSerial::handle(byte code)
{
  // Check for newline
  if (code == '\n')
  {
    // Find the first space
    int space = 0;
    for (int i = 0; i < strlen(debugStr); i++)
    {
      if (debugStr[i] == ' ')
        space = i;
    }
    
    // Get the command
    char buff[space + 1];
    memcpy(buff, debugStr, space);
    buff[space] = '\0';

    // Get the parameters
    float value = getToken(debugStr, space + 1, writePointer);
    
    // Match commands
    if (strcmp(buff, "rot") == 0)
    {
      Driving::rotate(value, NULL);
    }
    else if (strcmp(buff, "drive") == 0)
    {
      Driving::drive(value);
    }
    else if (strcmp(buff, "dist") == 0)
    {
      long dist = UDS::distanceAtDegree(value);

      Serial.print("Measured distance at ");
      Serial.print(value);
      Serial.print(" deg -> ");
      Serial.println(dist);
    }
    else if (strcmp(buff, "sweep") == 0)
    {
      UDS::setSweep((int)value);
    }
    else if (strcmp(buff, "sample") == 0)
    {
      Sample::setupDetectors();

      int enumId = (int)value;
      Serial.print("Sensor value: ");
      Serial.println(Sample::getValue((SensorPos)enumId));
    }
    else
    {
      Serial.print("Unrecognized command: ");
      Serial.println(debugStr);
    }

    // Clear the input
    memset(debugStr, 0, strlen(debugStr));

    // Reset pointer
    writePointer = 0;
  }
  else
  {
    debugStr[writePointer++] = code;
  }
}

// Translate input data
float DebugSerial::getToken(char* data, byte offset, byte ending)
{
  // Set defaults
  float value = 0;
  byte start = offset;
  bool negate = false;

  // Check if it's a negative number
  if (debugStr[start] == '-')
  {
    start++;
    negate = true;
  }

  // Get the powers of each number
  for (int i = start; i < ending; i++)
    value += (debugStr[i] - 48) * pow(10, ending - i - 1);

  // Return correct value
  return negate ? -value : value;
}

#endif // __DEBUG
