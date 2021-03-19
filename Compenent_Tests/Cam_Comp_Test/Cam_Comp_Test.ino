/*
   Island City Explorers
   Spy Cam board component test code
   V 1.0
   Release: 2/25/21
   S. Shaw

*/

int cam_trig = 0;
int cam_pow = 1;
int cam_gnd = 2;

void setup() {
  // initialize the digital pins as output.
  pinMode(cam_trig, OUTPUT);
  pinMode(cam_pow, OUTPUT);
  pinMode(cam_gnd, OUTPUT);

  digitalWrite(cam_gnd, LOW);
  digitalWrite(cam_pow, HIGH);
  digitalWrite(cam_trig, HIGH);
}

// Hold HIGH and trigger quick (<250ms) LOW to take a photo. Holding LOW and trigger HIGH starts/stops video recording

void loop() {
  digitalWrite(cam_trig, LOW);

  delay(1000);

  digitalWrite(cam_trig, HIGH);

  //Delay between pictures
  delay(4000);

}
