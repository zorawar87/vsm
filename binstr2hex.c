#include <stdio.h>
#include <stdlib.h>

void main() {
  char *bits = (char*)malloc (16 * sizeof (char));
  char *line = (char*)malloc (100 * sizeof (char));
  while (scanf ("%s", line) != EOF) {
    sscanf(line, "%16s", bits);
    int total = 0;
    while (*bits) {
      total <<= 1;
      if (*bits++ == '1') total ^= 1;
    }
    printf ("%04x\n", total);
  }
}
