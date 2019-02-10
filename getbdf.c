#include <stdio.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <malloc.h>

#include <Xargs.h>
#include <mouseX.h>

char **argvec;

char *getenv();

char *displayname;
char *fontname;

ARGDESC args[] = { { "display",      0,        ARGDESC_STRING(&displayname)  },
		   { "fn",           0,        ARGDESC_STRING(&fontname)     },
		   { "font",         0,        ARGDESC_STRING(&fontname)     },
		   { 0 } };

int argc;
char **argv;

Display *disp;
int scrno;
Screen *scr;
Window rootwin;
Visual *visual;

XFontStruct *font;

nint(x) double x;
{
    int i;

    i = x;
    return i;
}

saveargv(ac,av)
int ac;
char **av;
{
 int i;
 int nc;
 char *abuf;

 argc = ac;
 argv = (char **) malloc((ac+1)*sizeof(char *));
 nc = 1;
 for (i=0;i<ac;i++)
  { nc += strlen(av[i]) + 1;
  }
 abuf = (char *) malloc(nc*sizeof(char));
 for (i=0;i<ac;i++)
  { argv[i] = abuf;
    strcpy(abuf,av[i]);
    while (*abuf) abuf ++;
    abuf ++;
  }
 *abuf = '\0';
 argv[ac] = 0;
}

maybeset(strp,str)
char **strp;
char *str;
{
 if (str && !*strp) *strp = str;
}

setup_font()
{
 font = XLoadQueryFont(disp,fontname);
 if (!font)
  { fprintf(stderr,"%s: can't load font %s\n",argvec[0],fontname);
    exit(1);
  }
}

unsigned long int fontprop(name,desc,def)
Atom name;
char *desc;
unsigned long int def;
{
 int i;

 for (i=font->n_properties-1;i>=0;i--)
  { if (font->properties[i].name == name)
     { return(font->properties[i].card32);
     }
  }
 fprintf(stderr,"%s: no %s property found on %s, assuming %lu\n",argvec[0],desc,fontname,def);
 return(def);
}

print_bdf_string(s)
char *s;
{
 putchar('"');
 for (;*s;s++)
  { if (*s == '"') putchar('"');
    putchar(*s);
  }
 putchar('"');
}

