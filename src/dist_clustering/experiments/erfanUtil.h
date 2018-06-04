#ifndef _ERFAN_UTIL_H
#define _ERFAN_UTIL_H

// Erfan:
#define LEN(x) sizeof(x) / sizeof(x[0])
#define printLine printf("Line: %d (%s)\n", __LINE__, __FILE__);
#define VARi(x)         \
  {                     \
    printf(#x);         \
    printf("=%d\n", x); \
  }  // to print int values
#define VARd(x)         \
  {                     \
    printf(#x);         \
    printf("=%f\n", x); \
  }  // to print double values
#define ERR(x) printf("ERROR: %s\n", x);
#define UNUSED(x) (void)(x)
typedef std::size_t sint;  // for loops on index for unsinged size
//:Erfan

#endif
