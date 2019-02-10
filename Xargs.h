#ifndef _ARGS_H_bc00dd80_
#define _ARGS_H_bc00dd80_

typedef struct {
	  char *flag;
	  int *present;
	  char type;
#define ARGTYPE_NONE   0
#define ARGTYPE_STRING 1
#define ARGTYPE_REST   2
#define ARGTYPE_INT    3
#define ARGTYPE_HEXINT 4
#define ARGTYPE_DOUBLE 5
	  /* ghaa.  really want a union here. */
	  char **string;
	  char ***stringvec;
	  int *int_; /* this isn't PL/I, folks */
	  double *double_;
	  } ARGDESC;

#define ARGDESC_NONE()      ARGTYPE_NONE, 0, 0, 0, 0
#define ARGDESC_STRING(loc) ARGTYPE_STRING, (loc), 0, 0, 0
#define ARGDESC_REST(loc)   ARGTYPE_REST, 0, (loc), 0, 0
#define ARGDESC_INT(loc)    ARGTYPE_INT, 0, 0, (loc), 0
#define ARGDESC_HEXINT(loc) ARGTYPE_HEXINT, 0, 0, (loc), 0
#define ARGDESC_DOUBLE(loc) ARGTYPE_DOUBLE, 0, 0, 0, (loc)

#endif
