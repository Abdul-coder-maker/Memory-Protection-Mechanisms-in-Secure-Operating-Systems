#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for srand, rand
#include <time.h>   // for time
unsigned long generateCanary()
{
  srand((unsigned int)time(NULL));
  return ((unsigned long)rand()) ^ ((unsigned long)time(NULL));
}
void grantAdminReset()
{
  printf("🔓 Password Reset: Admin privileges granted!\n");
}
// ✅ Harmless log function
void logEvent(const char *msg)
{
  printf("[LOG] %s\n", msg);
}
// Fake validation to distract from the overflow
int fakeSecurityCheck(const char *input)

{
  return strlen(input) < 16;
}

// 🧨 Vulnerable Function: Stack Canary + Shadow Stack Needed Here
void resetPassword()
{

  unsigned long canary = generateCanary();
  unsigned long canaryCopy = canary;
  char inputBuffer[12];    // Vulnerable buffer
  void (*action)() = NULL; // Action pointer attacker can target

  printf("Enter your reset token: ");
  gets(inputBuffer); // UNSAFE! Allows overflow beyond buffer size

  printf("Canary value: 0x%lx\n", canary);

  if (canary != canaryCopy)
  {
    fprintf(stderr, "!!! Stack smashing detected manually !!!\n");
    // exit(1);
  }

  // If canary is intact, proceed
  printf("Input processed: %s\n", inputBuffer);

  // Check that gives false sense of security
  if (fakeSecurityCheck(inputBuffer))
  {
    logEvent("Reset token accepted.");
  }
  else
  {
    logEvent("Invalid reset token.");
  }

  printf("Input processed: %s\n", inputBuffer);

  // 🚨 Potential control hijack point
  if (action != NULL)
  {
    action(); // Could be redirected to grantAdminReset()
  }
}

int main()
{
  printf("🔐 SecureCorp Self-Service Password Reset Portal 🔐\n\n");
  resetPassword();
  printf("Process complete.\n");
  return 0;
}