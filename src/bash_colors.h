#ifndef _BASH_COLORS_
#define _BASH_COLORS_

#define DEFAULT colors[0]
#define BLACK   colors[1]
#define RED     colors[2]
#define GREEN   colors[3]
#define YELLOW  colors[4]
#define BLUE    colors[5]
#define PURPLE  colors[6]
#define CYAN    colors[7]
#define WHITE   colors[8]

#define BOLD_BLACK   colors[9]
#define BOLD_RED     colors[10]
#define BOLD_GREEN   colors[11]
#define BOLD_YELLOW  colors[12]
#define BOLD_BLUE    colors[13]
#define BOLD_PURPLE  colors[14]
#define BOLD_CYAN    colors[15]
#define BOLD_WHITE   colors[16]

char *colors[17] = {
  "\033[0m",
  "\033[0;30m",
  "\033[0;31m",
  "\033[0;32m",
  "\033[0;33m",
  "\033[0;34m",
  "\033[0;35m",
  "\033[0;36m",
  "\033[0;37m",
  "\033[1;30m",
  "\033[1;31m",
  "\033[1;32m",
  "\033[1;33m",
  "\033[1;34m",
  "\033[1;35m",
  "\033[1;36m",
  "\033[1;37m"
};

#endif
