typedef union {
  double greal;     // generic double
  int gint;         // generic integer
  long glong;       // generic long
  char *gstr;	    // generic string
  ybOper tope;	    // operator
  ybExp *exp;	    // expression
  ybExpLst *explst; // list of expressions
  ybCom *com;	    // command
} YYSTYPE;
#define	RNUM	258
#define	EVAL	259
#define	BNUM	260
#define	LNUM	261
#define	ID	262
#define	DNUM	263
#define	SNUM	264
#define	NOTOP	265
#define	ADDOP	266
#define	MULOP	267
#define	RELOP	268
#define	EQUOP	269
#define	ANDOP	270
#define	OROP	271
#define	NL	272
#define	DET	273
#define	ALIAS	274
#define	BOTTOM	275
#define	DO	276
#define	ELSE	277
#define	ENDDO	278
#define	ENDIF	279
#define	GOTO	280
#define	IF	281
#define	SELECT	282
#define	SKIP	283
#define	TOP	284
#define	WHILE	285
#define	USE	286
#define	UNOP	287


extern YYSTYPE yylval;
