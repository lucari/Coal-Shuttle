
// Define  connections

#define forwardLEDPin 2
#define reverseLEDPin 3

#define reedTestPin 4
#define tiltTestPin 5
#define forwardPin 6
#define reversePin 7

#define reedSwitchPin 8
#define tiltSwitchPin 9

#define conveyorPowerPin 10
#define trackPowerPin 11
#define trackDirectionPin 12

#define slowAnalog A0
#define fillAnalog A1

#define ANALOG_TIME 10000

#define SHUTTLE_IDLE 0
#define SHUTTLE_REVERSE 1
#define SHUTTLE_FORWARD 2
#define SHUTTLE_POSITION_TIME 3
#define SHUTTLE_FILL_WAGON 4
#define SHUTTLE_FILL_TIME 5


#define CONVEYOR_OFF 0
#define CONVEYOR_ON 1

int mode = SHUTTLE_IDLE;
int lastSwitch = SHUTTLE_IDLE;
int switchPos;

// Delay times (read from analog ports)
int slowTime = 500;
int fillTime = 1000;

bool lastLED = false;

unsigned long flashTimer;
unsigned long analogTimer = 0L;

int conveyor = CONVEYOR_OFF;

void setup() 
{
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  delay(100);

  Serial.println("*** Coal Train Shuttle V1 ***");   

  // Set the relay outputs high first so they don't false trigger the relays
  digitalWrite(conveyorPowerPin, HIGH);
  digitalWrite(trackPowerPin, HIGH);
  digitalWrite(trackDirectionPin, HIGH);

  // Then set them as outputs
  pinMode(conveyorPowerPin, OUTPUT);
  pinMode(trackPowerPin, OUTPUT);
  pinMode(trackDirectionPin, OUTPUT);

  // Set the LEDs
  pinMode(forwardLEDPin, OUTPUT);
  pinMode(reverseLEDPin, OUTPUT);

  // and clear the LEDs
  digitalWrite(forwardLEDPin, LOW);
  digitalWrite(reverseLEDPin, LOW);

  // finally, set the other pins as inputs with pullups
  pinMode(reedSwitchPin, INPUT_PULLUP);
  pinMode(tiltSwitchPin, INPUT_PULLUP);
  pinMode(reedTestPin, INPUT_PULLUP);
  pinMode(tiltTestPin, INPUT_PULLUP);
  pinMode(forwardPin, INPUT_PULLUP);
  pinMode(reversePin, INPUT_PULLUP);

}


void loop()
{
  // Get Switch settings
  if (digitalRead(forwardPin) == LOW)
  {
    switchPos = SHUTTLE_FORWARD;
  }
  else if (digitalRead(reversePin) == LOW)
  {
    switchPos = SHUTTLE_REVERSE;
  }
  else
  {
    switchPos = SHUTTLE_IDLE;
  }

  // Check if it has changed
  if (switchPos != lastSwitch)
  {
    // Reset the state
    mode = switchPos;
    lastSwitch = switchPos;
  }
  else
  {
    if (mode == SHUTTLE_IDLE)
    {
      // Turn off all Relays
      digitalWrite(conveyorPowerPin, HIGH);
      digitalWrite(trackPowerPin, HIGH);
      digitalWrite(trackDirectionPin, HIGH);

      // and clear the LEDs
      digitalWrite(forwardLEDPin, LOW);
      digitalWrite(reverseLEDPin, LOW);

      // Read the trimmers for the timeout values
      if ((millis() - analogTimer) > ANALOG_TIME)
      {
        analogTimer = millis();
        
        // Slow time is 0 - 1024mS
        slowTime = analogRead(slowAnalog);
        Serial.print("Slowdown time is ");
        Serial.print(slowTime);
        Serial.println("mS");

        // Fill time is 0 - 5120mS
        fillTime = analogRead(fillAnalog) * 5;
        Serial.print("Fill time is ");
        Serial.print(fillTime);
        Serial.println("mS");

      }
    }
    else if (mode == SHUTTLE_REVERSE)
    {
      // In reverse, just power the track
      digitalWrite(conveyorPowerPin, HIGH);
      digitalWrite(trackDirectionPin, LOW);
      digitalWrite(trackPowerPin, LOW);
      
      digitalWrite(forwardLEDPin, LOW);
      digitalWrite(reverseLEDPin, HIGH);
    }
    else if (mode == SHUTTLE_FORWARD)
    {
      // Forward, power the track
      digitalWrite(conveyorPowerPin, HIGH);
      digitalWrite(trackDirectionPin, HIGH);
      digitalWrite(trackPowerPin, LOW);
      
      digitalWrite(forwardLEDPin, HIGH);
      digitalWrite(reverseLEDPin, LOW);

      // Look to see if the reed switch is triggered
      if (digitalRead(reedSwitchPin) == LOW || digitalRead(reedTestPin) == LOW)
      {
        // Yes, start the delay and move to the fill state. Turn off the LED ready
        mode = SHUTTLE_FILL_WAGON;
        digitalWrite(forwardLEDPin, LOW);
        Serial.println("Moving to Fill state");
        delay(slowTime);
        flashTimer = millis();
        lastLED = false;
      }
    }
    else if (mode == SHUTTLE_FILL_WAGON)
    {
      // When filling, power off the track and turn on the conveyor. Flash the LED. Wait for tilt.
      digitalWrite(conveyorPowerPin, LOW);
      digitalWrite(trackDirectionPin, HIGH);
      digitalWrite(trackPowerPin, HIGH);
      
      digitalWrite(reverseLEDPin, LOW);

      // Flash the LED every 0.5s
      if ((millis() - flashTimer) > 500L)
      {
        if (lastLED == true)
        {
          digitalWrite(forwardLEDPin, LOW);
          lastLED = false;
          flashTimer = millis();
        }
        else
        {
          digitalWrite(forwardLEDPin, HIGH);
          lastLED = true;
          flashTimer = millis();
        }
      }

      // Look to see if the tilt switch is triggered
      if (digitalRead(tiltSwitchPin) == LOW || digitalRead(tiltTestPin) == LOW)
      {
        // Yes, stop the conveyor immediatly
        digitalWrite(conveyorPowerPin, HIGH);

        // Turn the LED back on. Start the delay and move back to forward. 
        mode = SHUTTLE_FORWARD;
        digitalWrite(forwardLEDPin, HIGH);
        Serial.println("Tipped into wagon");
        delay(fillTime);
      }
    }
    else
    {
      // Invalid state so go idle
      mode = SHUTTLE_IDLE;
    }
  }
}




