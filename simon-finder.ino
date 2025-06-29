#define NUM_BTNS 4
#define NUM_LEDS 4

int btnPins[NUM_BTNS] = {2, 3, 4, 5};     
int ledPins[NUM_LEDS] = {6, 7, 8, 9};    

const int maxSequence = 100;
int sequence[maxSequence];
int level = 0;
int inputIndex = 0;
bool isPlayerTurn = false;
bool inGame = false;
int winLevel = 0;

void setup() {
  for (int i = 0; i < NUM_BTNS; i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  randomSeed(analogRead(0)); 
  showMenu();
}

void loop() {
  if (!inGame) return;

  if (isPlayerTurn) {
    int btn = readButton();
    if (btn != -1) {
      flashLED(btn);
      if (btn == sequence[inputIndex]) {
        inputIndex++;
        if (inputIndex == level) {
          delay(400);
          if (level == winLevel) {
            winAnimation();
            showMenu();
            return;
          }
          nextLevel();
        }
      } else {
        gameOver();
      }
    }
  }
}


void showMenu() {
  inGame = false;

  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(300);
    digitalWrite(ledPins[i], LOW);
    delay(150);
  }


  while (true) {
    for (int i = 0; i < NUM_LEDS; i++) digitalWrite(ledPins[i], HIGH);
    delay(300);
    for (int i = 0; i < NUM_LEDS; i++) digitalWrite(ledPins[i], LOW);
    delay(300);

    int mode = readButton();
    if (mode != -1) {
      switch (mode) {
        case 0: winLevel = 10; break;
        case 1: winLevel = 15; break;
        case 2: winLevel = 25; break;
        case 3: winLevel = maxSequence; break;
      }

      countdownSequence();
      startGame();
      break;
    }
  }
}

void countdownSequence() {
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(300);
    digitalWrite(ledPins[i], LOW);
    delay(150);
  }
  delay(5000);
}

void startGame() {
  inGame = true;
  level = 1;
  inputIndex = 0;
  generateSequence();
  playSequence();
}


void nextLevel() {
  level++;
  inputIndex = 0;
  playSequence();
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
}

void flashLED(int index) {
  digitalWrite(ledPins[index], HIGH);
  delay(300);
  digitalWrite(ledPins[index], LOW);
  delay(150);
}


void gameOver() {
  isPlayerTurn = false;

 
  for (int i = 0; i < NUM_LEDS; i++) digitalWrite(ledPins[i], HIGH);
  delay(500);
  for (int i = 0; i < NUM_LEDS; i++) digitalWrite(ledPins[i], LOW);
  delay(500);

 
  int score = level - 1;
  int fullFlashes = score / 4;
  int singleFlashes = score % 4;

  for (int i = 0; i < fullFlashes; i++) {
    for (int j = 0; j < NUM_LEDS; j++) digitalWrite(ledPins[j], HIGH);
    delay(300);
    for (int j = 0; j < NUM_LEDS; j++) digitalWrite(ledPins[j], LOW);
    delay(300);
  }

  for (int i = 0; i < singleFlashes; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(300);
    digitalWrite(ledPins[i], LOW);
    delay(300);
  }

  delay(1000);
  showMenu();
}

void winAnimation() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < NUM_LEDS; j++) digitalWrite(ledPins[j], HIGH);
    delay(300);
    for (int j = 0; j < NUM_LEDS; j++) digitalWrite(ledPins[j], LOW);
    delay(300);
  }
}



int readButton() {
  for (int i = 0; i < NUM_BTNS; i++) {
    if (digitalRead(btnPins[i]) == LOW) {
      delay(20); // debounce
      while (digitalRead(btnPins[i]) == LOW); // wait for release
      return i;
    }
  }
  return -1;
}
