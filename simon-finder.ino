#define NUM_BTNS 4
#define NUM_LEDS 4

int btnPins[NUM_BTNS] = {26, 27, 28, 29};  
int ledPins[NUM_LEDS] = {6, 7, 0, 1};     

const int maxSequence = 100;
int sequence[maxSequence];
int level = 1;
int inputIndex = 0;
bool isPlayerTurn = false;

void setup() {
  for (int i = 0; i < NUM_BTNS; i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  randomSeed(analogRead(0));


  for (int i = 0; i < 5; i++) {
    setAllLEDs(HIGH);
    delay(300);
    setAllLEDs(LOW);
    delay(300);
  }

  generateSequence();
  playSequence();
}

void loop() {
  if (!isPlayerTurn) return;

  int btn = readButton();
  if (btn != -1) {
    flashLED(btn);

    if (btn == sequence[inputIndex]) {
      inputIndex++;
      if (inputIndex == level) {
        blinkAllLEDs(1);   
        level++;
        if (level > maxSequence) level = maxSequence;
        inputIndex = 0;
        delay(500);
        playSequence();
      }
    } else {
      blinkAllLEDs(3);   
      restartGame();
    }
  }
}

void generateSequence() {
  for (int i = 0; i < maxSequence; i++) {
    sequence[i] = random(NUM_LEDS);
  }
}

void playSequence() {
  isPlayerTurn = false;
  delay(500);
  for (int i = 0; i < level; i++) {
    flashLED(sequence[i]);
    delay(250);
  }
  isPlayerTurn = true;
  inputIndex = 0;
}

void flashLED(int index) {
  digitalWrite(ledPins[index], HIGH);
  delay(300);
  digitalWrite(ledPins[index], LOW);
  delay(150);
}

int readButton() {
  for (int i = 0; i < NUM_BTNS; i++) {
    if (digitalRead(btnPins[i]) == LOW) {
      delay(20); 
      while (digitalRead(btnPins[i]) == LOW); 
      return i;
    }
  }
  return -1;
}

void blinkAllLEDs(int times) {
  for (int i = 0; i < times; i++) {
    setAllLEDs(HIGH);
    delay(300);
    setAllLEDs(LOW);
    delay(300);
  }
}

void setAllLEDs(int state) {
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(ledPins[i], state);
  }
}

void restartGame() {
  level = 1;
  inputIndex = 0;
  generateSequence();
  delay(1000);
  playSequence();
}
