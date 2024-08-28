#include "Particle.h"

#define LOCK_CTRL_PIN D7
#define LOCK_STATE_PIN D6

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_INFO);

enum LockState
{
  UNKNOWN,
  UNLOCKED,
  LOCKED
};

LockState lastLockState = UNKNOWN;

int lockFnCallback(String lockState)
{
  Log.info(lockState);
  if (lockState == "UNLOCK")
  {
    digitalWrite(LOCK_CTRL_PIN, 1);
    delay(500);
    digitalWrite(LOCK_CTRL_PIN, 0);
  }
  return 0;
}

LockState readLockState()
{
  return digitalRead(LOCK_STATE_PIN) == 1 ? UNLOCKED : LOCKED;
}

void setup()
{
  bool success = Particle.function("lockFn", lockFnCallback);
  if (!success)
  {
    Log.info("Failed to initialize function");
  }
  pinMode(LOCK_CTRL_PIN, OUTPUT);
  pinMode(LOCK_STATE_PIN, INPUT_PULLUP);
  lastLockState = readLockState();
}

void loop()
{
  LockState currentLockState = readLockState();
  if (currentLockState != lastLockState)
  {
    Log.info(currentLockState == UNLOCKED ? "Door unlocked" : "Door locked");
    lastLockState = currentLockState;
  }
}

