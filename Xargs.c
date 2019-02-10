#include <ctype.h>

#include <malloc.h>

#include <Xargs.h>

readargs(acp,avp,args)
int *acp;
char ***avp;
ARGDESC *args;
{
 int ac;
 char **av;
 char *zapit;
 int i;
 int j;
 int rest;
 char ***restvp;

 ac = *acp;
 av = *avp;
 zapit = (char *) malloc(ac*sizeof(char));
 for (i=0;i<ac;i++)
  { zapit[i] = 0;
  }
 for (i=0;args[i].flag;i++)
  { if (args[i].present) *args[i].present = 0;
  }
 rest = -1;
 for (i=0;(i<ac)&&(rest<0);i++)
  { if ((i < ac) && (av[i][0] == '-'))
     { for (j=0;args[j].flag;j++)
	{ if (!strcmp(args[j].flag,&av[i][1])) break;
	}
       if (args[j].flag)
	{ switch (args[j].type)
	   { case ARGTYPE_NONE:
		zapit[i] = 1;
		if (args[j].present) (*args[j].present) ++;
		continue;
		break;
	     case ARGTYPE_STRING:
		zapit[i] = 1;
		if (i+1 < ac)
		 { zapit[i+1] = 1;
		   if (args[j].present) (*args[j].present) ++;
		   *args[j].string = &av[i+1][0];
		   i ++;
		 }
		continue;
		break;
	     case ARGTYPE_REST:
		zapit[i] = 1;
		if (args[j].present) (*args[j].present) ++;
		rest = i + 1;
		restvp = args[j].stringvec;
		break;
	     case ARGTYPE_INT:
	     case ARGTYPE_HEXINT:
		zapit[i] = 1;
		if (i+1 < ac)
		 { int radix;
		   int value;
		   char *cp;
		   int dv;
		   zapit[i+1] = 1;
		   switch(args[j].type)
		    { case ARGTYPE_INT:
			 radix = 10;
			 break;
		      case ARGTYPE_HEXINT:
			 radix = 16;
			 break;
		    }
		   cp = &av[i+1][0];
		   if ((cp[0] == '0') && cp[1] && cp[2])
		    { switch (cp[1])
		       { case '0': case '1': case '2': case '3':
			 case '4': case '5': case '6': case '7':
			    radix = 8;
			    break;
			 case 'b':
			    radix = 2;
			    cp += 2;
			    break;
			 case 'o':
			    radix = 8;
			    cp += 2;
			    break;
			 case 't':
			    radix = 10;
			    cp += 2;
			    break;
			 case 'x':
			    radix = 16;
			    cp += 2;
			    break;
		       }
		    }
		   value = 0;
		   for (;*cp;cp++)
		    { switch (*cp)
		       { case '0': dv = 0; break;
			 case '1': dv = 1; break;
			 case '2': dv = 2; break;
			 case '3': dv = 3; break;
			 case '4': dv = 4; break;
			 case '5': dv = 5; break;
			 case '6': dv = 6; break;
			 case '7': dv = 7; break;
			 case '8': dv = 8; break;
			 case '9': dv = 9; break;
			 case 'a': case 'A': dv = 10; break;
			 case 'b': case 'B': dv = 11; break;
			 case 'c': case 'C': dv = 12; break;
			 case 'd': case 'D': dv = 13; break;
			 case 'e': case 'E': dv = 14; break;
			 case 'f': case 'F': dv = 15; break;
			 default: dv = -1; break;
		       }
		      if ((dv < 0) || (dv >= radix)) break;
		      value = (value * radix) + dv;
		    }
		   if (!*cp)
		    { if (args[j].present) (*args[j].present) ++;
		      *args[j].int_ = value;
		    }
		   i ++;
		 }
		continue;
		break;
	     case ARGTYPE_DOUBLE:
		zapit[i] = 1;
		if (i+1 < ac)
		 { double v;
		   zapit[i+1] = 1;
		   if (sscanf(&av[i+1][0],"%lf",&v) == 1)
		    { if (args[j].present) (*args[j].present) ++;
		      *args[j].double_ = v;
		    }
		   i ++;
		 }
		continue;
		break;
	   }
	}
     }
  }
 j = 0;
 for (i=0;i<ac;i++)
  { if (!zapit[i])
     { if (i != j) av[j] = av[i];
       j ++;
     }
  }
 av[j] = 0;
 free((char *)zapit);
 if (rest >= 0)
  { *restvp = av + rest - (i - j);
  }
 *acp = j;
}

static int number(cpp)
char **cpp;
{
 register char *cp;
 int n;

 cp = *cpp;
 n = 0;
 for (;*cp&&isascii(*cp)&&isdigit(*cp);cp++)
  { n = (n * 10) + (*cp - '0');
  }
 if (cp == *cpp) return(-1);
 *cpp = cp;
 return(n);
}
