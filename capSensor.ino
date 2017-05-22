// Class to represent a capacitive sensor
// The two pins are connected to some conductive surface, in our case some copper leaf.
// and a strong resistor, in our case 1 megaOhm. 
class capSensor 
{
  public: 
    capSensor();
    capSensor(int sender, int receive);
    void setPins (int sender, int receive); // In case using default constructor for some reason
    void changeTimeout (int cycles); // Can be calibrated to work with whatever resistor used
    int measure (int samples); // main use case function
    
  private:
    int sendPin; 
    int receivePin;
    int timeout;
    int timerTotal;
};

capSensor::capSensor()
{
  sendPin = -1;
  receivePin = -1;
}

capSensor::capSensor(int sender, int receiver)
{
  sendPin = sender;
  receivePin = receiver;
  timerTotal = 0;
  timeout = 1000;
  pinMode(sendPin, OUTPUT);
  pinMode(receivePin, INPUT);
}

void capSensor::setPins(int sender, int receiver)
{
  sendPin = sender;
  receivePin = receiver;
  timerTotal = 0;
  timeout = 1000;
  pinMode(sendPin, OUTPUT);
  pinMode(receivePin, INPUT);
}

void capSensor::changeTimeout(int cycles)
{
  timeout = cycles;
}

int capSensor::measure( int samples)
{
  if (sendPin == -1)
  {
    return -1;
  }
  timerTotal = 0;
  for (int i = 0; i < samples; i++)
  {
    digitalWrite(sendPin, HIGH);
    while( digitalRead(receivePin) == LOW && timerTotal < timeout)
    {
      timerTotal ++;
    }
    digitalWrite(sendPin, LOW);
    while (digitalRead(receivePin) == HIGH && timerTotal < timeout)
    {
      timerTotal ++;
    }
    
    if (timerTotal >= timeout)
    {
      return -1;
    }
  }
  return timerTotal;
}

// ACTUAL RUNNING CODE

capSensor play    = capSensor(3, 4);
capSensor pause   = capSensor(5,6);
capSensor volUp   = capSensor(7,8);
capSensor volDown = capSensor(9,10);

int playC    = 0;
int pauseC   = 0;
int volUpC   = 0;
int volDownC = 0;

void setup() {
  Serial.begin(38400); // This appears to be the right baud
}

void loop() {
  int start = millis();
  playC  = play.measure(20);
  pauseC = pause.measure(20);
  volUpC = volUp.measure(20);
  volDownC= volDown.measure(20);

  if(Serial.available() > 0)  // If the serial is available, send capacitance measurements
  {
    Serial.print("play ");
    Serial.println(playC);
    
    Serial.print("pause ");
    Serial.println(pauseC);
    
    Serial.print("volUp ");
    Serial.println(volUpC);
    
    Serial.print("volDown ");
    Serial.println(volDownC);
    
  }
  
}
