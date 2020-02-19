#include <stdio.h>
#include <stdint.h>

#define TIME_MASK 255
#define TIME_SHIFT 24
#define TYPE_MASK 3
#define TYPE_SHIFT 22
#define CHANNEL_MASK 15
#define CHANNEL_SHIFT 18

void displayBinary (uint32_t n) {
  int l = sizeof(n) * 8;
  for (int i = l - 1; i >= 0; i--) {
    printf("%x", (n & (1 << i)) >> i);
  }
  printf("\n");
}

int main() {
  uint32_t event = 0;
  uint8_t time = 255;
  uint8_t type = 3;
  uint8_t channel = 15;

  event |= ((time & TIME_MASK) << TIME_SHIFT);
  event |= ((type & TYPE_MASK) << TYPE_SHIFT);
  event |= ((channel & CHANNEL_MASK) << CHANNEL_SHIFT);

  displayBinary(event);

  printf("time %i\n", (event >> TIME_SHIFT) & TIME_MASK);
  printf("type %i\n", (event >> TYPE_SHIFT) & TYPE_MASK);
  printf("channel %i\n", (event >> CHANNEL_SHIFT) & CHANNEL_MASK);
  return 0;
}
