/*
 * Project intervalometer-photon
 * Description:
 * Author:
 * Date:
 */
char *sourceCode =  "https://github.com/Toby-Mills/intervalometer-photon";

int shutterPin = D1;
int LEDPin = D7;

// user settings
int photoIntervalSeconds = 15;
bool mirrorLockupDelay = true;
bool blackFrameEnabled = true;
int exposureLengthMillis = 3000;
int bracketExposureLengthMillis = 0;

// internal variables
bool connectedOnce = false;
enum ExposureBracketShot {UnderExposed, Exposed, OverExposed};
enum PhotoPhase {None = 0, MirrorLockupDelay = 1, MirrorLockupBuffer = 2, Exposure = 3, BlackFrameDelay = 4, Processing = 5};
PhotoPhase currentPhase = None;
int lastPhotoStartTime = 0;
int currentPhaseStartTime = 0;
ExposureBracketShot currentBracketShot = Exposed;
int currentBracketExposureDuration = 0;
int mirrorLockupDuration = 500;
int mirrorLockupBuffer = 1000;
int processingDuration = 300;

void setPhase(PhotoPhase value){
  currentPhase = value;
  currentPhaseStartTime = -1;//indicates that the phase has not started yet
}

void setShutter(int value){
  digitalWrite(shutterPin, value);
  digitalWrite(LEDPin, value);
}

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  pinMode(shutterPin, OUTPUT);
  pinMode(LEDPin, OUTPUT);
  setPhase(None);
}

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
      connectedOnce = true;
    }
  }

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
        if (currentPhaseStartTime == -1){
          currentPhaseStartTime = millis();
          setShutter(HIGH);
        }else{
          if (millis() - currentPhaseStartTime >= mirrorLockupDuration){
            setShutter(LOW);
            setPhase(MirrorLockupBuffer);
          }
        }
      }else{
        setPhase(Exposure);
      }
      break;
    case MirrorLockupBuffer:
        if (mirrorLockupDuration > 0){
          if (currentPhaseStartTime == -1){
            currentPhaseStartTime = millis();
          }else{
            if (millis() - currentPhaseStartTime >= mirrorLockupBuffer){
              setPhase(Exposure);
            }
          }
        }else{
          setPhase(Exposure);
        }
      break;
    case Exposure:
      if (currentPhaseStartTime == -1){
        currentPhaseStartTime = millis();
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
      }else{
        if (millis() - currentPhaseStartTime >= currentBracketExposureDuration){
          setShutter(LOW);
          setPhase(BlackFrameDelay);
        }
      }
      break;
    case BlackFrameDelay:
      if (blackFrameEnabled){
        if (currentPhaseStartTime == -1){
          currentPhaseStartTime = millis();
        }else{
          if (millis() - currentPhaseStartTime >= currentBracketExposureDuration){
            setPhase(Processing);
          }
        }
      }else{
        setPhase(Processing);
      }
      break;
    case Processing:
      if (currentPhaseStartTime == -1){
        currentPhaseStartTime = millis();
      }else{
        if (millis() - currentPhaseStartTime >= processingDuration){
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
      }
      break;
  }
}
