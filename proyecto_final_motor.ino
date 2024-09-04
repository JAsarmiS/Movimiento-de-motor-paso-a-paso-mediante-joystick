#include <AccelStepper.h>
#include <Encoder.h>

// Pines de control del motor
#define DIR_PIN 2
#define STEP_PIN 3

// Potenciómetros, pulsador y encoder
#define POT1_PIN A2
#define POT2_PIN A3
#define BUTTON_PIN 5
Encoder encoder(A0, A1);

// Variables para el manejo del motor
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);
bool buttonPressed = false;

//parte necesaria para el encoder
int stepsPerRevolution = 400; //pasos que da el motor


void setup() {

  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  stepper.setMaxSpeed(1000); // Velocidad máxima del motor
  stepper.setAcceleration(500); // Aceleración del motor
}

void loop() {
  int pot1Value = analogRead(POT1_PIN);
  int pot2Value = analogRead(POT2_PIN);
  int buttonState = digitalRead(BUTTON_PIN);
  long currentPosition = encoder.read();

  currentPosition = (currentPosition + stepsPerRevolution) % stepsPerRevolution;

  // Mover un paso si se presiona el botón
  if (buttonState == LOW && !buttonPressed) {
    stepper.move(1);
    stepper.run();

    buttonPressed = true;
  } else if (buttonState == HIGH) {
    buttonPressed = false;
  }

  // Movimiento con potenciómetro A1 (velocidad lenta)
  float speedPot1 = map(pot1Value, 0, 1000, -500, 500);
  if (!buttonPressed && (speedPot1 < -30 || speedPot1 > 30)) {
    stepper.setSpeed(speedPot1);
    stepper.runSpeed();

  }

  // Movimiento con potenciómetro A0 (velocidad más rápida)
  float speedPot2 = map(pot2Value, 0, 1000, -10, 10);
  if (!buttonPressed && (speedPot2 < -30 || speedPot2 > 30)) {
    stepper.setSpeed(speedPot2);
    stepper.runSpeed();

  }

    //Para sacar la informacion por el serial
    Serial.print("Posicion: ");
    Serial.print(stepper.currentPosition());
    Serial.print("\t");
    Serial.print("Velocidad: ");
    Serial.println(stepper.runSpeed());
}

