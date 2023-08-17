const int linHall = A1;
const int switchPin = 2;

float baseline = 521.0;
long polls[100] = {};
float deltas[50] = {};


void setup() {
  pinMode(linHall, INPUT);
  pinMode(switchPin, OUTPUT);
}

void loop() {
  // get measurement minus base
  long measure = (analogRead(linHall) - baseline);
  
  // scale to emphasize trends
  measure <<= 4;

  // reset range
  long peak = 0;
  long low = 0;

  // slide values in array down one, dropping last
  for (int i = 99; i > 0; i--) {
    polls[i] = polls[i - 1];
    if (polls[i] > peak) {
      peak = polls[i];
    }
    if (polls[i] < low) {
      low = polls[i];
    }
  }

  // add latest measurement
  polls[0] = measure;
  
  // finding mode (ish) to weed out spikes
  long dTotal = 0;
  float delta = peak - low;
  for (int i = 49; i > 0; i--) {
    deltas[i] = deltas[i - 1];
    dTotal += deltas[i];
  }

  deltas[0] = delta;    // usually ~96 when on and ~32 when off

  // using sum of last 50 deltas to avoid outliers
  if (dTotal > 2500) {
    // Serial.println("ON");
    digitalWrite(switchPin, HIGH);
  } else {
    // Serial.println("OFF");
    digitalWrite(switchPin, LOW);
  }
}

