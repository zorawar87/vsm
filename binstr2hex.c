#include <stdio.h>
#include <stdlib.h>

void main() {
  char *bits = (char*)malloc (16 * sizeof (char));
  char comments[BUFSIZ] = {0};
  while (scanf ("%16s", bits) != EOF) {
    fgets (comments, BUFSIZ, stdin);
    int total = 0;
    while (*bits) {
      total <<= 1;
      if (*bits++ == '1') total ^= 1;
    }
    printf ("%04x\n", total);
  }
}
