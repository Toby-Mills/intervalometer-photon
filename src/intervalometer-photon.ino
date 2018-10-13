//--------------------------------------------------------------
// System Configuration
//--------------------------------------------------------------

SYSTEM_THREAD(ENABLED); //enables system functions to happen in a separate thread from the application setup and loop
//this includes connecting to the network and the cloud

//--------------------------------------------------------------
// Pins
//--------------------------------------------------------------

int shutterPin = D1;
int LEDPin = D7;
int switchPin = D6;

//--------------------------------------------------------------
// User Settings
//--------------------------------------------------------------

int photoIntervalSeconds = 15;
bool mirrorLockupDelay = true;
bool blackFrameEnabled = true;
int exposureLengthMillis = 3000;
int bracketExposureLengthMillis = 0;
bool started = false;

//--------------------------------------------------------------
// Internal Variables
//--------------------------------------------------------------

//enums
enum ExposureBracketShot {UnderExposed, Exposed, OverExposed};
enum PhotoPhase {None = 0, MirrorLockupDelay = 1, MirrorLockupBuffer = 2, Exposure = 3, BlackFrameDelay = 4, Processing = 5};

// internal variables
bool connectedOnce = false;
int debugTimeout = 0;

PhotoPhase currentPhase = None;
int lastPhotoStartTime = 0;
int currentPhaseStartTime = 0;
ExposureBracketShot currentBracketShot = Exposed;
int currentBracketExposureDuration = 0;
int mirrorLockupDuration = 500;
int mirrorLockupBuffer = 1000;
int processingDuration = 300;

char *sourceCode =  "https://github.com/Toby-Mills/intervalometer-photon";

//---------------------------------------------------------------
// Functions
//---------------------------------------------------------------

void start(){
  started = true;
}

void end(){
  started = false;
  setShutter(LOW);
}

void setPhase(PhotoPhase value){
  currentPhase = value;
  currentPhaseStartTime = -1;//indicates that the phase has not started yet
}

bool phaseStarted(){
  return currentPhaseStartTime > -1;
}

void startPhase(){
  currentPhaseStartTime = millis();
}

int phaseElapsedTime(){
  return millis() - currentPhaseStartTime;
}

void setShutter(int value){
  digitalWrite(shutterPin, value);
  digitalWrite(LEDPin, value);

}

bool debugging(){
  return millis() < debugTimeout;
}

int startDebugging(String duration){
  debugTimeout = millis() + duration.toFloat();
  debugMessage("DebugStart","");
  return duration.toFloat();
}

bool debugMessage(String eventName, String data){
  if (debugging()){
    return Particle.publish(eventName, data);
  }
}

//--------------------------------------------------------------
// Setup
//--------------------------------------------------------------

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  pinMode(shutterPin, OUTPUT);
  pinMode(LEDPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLDOWN);
}

//--------------------------------------------------------------
// Loop
//--------------------------------------------------------------

// loop() runs over and over again, as quickly as it can execute.
void loop() {

  //code to register cloud functions once the particle is connected
  if (connectedOnce == false) {
    if (Particle.connected()) {
      //Register variables and methods to allow control via Particle Cloud
      Particle.variable("sourceCode", sourceCode, STRING);
      Particle.variable("interval", photoIntervalSeconds);
      Particle.variable("lockup", mirrorLockupDelay);
      Particle.variable("blackFrame", blackFrameEnabled);
      Particle.variable("exposure", exposureLengthMillis);
      Particle.variable("bracket", bracketExposureLengthMillis);
      Particle.variable("phaseStart", currentPhaseStartTime);
      Particle.variable("phase", currentPhase);
      Particle.variable("lastPhoto", lastPhotoStartTime);
      Particle.variable("phase", currentPhase);
      Particle.function("startDebug", startDebugging);
      connectedOnce = true;
    }
  }
  if(digitalRead(switchPin) == HIGH){
    start();
  }else{
    end();
  }

  if(started){
    switch(currentPhase){
      case None:
        if(millis() - lastPhotoStartTime >= photoIntervalSeconds * 1000){
          lastPhotoStartTime = millis();
          if (bracketExposureLengthMillis > 0){
            currentBracketShot = UnderExposed;
          }
          setPhase(MirrorLockupDelay);
        }
        break;
      case MirrorLockupDelay:
        if (mirrorLockupDelay){
          if (phaseStarted()){
            if (phaseElapsedTime() >= mirrorLockupDuration){
              setShutter(LOW);
              setPhase(MirrorLockupBuffer);
            }
          }else{
              startPhase();
              setShutter(HIGH);
          }
        }else{
          setPhase(Exposure);
        }
        break;

      case MirrorLockupBuffer:
          if (mirrorLockupDuration > 0){
            if (phaseStarted()){
              if (phaseElapsedTime() >= mirrorLockupBuffer){
                setPhase(Exposure);
              }
            }else{
              startPhase();
            }
          }else{
            setPhase(Exposure);
          }
        break;

      case Exposure:
        if (phaseStarted()){
          if (phaseElapsedTime() >= currentBracketExposureDuration){
            setShutter(LOW);
            setPhase(BlackFrameDelay);
          }
        }else{
          startPhase();
          switch (currentBracketShot) {
            case UnderExposed:
              currentBracketExposureDuration = exposureLengthMillis - bracketExposureLengthMillis;
              break;
            case Exposed:
              currentBracketExposureDuration = exposureLengthMillis;
              break;
            case OverExposed:
              currentBracketExposureDuration = exposureLengthMillis + bracketExposureLengthMillis;
              break;
          }
          setShutter(HIGH);
        }
        break;

      case BlackFrameDelay:
        if (blackFrameEnabled){
          if (phaseStarted()){
            if (phaseElapsedTime() >= currentBracketExposureDuration){
              setPhase(Processing);
            }
          }else{
            startPhase();
          }
        }else{
          setPhase(Processing);
        }
        break;

      case Processing:
        if (phaseStarted()){
          if (phaseElapsedTime() >= processingDuration){
            if (bracketExposureLengthMillis == 0){
              setPhase(None);
            }else{
              switch(currentBracketShot){
                case UnderExposed:
                  currentBracketShot = Exposed;
                  setPhase(MirrorLockupDelay);
                  break;
                case Exposed:
                  currentBracketShot = OverExposed;
                  setPhase(MirrorLockupDelay);
                  break;
                case OverExposed:
                  setPhase(None);
                  break;
              }
            }
          }
        }else{
          startPhase();
        }
        break;
    }
  }
}
