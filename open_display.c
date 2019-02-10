#include <stdio.h>

#include <X11/Xlib.h>

extern char **argvec;

Display *open_display(disp)
char *disp;
{
 Display *rv;

 rv = XOpenDisplay(disp);
 if (rv == 0)
  { fprintf(stderr,"%s: can't open display %s\n",argvec[0],XDisplayName(disp));
    exit(1);
  }
 return(rv);
}
