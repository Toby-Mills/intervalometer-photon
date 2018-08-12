/*
 * Project intervalometer-photon
 * Description:
 * Author:
 * Date:
 */

int shutterPin = D7;
int LEDPin = D5;

// user settings
int photoIntervalSeconds = 30;
bool mirrorLockupDelay = true;
bool blackFrameEnabled = true;
int exposureLengthMillis = 1;
int bracketExposureLengthMillis = 0;

// internal variables
enum ExposureBracketShot {UnderExposed, Exposed, OverExposed};
enum CameraMode {None, MirrorLockupDelay, Exposure, BlackFrameDelay, Processing};
CameraMode currentMode = None;
int lastPhotoStartTime = 0;
int currentModeStartTime = 0;
ExposureBracketShot currentBracketShot = Exposed;
int currentBracketExposureDuration = 0;
int mirrorLockupDuration = 1000;
int processingDuration = 300;

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
pinMode(shutterPin, OUTPUT);
pinMode(LEDPin, OUTPUT);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  switch(currentMode){
    case None:
      if(millis() - lastPhotoStartTime > photoIntervalSeconds * 1000){
        if (bracketExposureLengthMillis > 0){
          currentBracketShot = UnderExposed
        }
        setMode(MirrorLockupDelay);
      }
      break;
    case:MirrorLockupDelay:
      if (mirrorLockupDelay){
        if (currentModeStartTime = -1){
          currentModeStartTime = millis();
          setShutter(HIGH);
        }else{
          if (millis() - currentModeStartTime < mirrorLockupDuration){
            setShutter(LOW);
          }else{
            setMode(Exposure);
          }
        }
      }else{
        setMode(Exposure):
      }
      break;
    case Exposure:
      if (currentModeStartTime = -1){
        currentModeStartTime = millis();
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
        if (millis() - currentModeStartTime >= currentBracketExposureDuration){
          setShutter(LOW);
          setMode(BlackFrameDelay)
        }
      }
      break;
    case BlackFrameDelay:
      if (blackFrameEnabled){
        if (currentModeStartTime = -1){
          currentModeStartTime = millis();
        }else{
          if (millis() - currentModeStartTime >= currentBracketExposureDuration){
            setMode(Processing);
          }
        }
      }else{
        setMode(Processing);
      }
      break;
    case Processing:
      if (currentModeStartTime = -1){
        currentModeStartTime = millis();
      }else{
        if (millis() - currentModeStartTime >= processingDuration){
          if (bracketExposureLengthMillis = 0){
            setMode(None);
          }else{
            switch(currentBracketShot){
              case UnderExposed:
                currentBracketShot = Exposed;
                setMode(MirrorLockupDelay);
                break;
              case Exposed:
                currentBracketShot = OverExposed;
                setMode(MirrorLockupDelay);
                break;
              case OverExposed:
                setMode(None);
                break;
            }
          }
        }
      }
      break;
  }
}

void setMode(value){
  currentMode = value;
  currentModeStartTime = -1;
}

void setShutter(value){
  digitalWrite(shutterPin, value);
  digitalWrite(LEDPin, value);
}
