#include <CircularBuffer/circularBuffer.h>
#include <stdio.h>

int main() {
  CircularBuffer cb = CB_initialize(sizeof(char), 3);
  //    r/w                       toRead = 0
  // [   •   ,   •   ,   •   ]
  //     r       w                toRead = 1
  // [  'a'  ,   •   ,   •   ]
  //     r               w        toRead = 2
  // [  'a'  ,  'b'  ,   •   ]
  //    w/r                       toRead = 3
  // [  'a'  ,  'b'  ,  'c'  ]
  //    w/r                       toRead = 3
  // [  'a'  ,  'b'  ,  'c'  ]
  //    w/r                       toRead = 3
  // [  'a'  ,  'b'  ,  'c'  ]
  //            w/r               toRead = 3
  // [  'd'  ,  'b'  ,  'c'  ]
  char c = 'a';
  CB_writeNext(cb, &c);
  c = 'b';
  CB_writeNext(cb, &c);
  c = 'c';
  CB_writeNext(cb, &c);
  c = 'd';
  CB_writeNext(cb, &c);
  c = 'e';
  CB_writeNext(cb, &c);

  char* c2;
  while ((c2 = CB_readNext(cb)) != NULL) {
    printf("%c, ", *c2);
  }
  printf("\n");
  CB_free(cb);

  cb = CB_initialize(sizeof(char), 5);

  while ((c = getc()) != EOF) {
    CB_writeNext(cb, &c);
  }
  printf("\n");

  while ((c2 = CB_readNext(cb)) != NULL) {
    printf("%c, ", *c2);
  }

  CB_free(cb);
  return 0;
}
