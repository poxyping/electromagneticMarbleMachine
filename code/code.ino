unsigned long solenoidMillis, dischargeMillis, currentMillis;  // defines 3 variables to allow for precise timing without the limitations of the 'delay()' function
const int solenoidPulse = 500, dischargePulse = 100; // defines and assigns pulse lengths for the solenoid and firing coils respectively (in ms)
const int timeBetweenLaunches = 5000; // defines and assigns the minimum time between shots to ensure that the capacitors have a chance to fully charge before firing (in ms)

const int receiverIR = A4, hallEffect = A5; // defines the two input pin numbers
const int solenoid = 8, bleed = 9, discharge = 10, charge = 11, indicatorLED = 13; // defines the five output pin numbers

void setup() { // runs once at the start of the code  
  pinMode(hallEffect, INPUT); // sets the receiverIR and hallEffect pins to INPUT mode
  pinMode(receiverIR, INPUT);
  
  pinMode(solenoid, OUTPUT); // sets the solenoid, bleed, discharge, charge, and indicatorLED pins to OUTPUT mode
  pinMode(bleed, OUTPUT);
  pinMode(discharge, OUTPUT);
  pinMode(charge, OUTPUT);
  pinMode(indicatorLED, OUTPUT);

  digitalWrite(bleed, HIGH); // sets the 'bleed' pin to HIGH, as its output flows through a NOT gate to keep a thyristor that lets the firing capacitors to discharge into a bleeder resistor off until power is off 
  for (int i = 0; i < 9; i++) digitalWrite(indicatorLED, !digitalRead(indicatorLED)), delay(200); // flashes indicator LED on 5 times before leaving it on to indicate code is running

  solenoidMillis = 0; // sets the initial value of solenoidMillis so that there are 5000ms between when the circuit turns on and the first firing occurs, allowing the firing capacitors to charge
}

void loop() { // runs after 'setup()' until power is removed or the 'RESET' pin is grounded via a pushbbotton
  currentMillis = millis(); // assigns the time since the code was initialised in ms to the variable 'currentMillis'
  
  if (analogRead(hallEffect) > 614 && digitalRead(solenoid) == LOW && currentMillis - solenoidMillis >= timeBetweenLaunches) { // reads whether voltage through hall-effect sensor is greater than 3V, the solenoid is LOW, and 5000ms has passed since the last launch
    digitalWrite(charge, LOW); // sets the 'charge' pin to LOW to ensure the charging circuit is isolated
    digitalWrite(solenoid, HIGH); // sets the 'solenoid' pin to HIGH which activates a relay and triggers a push-type solenoid to push the marble into the firing coils
    solenoidMillis = currentMillis; // starts the solenoid timer so that the pulse length can be modified
  }
  if (currentMillis - solenoidMillis >= solenoidPulse && digitalRead(solenoid) == HIGH) { // reads whether the solenoid is activted and if so whether it has been on for the desired pulse time
    digitalWrite(solenoid, LOW);  // sets the 'solenoid' pin to LOW to end the pulse
  }
  
  if (analogRead(receiverIR) < 512 && digitalRead(charge) == LOW) { // reads whether voltage through IR reciever is less than 2.5V (i.e. the marble is obstructing the IR LED) and the charging circuit is isolated
    digitalWrite(discharge, HIGH); // sets the 'discharge' pin to HIGH which triggers an NPN transistor which in turn activates a thyristor to discharge a capacitor into the first firing coil
    dischargeMillis = currentMillis; // starts the discharge timer so that the pulse length can be modified
  }
  if (currentMillis - dischargeMillis >= dischargePulse && digitalRead(discharge) == HIGH) { // reads whether the discharge pin is HIGH and if so whether it has been on for the desired pulse time
    digitalWrite(discharge, LOW); // sets the 'discharge' pin to LOW to end the pulse
    digitalWrite(charge, HIGH); // reenables the charging circuit so that the capacitors can recharge and be ready to discharge again
  }
}


















