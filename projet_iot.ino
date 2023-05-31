/*
  PROJET IOT MAXENCE GELIG / AMAURY BRISACH
*/

// Déclaration des ports

// 3 lumieres RGB
int led1b = 2;
int led1r = 3;
int led1g = 4;

int led2b = 5;
int led2r = 6;
int led2g = 7;

int led3b = 8;
int led3r = 9;
int led3g = 10;

// Trafic Light
int tlR = 23;
int tlY = 25;
int tlG = 24;

char etatTraficLight;
bool isVoitureWaiting;
float currentTime;
float passageVertTime;
float passageJauneTime;
float passageRougeTime;


// Switch 3 ways
int switch3WayL = 52;
int switch3WayR = 53;

// Magnetic Switch
int switchReader = 22;

int mode;







void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initPin();
  isVoitureWaiting = false;
  etatTraficLight = 'R';
  setTraficLightColor(etatTraficLight);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  // Si pas de voiture actuellement, vérifier si il y'a une voiture qui attend
  if (isVoitureWaiting == false) {
    isVoitureWaiting = getIsVoiture();

    // Si une voiture vient d'arriver, on récupère le current time, et le time où le feu passera au vert
    if(isVoitureWaiting == true) {
      currentTime = getCurrentTime();
      passageVertTime = trafficLightGoGreen(currentTime);
      passageJauneTime = trafficLightGoYellow(passageVertTime);
      passageRougeTime = trafficLightGoRed(passageJauneTime);
    } else {
      passageVertTime = 0;
      passageJauneTime = 0;
      passageRougeTime = 0;
    }
    
    

  }

  if(isVoitureWaiting == true) {
    
    currentTime = getCurrentTime();
    if(currentTime > passageRougeTime) {
      setTraficLightColor('R');
      etatTraficLight = 'R';
      isVoitureWaiting = false;
    } else if(currentTime > passageJauneTime) {
      setTraficLightColor('Y');
      etatTraficLight = 'Y';
    } else if(currentTime > passageVertTime) {
      setTraficLightColor('G');
      etatTraficLight = 'G';
    }
  } else {
    Serial.println("non");
  }

  
  mode = getSwitch3WayValue();
  
  switch(mode) {
    case 0 : 
      modeJour();
      break;
    case 1 :
      modeSoir(isVoitureWaiting);
      break;
    case 2 :
      modeNuit();
      break;      
  }
  
  
  delay(100);
}

// Initialisation des pins
void initPin() {
  // 3 lumieres RGB
  pinMode(led1b, OUTPUT);
  pinMode(led1r, OUTPUT);
  pinMode(led1g, OUTPUT);

  pinMode(led2b, OUTPUT);
  pinMode(led2r, OUTPUT);
  pinMode(led2g, OUTPUT);

  pinMode(led3b, OUTPUT);
  pinMode(led3r, OUTPUT);
  pinMode(led3g, OUTPUT);

  // Switch 3 ways
  pinMode(switch3WayL, INPUT_PULLUP);
  pinMode(switch3WayR, INPUT_PULLUP);

  // Magnetic Switch
  pinMode(switchReader, INPUT_PULLUP);

  // Trafic Light
  pinMode(tlR, OUTPUT);
  pinMode(tlY, OUTPUT);
  pinMode(tlG, OUTPUT);

}

int getSwitch3WayValue() {

  int valueL = digitalRead(switch3WayL);
  int valueR = digitalRead(switch3WayR);
  int valueSwitch;
  
  if(valueL == 0) {
    // Switch à droite
    valueSwitch = 2;
    
  } else if(valueR == 0) {
    // Switch à gauche
    valueSwitch = 0;
  } else {
    // Switch au milieu
    valueSwitch = 1;
  }

  return valueSwitch;
}

bool getIsVoiture() {
  int switchValue = digitalRead(switchReader);
  if(switchValue == 1) {
    return false;
  } else {
    return true;
  } 
}


void modeJour() {
  setAllLightVal(0);
}

void modeSoir(bool isVoitureWaiting) {
  if(isVoitureWaiting == true) {
    setAllLightVal(255);
  } else {
    setAllLightVal(20);
  }
  
}

void modeNuit() {
  setAllLightVal(255);
}

void setAllLightVal(int val) {
  setLight1Val(val);
  setLight2Val(val);
  setLight3Val(val);
}

void setLight1Val(int val) {
  analogWrite(led1b, val);
  analogWrite(led1r, val);
  analogWrite(led1g, val);
}

void setLight2Val(int val) {
  analogWrite(led2b, val);
  analogWrite(led2r, val);
  analogWrite(led2g, val);
}

void setLight3Val(int val) {
  analogWrite(led3b, val);
  analogWrite(led3r, val);
  analogWrite(led3g, val);
}


void setTraficLightColor(char color) {

  switch(color) {
    case 'R' :
      digitalWrite(tlR, HIGH);
      digitalWrite(tlY, LOW);
      digitalWrite(tlG, LOW);
      break;
    case 'Y' :
      digitalWrite(tlR, LOW);
      digitalWrite(tlY, HIGH);
      digitalWrite(tlG, LOW);
      break;
    case 'G' :
      digitalWrite(tlR, LOW);
      digitalWrite(tlY, LOW);
      digitalWrite(tlG, HIGH);
      break;
  }
  
}

float getCurrentTime() {
  return (float)millis() / 1000;
}


float trafficLightGoGreen(float referenceTime) {
  return referenceTime + 2;
}

float trafficLightGoYellow(float referenceTime) {
  return referenceTime + 10;
}

float trafficLightGoRed(float referenceTime) {
  return referenceTime + 2;
}



