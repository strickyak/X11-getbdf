OBJS=           getbdf.o Xargs.o open_display.o

# HP or SUN ?
#CFLAGS=        -g -DHP
CFLAGS=         -g


getbdf:         $(OBJS)
	               cc $(CFLAGS) $(OBJS) -o getbdf -lX11 -lm


getbdf.o:       getbdf.c
	               cc -c $(CFLAGS) getbdf.c -I. -I/usr/include/X11

Xargs.o:        Xargs.c
	               cc -c $(CFLAGS) Xargs.c -I. -I/usr/include/X11

open_display.o: open_display.c
	               cc -c $(CFLAGS) open_display.c -I. -I/usr/include/X11

clean:
	               rm -f $(OBJS) getbdf