do_convert()
{
 int i;
 int j;
 int b1;
 int b2;
 int b;
 int ptsize;
 int xdpi;
 int ydpi;
 int pmw;
 int pmh;
 char *bytes;
 XImage *img;
 Pixmap buf;
 GC cleargc;
 GC drawgc;
 int nchars;
#ifndef XA_POINT_SIZE
 int XA_POINT_SIZE = XInternAtom(disp,"POINT_SIZE",0);
#endif
#ifndef XA_RESOLUTION_X
 int XA_RESOLUTION_X = XInternAtom(disp,"RESOLUTION_X",0);
#endif
#ifndef XA_RESOLUTION_Y
 int XA_RESOLUTION_Y = XInternAtom(disp,"RESOLUTION_Y",0);
#endif

 printf("STARTFONT 2.1\n");
 printf("COMMENT Generated from X font `%s' by getbdf\n",fontname);
 printf("FONT %s\n",fontname);
 ptsize = nint(fontprop(XA_POINT_SIZE,"point size",(unsigned long int)100)/10.0);
 xdpi = fontprop(XA_RESOLUTION_X,"X resolution",(unsigned long int)nint(XDisplayWidth(disp,scrno)*25.4/XDisplayWidthMM(disp,scrno)));
 ydpi = fontprop(XA_RESOLUTION_Y,"Y resolution",(unsigned long int)nint(XDisplayHeight(disp,scrno)*25.4/XDisplayHeightMM(disp,scrno)));
 printf("SIZE %d %d %d\n",ptsize,xdpi,ydpi);
 printf("FONTBOUNDINGBOX %d %d %d %d\n",
	font->max_bounds.rbearing - font->min_bounds.lbearing,
	font->max_bounds.ascent + font->max_bounds.descent,
	font->min_bounds.lbearing,
	-font->max_bounds.descent );
 printf("STARTPROPERTIES %d\n",font->n_properties+3);
 printf("FONT_ASCENT %d\nFONT_DESCENT %d\nDEFAULT_CHAR %u\n",font->ascent,font->descent,font->default_char);
 for (i=0;i<font->n_properties;i++)
  { char *s;
    Atom format;
    static int mustinit = 1;
    static struct {
	     char *name;
	     Atom format;
	     Atom atom;
	     } proptbl[] = { { "ADD_STYLE_NAME", XA_ATOM },
			     { "CHARSET_COLLECTIONS", XA_ATOM },
			     { "CHARSET_ENCODING", XA_ATOM },
			     { "CHARSET_REGISTRY", XA_ATOM },
			     { "COPYRIGHT", XA_ATOM },
			     { "DEVICE_FONT_NAME", XA_ATOM },
			     { "FACE_NAME", XA_ATOM },
			     { "FAMILY_NAME", XA_ATOM },
			     { "FONT", XA_ATOM },
			     { "FONTNAME_REGISTRY", XA_ATOM },
			     { "FOUNDRY", XA_ATOM },
			     { "FULL_NAME", XA_ATOM },
			     { "NOTICE", XA_ATOM },
			     { "SETWIDTH_NAME", XA_ATOM },
			     { "SLANT", XA_ATOM },
			     { "SPACING", XA_ATOM },
			     { "WEIGHT_NAME", XA_ATOM },
			     { "AVERAGE_WIDTH", XA_CARDINAL },
			     { "CAP_HEIGHT", XA_CARDINAL },
			     { "DESTINATION", XA_CARDINAL },
			     { "END_SPACE", XA_CARDINAL },
			     { "MAX_SPACE", XA_CARDINAL },
			     { "MIN_SPACE", XA_CARDINAL },
			     { "NORM_SPACE", XA_CARDINAL },
			     { "PIXEL_SIZE", XA_CARDINAL },
			     { "POINT_SIZE", XA_CARDINAL },
			     { "RELATIVE_SETWIDTH", XA_CARDINAL },
			     { "RELATIVE_WEIGHT", XA_CARDINAL },
			     { "RESOLUTION", XA_CARDINAL },
			     { "RESOLUTION_X", XA_CARDINAL },
			     { "RESOLUTION_Y", XA_CARDINAL },
			     { "SMALL_CAP_SIZE", XA_CARDINAL },
			     { "SUBSCRIPT_SIZE", XA_CARDINAL },
			     { "SUPERSCRIPT_SIZE", XA_CARDINAL },
			     { "UNDERLINE_THICKNESS", XA_CARDINAL },
			     { "WEIGHT", XA_CARDINAL },
			     { "X_HEIGHT", XA_CARDINAL },
			     { "AVG_CAPITAL_WIDTH", XA_INTEGER },
			     { "AVG_LOWERCASE_WIDTH", XA_INTEGER },
			     { "FIGURE_WIDTH", XA_INTEGER },
			     { "ITALIC_ANGLE", XA_INTEGER },
			     { "QUAD_WIDTH", XA_INTEGER },
			     { "STRIKEOUT_ASCENT", XA_INTEGER },
			     { "STRIKEOUT_DESCENT", XA_INTEGER },
			     { "SUBSCRIPT_X", XA_INTEGER },
			     { "SUBSCRIPT_Y", XA_INTEGER },
			     { "SUPERSCRIPT_X", XA_INTEGER },
			     { "SUPERSCRIPT_Y", XA_INTEGER },
			     { "UNDERLINE_POSITION", XA_INTEGER },
			     { 0 } };
    if (mustinit)
     { mustinit = 0;
       for (j=0;proptbl[j].name;j++) proptbl[j].atom = XInternAtom(disp,proptbl[j].name,0);
     }
    s = XGetAtomName(disp,font->properties[i].name);
    printf("%s ",s);
    format = XA_CARDINAL;
    for (j=0;proptbl[j].name;j++) if (font->properties[i].name == proptbl[j].atom) format = proptbl[j].format;
    switch (format)
     { case XA_ATOM:
	  print_bdf_string(XGetAtomName(disp,(Atom)font->properties[i].card32));
	  printf("\n");
	  break;
       case XA_CARDINAL:
	  printf("%lu\n",font->properties[i].card32);
	  break;
       case XA_INTEGER:
	  printf("%ld\n",(long int)font->properties[i].card32);
	  break;
     }
  }
 printf("ENDPROPERTIES\n");
 if (font->per_char)
  { nchars = 0;
    for (b1=font->min_byte1;b1<=font->max_byte1;b1++)
     { b = (b1-font->min_byte1) * (font->max_char_or_byte2+1-font->min_char_or_byte2);
       for (b2=font->min_char_or_byte2;b2<=font->max_char_or_byte2;b2++)
	{ XCharStruct *chr;
	  chr = font->per_char + b;
	  if (chr->lbearing || chr->rbearing || chr->ascent || chr->descent || chr->width) nchars ++;
	  b ++;
	}
     }
  }
 else
  { nchars = (font->max_byte1+1-font->min_byte1) * (font->max_char_or_byte2+1-font->min_char_or_byte2);
  }
 printf("CHARS %d\n",nchars);
 pmw = ((font->max_bounds.rbearing - font->min_bounds.lbearing) + 7) & ~7;
 pmh = font->max_bounds.ascent + font->max_bounds.descent;
 buf = XCreatePixmap(disp,rootwin,pmw,pmh,1);
 cleargc = XCreateGC(disp,(Drawable)buf,0L,(XGCValues *)0);
 drawgc = XCreateGC(disp,(Drawable)buf,0L,(XGCValues *)0);
 XSetForeground(disp,cleargc,0L);
 XSetForeground(disp,drawgc,1L);
 XSetBackground(disp,drawgc,0L);
 XSetFont(disp,drawgc,font->fid);
 for (b1=font->min_byte1;b1<=font->max_byte1;b1++)
  { b = (b1-font->min_byte1) * (font->max_char_or_byte2+1-font->min_char_or_byte2);
    for (b2=font->min_char_or_byte2;b2<=font->max_char_or_byte2;b2++)
     { XCharStruct *chr;
       XTextItem16 ti;
       XChar2b c2b;
       int cw;
       int ch;
       int x;
       int y;
       int h;
       chr = font->per_char ? (font->per_char + b) : &font->min_bounds;
       if (chr->lbearing || chr->rbearing || chr->ascent || chr->descent || chr->width)
	{ cw = chr->rbearing - chr->lbearing;
	  ch = chr->ascent + chr->descent;
	  printf("STARTCHAR %d\n",b);
	  printf("ENCODING %d\n",(b1<<8)+b2);
	  printf("SWIDTH %d 0\n",nint((chr->width*72000.0)/(ptsize*xdpi)));
	  printf("DWIDTH %d 0\n",chr->width);
	  printf("BBX %d %d %d %d\n",cw,ch,chr->lbearing,-chr->descent);
	  printf("BITMAP\n");
	  XFillRectangle(disp,(Drawable)buf,cleargc,0,0,pmw,pmh);
	  c2b.byte1 = (b1 == 0) ? (b2 >> 8) : b1;
	  c2b.byte2 = b2;
	  ti.chars = &c2b;
	  ti.nchars = 1;
	  ti.delta = 0;
	  ti.font = None;
	  XDrawText16(disp,(Drawable)buf,drawgc,-chr->lbearing,chr->ascent,&ti,1);
	  img = XGetImage(disp,(Drawable)buf,0,0,pmw,pmh,1L,XYPixmap);
	  for (y=0;y<ch;y++)
	   { for (x=0;x<cw;)
	      { h = 0;
		for (i=0;i<8;i++)
		 { h <<= 1;
		   if (XGetPixel(img,x,y)) h |= 1;
		   x ++;
		 }
		printf("%02x",h);
	      }
	     putchar('\n');
	   }
	  printf("ENDCHAR\n");
	  XDestroyImage(img);
	}
       b ++;
     }
  }
 printf("ENDFONT\n");
/*
 XCharStruct: short lbearing, rbearing, width, ascent, descent, unsigned short attributes
 XFontProp: Atom name, unsigned long card32
 XChar2b: unsigned char byte1, byte2
 XFontStruct: XExtData *ext_data, Font fid, unsigned direction, min_char_or_byte2, max_char_or_byte2,
	unsigned min_byte1, max_byte1, Bool all_chars_exist, unsigned default_char, int n_properties,
	XFontProp *properties, XCharStruct min_bounds, max_bounds, *per_char, int ascent, descent

XFontStruct:
 if min_byte1 and max_byte1 both zero, min_char_or_byte2 is first index for per_char; max_c_o_b2 last.
 if min_byte1 or min_byte2 nonzero, min_c_o_b2 and max_c_o_b2 both <256, and 2-byte indices for
	per_char[N] are byte1=(N/D)+min_byte1, byte2=(N\D)+min_c_o_b2 (\ is modulus)
	where D = max_c_o_b2 - min_c_o_b2 + 1
 if per_char is 0, each char has same info, found in both min_bounds and max_bounds.
 if all_chars_exist is True, all chars in per_char have nonzero bounding boxes.
 default_char is char to be used when undef'd or nonexistent char is printed.  def_char is 16-bit char
	(not 2-byte char).  if font uses 2-byte matrix format, default_char is (byte1<<8)+byte2.
	if default_char itself is undefined or nonexistent, no printing is performed for undef'd/nx chars.
 min_bounds and max_bounds contain most extreme values of each XCharStruct element over all els of array
	(ignoring nonexistent chars).  bounding box of font has UL corner at
	(x+min_bounds.lbearing,y-max_bounds.ascent), width max_bounds.rbearing-min_bounds.lbearing,
	and height max_bounds.ascent+max_bounds.descent.
 ascent is logical extent above baseline, for line spacing.  not guaranteed >= max_bounds.ascent.
 descent is logical extent below baseline, for line spacing.  not guaranteed >= max_bounds.descent.
 baseline at y, logical extent is [y-font.ascent .. y+font.descent-1].

XCharStruct:
 for char at (x,y), bounding box of char has UL corner at (x+lbearing,y-ascent),
	width rbearing-lbearing, and height ascent+descent.  next char origin is (x+width,y).
 lbearing, rbearing, ascent, descent box the ink.  baseline is scanline just below nondescending chars.
 attribues is not defined by protocol.
 nonexistent char: all members set to zero.
*/
}

main(ac,av)
int ac;
char **av;
{
 argvec=av;
 saveargv(ac,av);
 readargs(&ac,&av,&args[0]);
 if (! fontname)
  { fprintf(stderr,"%s: must specify a font with -font\n",argvec[0]);
    exit(1);
  }
 disp = open_display(displayname);
 scr = XDefaultScreenOfDisplay(disp);
#ifdef HP
 scrno = 0;
#else
 scrno = XScreenNumberOfScreen(scr);
#endif
 rootwin = XRootWindowOfScreen(scr);
 setup_font();
 do_convert();
 exit(0);
}
