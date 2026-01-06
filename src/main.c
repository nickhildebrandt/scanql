#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <SQL-String>\n", argv[0]);
    return 1;
  }
  printf("Received SQL String: %s\n", argv[1]);
  return 0;
}
