
#define NOTE_C5  523
#define NOTE_E5  659
#define NOTE_G5  784
#define NOTE_C6  1047

#define WHITE    0
#define RED      1
#define GREEN    2
#define YELLOW   3

const int numColors = 4;

int buttons[numColors] = {2, 3, 4, 5};
int leds[numColors] = {9, 10, 11, 12};
int notes[numColors] = {NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6};

int code[numColors];
int answer[numColors];
int correctNum;
bool finished = false;

// Speaker
int spk = 7;

// Speed multiplier; (1-10) is advised
int tempo = 3;


void test_outputs() {
  Serial.print("Starting output test");
  
  play(WHITE);
  Serial.print(".");
  
  play(RED);
  Serial.print(".");
  
  play(GREEN);
  Serial.print(".");
  
  play(YELLOW);
  Serial.println(" Done!");
}

void play(int color) {
  // Start playing
  digitalWrite(leds[color], HIGH);
  tone(spk, notes[color]);
  delay(40 * tempo);

  // Stop playing
  digitalWrite(leds[color], LOW);
  noTone(spk);
  delay(40 * tempo);  
}

void set_code() {
  // Populate the code array with the number of colors. 
  for (int i = 0; i < numColors; i++) {
    code[i] = i+1;
  }

  // Everyday I'm shufflin'...
  for (int i = 0; i < numColors; i++) {
    randomSeed(analogRead(A0));
    int randNum = random(0, numColors);
    int temp = code[randNum];
    code[randNum] = code[i];
    code[i] = temp;
  }

  // Print the solution to the serial com
  Serial.print("The code is: ");
  for (int i = 0; i < numColors; i++) {
    Serial.print(code[i]);
  }
  Serial.println(".");
}

void reset() {
  for (int i = 0; i < numColors; i++) {
    answer[i] = 0;
  }
  correctNum = 0;
  finished = false;
}

void setup() {
  Serial.begin(9600);
  // Speaker
  pinMode(spk, OUTPUT);

  // LEDs and buttons
  for (int i = 0; i < numColors; i++) {
    pinMode(leds[i], OUTPUT);
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH); // pull-up    
  }

  test_outputs();
  set_code();
  reset();
}

void loop() {
  // Light up the "unlocked" LEDs
  for (int i = 0; i < numColors; i++) {
    int button = code[i] - 1;
    if (answer[i] == code[i]) {
      digitalWrite(leds[button], HIGH);
    } else {
      digitalWrite(leds[button], LOW);
    }
  }

  // Check if a button is pushed
  for (int i = 0; i < numColors; i++) {
    if (digitalRead(buttons[i]) == LOW) {
      if (buttons[i] - 1 == code[correctNum]) {
        Serial.print("Correct. You pressed ");
        Serial.print(buttons[i] -1);
        Serial.print(" and the code at position ");
        Serial.print(correctNum);
        Serial.print(" is ");
        Serial.print(code[correctNum]);
        Serial.println(".");
        answer[correctNum] = code[correctNum];
        correctNum++;

        Serial.print("The code: ");
        for (int i = 0; i < numColors; i++) Serial.print(code[i]);
        Serial.println();

        Serial.print("The answer: ");
        for (int i = 0; i < numColors; i++) Serial.print(answer[i]);
        Serial.println();
        play(i);
      } else {
        reset();
        set_code();
        failed();        
      }      
    }
    if (correctNum == numColors && !finished) {
      victory();
      finished = true;
    }
  }  
}

void failed() {
  tone(spk, 55);
  delay(160 * tempo);

  noTone(spk);
  delay(40 * tempo);  
}

void victory() {
  play(WHITE);
  play(RED);
  play(GREEN);
  play(YELLOW);
  delay(80 * tempo);
  play(GREEN);
  play(YELLOW);  
}
