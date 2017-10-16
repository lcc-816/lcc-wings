%{
enum { MREG=1, MREG2, MREG3, M4, M5, M6 };
#include "c.h"
#define NODEPTR_TYPE Node
#define OP_LABEL(p) ((p)->op)
#define LEFT_CHILD(p) ((p)->kids[0])
#define RIGHT_CHILD(p) ((p)->kids[1])
#define STATE_LABEL(p) ((p)->x.state)
static void address(Symbol, Symbol, long);
static void blkfetch(int, int, int, int);
static void blkloop(int, int, int, int, int, int[]);
static void blkstore(int, int, int, int);
static void defaddress(Symbol);
static void defconst(int, int, Value);
static void defstring(int, char *);
static void defsymbol(Symbol);
static void doarg(Node);
static void emit2(Node);
static void export(Symbol);
static void clobber(Node);
static void function(Symbol, Symbol [], Symbol [], int);
static void global(Symbol);
static void import(Symbol);
static void local(Symbol);
static void progbeg(int, char **);
static void progend(void);
static void segment(int);
static void space(int);
static void target(Node);
extern int ckstack(Node, int);
extern int memop(Node);
extern int sametree(Node, Node);
static Symbol charreg[32], shortreg[32], intreg[32];
static Symbol fltreg[32];

static Symbol charregw, shortregw, intregw, fltregw;

static int cseg;

static int mreg;
static int mreg2;

static Symbol quo, rem;

static int shareseg();
static int segshare();
static int shsegs;

%}
%start stmt
%term CNSTF4=4113
%term CNSTF8=8209
%term CNSTI1=1045
%term CNSTI2=2069
%term CNSTI4=4117
%term CNSTP4=4119
%term CNSTU1=1046
%term CNSTU2=2070
%term CNSTU4=4118

%term ARGB=41
%term ARGF4=4129
%term ARGF8=8225
%term ARGI2=2085
%term ARGI4=4133
%term ARGP4=4135
%term ARGU2=2086
%term ARGU4=4134

%term ASGNB=57
%term ASGNF4=4145
%term ASGNF8=8241
%term ASGNI1=1077
%term ASGNI2=2101
%term ASGNI4=4149
%term ASGNP4=4151
%term ASGNU1=1078
%term ASGNU2=2102
%term ASGNU4=4150

%term INDIRB=73
%term INDIRF4=4161
%term INDIRF8=8257
%term INDIRI1=1093
%term INDIRI2=2117
%term INDIRI4=4165
%term INDIRP4=4167
%term INDIRU1=1094
%term INDIRU2=2118
%term INDIRU4=4166

%term CVFF4=4209
%term CVFF8=8305
%term CVFI2=2165
%term CVFI4=4213

%term CVIF4=4225
%term CVIF8=8321
%term CVII1=1157
%term CVII2=2181
%term CVII4=4229
%term CVIU1=1158
%term CVIU2=2182
%term CVIU4=4230

%term CVPU4=4246

%term CVUI1=1205
%term CVUI2=2229
%term CVUI4=4277
%term CVUP4=4279
%term CVUU1=1206
%term CVUU2=2230
%term CVUU4=4278

%term NEGF4=4289
%term NEGF8=8385
%term NEGI2=2245
%term NEGI4=4293

%term CALLB=217
%term CALLF4=4305
%term CALLF8=8401
%term CALLI2=2261
%term CALLI4=4309
%term CALLP4=4311
%term CALLU2=2262
%term CALLU4=4310
%term CALLV=216

%term RETF4=4337
%term RETF8=8433
%term RETI2=2293
%term RETI4=4341
%term RETP4=4343
%term RETU2=2294
%term RETU4=4342
%term RETV=248

%term ADDRGP4=4359

%term ADDRFP4=4375

%term ADDRLP4=4391

%term ADDF4=4401
%term ADDF8=8497
%term ADDI2=2357
%term ADDI4=4405
%term ADDP4=4407
%term ADDU2=2358
%term ADDU4=4406

%term SUBF4=4417
%term SUBF8=8513
%term SUBI2=2373
%term SUBI4=4421
%term SUBP4=4423
%term SUBU2=2374
%term SUBU4=4422

%term LSHI2=2389
%term LSHI4=4437
%term LSHU2=2390
%term LSHU4=4438

%term MODI2=2405
%term MODI4=4453
%term MODU2=2406
%term MODU4=4454

%term RSHI2=2421
%term RSHI4=4469
%term RSHU2=2422
%term RSHU4=4470

%term BANDI2=2437
%term BANDI4=4485
%term BANDU2=2438
%term BANDU4=4486

%term BCOMI2=2453
%term BCOMI4=4501
%term BCOMU2=2454
%term BCOMU4=4502

%term BORI2=2469
%term BORI4=4517
%term BORU2=2470
%term BORU4=4518

%term BXORI2=2485
%term BXORI4=4533
%term BXORU2=2486
%term BXORU4=4534

%term DIVF4=4545
%term DIVF8=8641
%term DIVI2=2501
%term DIVI4=4549
%term DIVU2=2502
%term DIVU4=4550

%term MULF4=4561
%term MULF8=8657
%term MULI2=2517
%term MULI4=4565
%term MULU2=2518
%term MULU4=4566

%term EQF4=4577
%term EQF8=8673
%term EQI2=2533
%term EQI4=4581
%term EQU2=2534
%term EQU4=4582

%term GEF4=4593
%term GEF8=8689
%term GEI2=2549
%term GEI4=4597
%term GEU2=2550
%term GEU4=4598

%term GTF4=4609
%term GTF8=8705
%term GTI2=2565
%term GTI4=4613
%term GTU2=2566
%term GTU4=4614

%term LEF4=4625
%term LEF8=8721
%term LEI2=2581
%term LEI4=4629
%term LEU2=2582
%term LEU4=4630

%term LTF4=4641
%term LTF8=8737
%term LTI2=2597
%term LTI4=4645
%term LTU2=2598
%term LTU4=4646

%term NEF4=4657
%term NEF8=8753
%term NEI2=2613
%term NEI4=4661
%term NEU2=2614
%term NEU4=4662

%term JUMPV=584

%term LABELV=600

%term LOADB=233
%term LOADF4=4321
%term LOADF8=8417
%term LOADF16=16609
%term LOADI1=1253
%term LOADI2=2277
%term LOADI4=4325
%term LOADI8=8421
%term LOADP4=4327
%term LOADP8=8423
%term LOADU1=1254
%term LOADU2=2278
%term LOADU4=4326
%term LOADU8=8422

%term VREGP=711
%%
con:	CNSTU4		"%a"
con:	CNSTI4		"%a"
con:	CNSTP4		"%a"
con:	CNSTI1		"%a"
con:	CNSTU1		"%a"
con:	CNSTI2		"%a"
con:	CNSTU2		"%a"

con1:	CNSTU2		"1"	range(a, 1, 1)
con1:	CNSTI2		"1"	range(a, 1, 1)
con1:	CNSTP4		"1"	range(a, 1, 1)
con1:	CNSTU4		"1"	range(a, 1, 1)
con1:	CNSTI4		"1"	range(a, 1, 1)
con0:	CNSTU4		"0"	range(a, 0, 0)
con0:	CNSTI4		"0"	range(a, 0, 0)
con0:	CNSTP4		"0"	range(a, 0, 0)
con0:	CNSTU2		"0"	range(a, 0, 0)
con0:	CNSTI2		"0"	range(a, 0, 0)

con16b:	CNSTU4		"%a"	range(a, 0, 65535)
con16b:	CNSTI4		"%a"	range(a, 0, 65535)
con16b:	CNSTP4		"%a"	range(a, 0, 65535)
con16b:	CNSTU2		"%a"	range(a, 0, 65535)
con16b:	CNSTI2		"%a"	range(a, 0, 65535)


loc:	ADDRLP4		"%a"
glob:	ADDRGP4		"%a" shareseg(a)
arg:	ADDRFP4		"%a"
tglob:	ADDRGP4		"%a"

var:	loc		"%0"
var:	glob		"%0"
var:	arg		"%0"
varp:	loc		"%0"
varp:	arg		"%0"

acon:	con		"%0"
vc:	acon		"%0"
vc:	glob		"%0"
vc:	LOADU4(vc)	"%0"

omem4:	INDIRU4(var)	"%0"
omem4:	INDIRI4(var)	"%0"
omem4:	INDIRP4(var)	"%0"
omem4:	LOADU4(omem4)	"%0"
omem4:	LOADI4(omem4)	"%0"

omem2:	INDIRU2(var)	"%0"
omem2:	INDIRI2(var)	"%0"
omem2:	LOADU2(omem4)	"%0"
omem2:	LOADI2(omem4)	"%0"
omem2:	LOADU2(omem2)	"%0"
omem2:	LOADI2(omem2)	"%0"

omem1:	INDIRI1(var)	"%0"
omem1:	INDIRU1(var)	"%0"

mem2:	omem2		"%0"
mem2:	amreg2		"mreg2" 7
mem4:	omem4		"%0"
mem4:	mreg2		"mreg2" 7
mem41:	omem4		"%0"
mem41:	mreg		"mreg" 7

omem16:	CVUU4(omem2)	"%0"

memp:	INDIRI1(varp)	"%0"
memp:	INDIRU1(varp)	"%0"
memp:	INDIRU2(varp)	"%0"
memp:	INDIRI2(varp)	"%0"
memp:	INDIRU4(varp)	"%0"
memp:	INDIRI4(varp)	"%0"
memp:	INDIRP4(varp)	"%0"

poiad:	memp		"%0"
poiad:	mreg		"mreg"

poiad2:	memp		"%0"
poiad2: mreg2		"mreg2"

opoi1:	INDIRI1(memp)	"%0"
opoi1:	INDIRU1(memp)	"%0"
poi1:	INDIRI1(poiad2)	"%0"
poi1:	INDIRU1(poiad2)	"%0"
opoi2:	INDIRI2(memp)	"%0"
opoi2:	INDIRU2(memp)	"%0"
poi2:	INDIRI2(poiad2)	"%0"
poi2:	INDIRU2(poiad2)	"%0"
poi4:	INDIRI4(poiad2)	"%0"
poi4:	INDIRU4(poiad2)	"%0"
poi4:	INDIRP4(poiad2)	"%0"
opoi4:	INDIRI4(memp)	"%0"
opoi4:	INDIRU4(memp)	"%0"
poi41:	INDIRI4(poiad)	"%0"
poi41:	INDIRU4(poiad)	"%0"
poi41:	INDIRP4(poiad)	"%0"

call2:	CALLI2(tglob)	"\tjsr @%0\n\t!POP %a\n"
call2:	CALLI2(xareg)	"\tjsr @__callxa\n\t!POP %a\n" 6
call2:	CALLU2(tglob)	"\tjsr @%0\n\t!POP %a\n"
call2:	CALLU2(xareg)	"\tjsr @__callxa\n\t!POP %a\n" 6

call4:	CALLI4(tglob)	"\tjsr @%0\n\t!POP %a\n"
call4:	CALLI4(xareg)	"\tjsr @__callxa\n\t!POP %a\n" 6
call4:	CALLU4(tglob)	"\tjsr @%0\n\t!POP %a\n"
call4:	CALLU4(xareg)	"\tjsr @__callxa\n\t!POP %a\n" 6
call4:	CALLP4(tglob)	"\tjsr @%0\n\t!POP %a\n"
call4:	CALLP4(xareg)	"\tjsr @__callxa\n\t!POP %a\n" 6

callv:	CALLV(tglob)	"\tjsr @%0\n\t!POP %a\n"
callv:	CALLV(xareg)	"\tjsr @__callxa\n\t!POP %a\n" 6

xareg:	CVUU4(xareg)	""
xareg:	CVUP4(xareg)	""

xareg:	LOADI4(xareg)	""
xareg:	LOADU4(xareg)	""
xareg:	LOADP4(xareg)	""

xareg:	LSHI4(xareg,omem4) "\t.(\n\tstx mreg2\n\tldy %1\n\tbeq skip\nblah\tasl mreg2\n\trol\n\tdey\n\tbne blah\nskip\tldx mreg2\n\t.)\n"
xareg:	LSHI4(xareg,omem2) "\t.(\n\tstx mreg2\n\tldy %1\n\tbeq skip\nblah\tasl mreg2\n\trol\n\tdey\n\tbne blah\nskip\tldx mreg2\n\t.)\n"
xareg:	LSHI4(xareg,vc) "\t!LSH32 %1,mreg2\n" 5
xareg:	LSHI4(xareg,con1) "\ttay\n\ttxa\n\tasl\n\ttax\n\ttya\n\trol\n" 3
xareg:	LSHI4(CVII4(mem2),con1) "\tlda %0\n\tasl\n\ttax\n\tlda #0\n\trol\n"
xareg:	LSHI4(xastk,areg) "\t.(\n\ttay\n\tpla\n\tsta mreg2\n\tpla\n\tcpy #0\n\tbeq skip\n\tblah\tasl mreg2\n\trol\n\tdey\n\tbne blah\nskip\tldx mreg2\n\t.)\n" 7

xareg:	LSHU4(xareg,omem4) "\t.(\n\tstx mreg2\n\tldy %1\n\tbeq skip\nblah\tasl mreg2\n\trol\n\tdey\n\tbne blah\nskip\tldx mreg2\n\t.)\n"
xareg:	LSHU4(xareg,omem2) "\t.(\n\tstx mreg2\n\tldy %1\n\tbeq skip\nblah\tasl mreg2\n\trol\n\tdey\n\tbne blah\nskip\tldx mreg2\n\t.)\n"
xareg:	LSHU4(xareg,vc) "\t!LSH32 %1,mreg2\n" 5
xareg:	LSHU4(xareg,con1) "\ttay\n\ttxa\n\tasl\n\ttax\n\ttya\n\trol\n" 3
xareg:	LSHU4(CVII4(mem2),con1) "\tlda %0\n\tasl\n\ttax\n\tlda #0\n\trol\n"
xareg:	LSHU4(xastk,areg) "\t.(\n\ttay\n\tpla\n\tsta mreg2\n\tpla\n\tcpy #0\n\tbeq skip\n\tblah\tasl mreg2\n\trol\n\tdey\n\tbne blah\nskip\tldx mreg2\n\t.)\n" 7

xareg:	RSHI4(xareg,omem4) "\t.(\n\tstx mreg2\n\tldy %1\n\tbeq skip\nblah\tlsr\n\tror mreg2\n\tdey\n\tbne blah\nskip\tldx mreg2\n\t.)\n"
xareg:	RSHI4(xareg,omem2) "\t.(\n\tstx mreg2\n\tldy %1\n\tbeq skip\nblah\tlsr\n\tror mreg2\n\tdey\n\tbne blah\nskip\tldx mreg2\n\t.)\n"
xareg:	RSHI4(xareg,vc) "\t!RSH32 %1,mreg2\n" 5
xareg:	RSHI4(xastk,areg) "\t.(\n\ttay\n\tpla\n\tsta mreg2\n\tpla\n\tcpy #0\n\tbeq skip\nblah\tlsr\n\tror mreg2\n\tdey\n\tbne blah\nskip\tldx mreg2\n\t.)\n" 7

xareg:	RSHU4(xareg,omem4) "\t.(\n\tstx mreg2\n\tldy %1\n\tbeq skip\nblah\tlsr\n\tror mreg2\n\tdey\n\tbne blah\nskip\tldx mreg2\n\t.)\n"
xareg:	RSHU4(xareg,vc) "\t!RSH32 %1,mreg2\n" 5
xareg:	RSHU4(xareg,omem2) "\t.(\n\tstx mreg2\n\tldy %1\n\tbeq skip\nblah\tlsr\n\tror mreg2\n\tdey\n\tbne blah\nskip\tldx mreg2\n\t.)\n"
xareg:	RSHU4(xastk,areg) "\t.(\n\ttay\n\tpla\n\tsta mreg2\n\tpla\n\tcpy #0\n\tbeq skip\nblah\tlsr\n\tror mreg2\n\tdey\n\tbne blah\nskip\tldx mreg2\n\t.)\n" 7

xareg:	MULI4(mreg2,xalod) "\tjsr @__muli32\n"
xareg:	MULI4(xastk,xareg) "\tply\n\tsty mreg2\n\tply\n\tsty mreg2+2\n\tjsr @__muli32\n" 5
xareg:	MULU4(mreg2,xalod) "\tjsr @__muli32\n"
xareg:	MULU4(xastk,xareg) "\tply\n\tsty mreg2\n\tply\n\tsty mreg2+2\n\tjsr @__muli32\n" 5

xareg:	DIVI4(mreg2,xalod) "\tjsr @__divi32\n"
xareg:	DIVI4(xastk,xareg) "\tply\n\tsty mreg2\n\tply\n\tsty mreg2+2\n\tjsr @__divi32\n" 5
xareg:	DIVU4(mreg2,xalod) "\tjsr @__divu32\n"
xareg:	DIVU4(xastk,xareg) "\tply\n\tsty mreg2\n\tply\n\tsty mreg2+2\n\tjsr @__divu32\n" 5

xareg:	MODI4(mreg2,xalod) "\tjsr @__modi32\n"
xareg:	MODI4(xastk,xareg) "\tply\n\tsty mreg2\n\tply\n\tsty mreg2+2\n\tjsr @__modi32\n" 5
xareg:	MODU4(mreg2,xalod) "\tjsr @__modu32\n"
xareg:	MODU4(xastk,xareg) "\tply\n\tsty mreg2\n\tply\n\tsty mreg2+2\n\tjsr @__modu32\n" 5

xareg:	ADDI4(poi4,vc)	"\tclc\n\tlda [%0]\n\tadc #!%1\n\ttax\n\tldy #2\n\tlda [%0],y\n\tadc #^%1\n"
xareg:	ADDI4(mem4,vc)	"\tclc\n\tlda %0\n\tadc #!%1\n\ttax\n\tlda %0+2\n\tadc #^%1\n"
xareg:	ADDI4(xareg,vc) "\tclc\n\ttay\n\ttxa\n\tadc #!%1\n\ttax\n\ttya\n\tadc #^%1\n" 3
xareg:	ADDI4(xareg,con1) "\t.(\n\tinx\n\tbne skip\n\tinc\nskip\t.)\n" 
xareg:	ADDI4(mem4,omem4) "\tclc\n\tlda %0\n\tadc %1\n\ttax\n\tlda %0+2\n\tadc %1+2\n"
xareg:	ADDI4(mem4,omem16) "\tclc\n\tlda %0\n\tadc %1\n\ttax\n\tlda %0+2\n\tadc #0\n"
xareg:	ADDI4(omem4,xareg) "\tclc\n\ttay\n\ttxa\n\tadc %0\n\ttax\n\ttya\n\tadc %0+2\n"
xareg:	ADDI4(xastk,xareg) "\tclc\n\ttay\n\ttxa\n\tadc 1,s\n\ttax\n\ttya\n\tadc 3,s\n\tply\n\tply\n" 7

xareg:	ADDU4(mem4,vc)	"\tclc\n\tlda %0\n\tadc #!%1\n\ttax\n\tlda %0+2\n\tadc #^%1\n"
xareg:	ADDU4(poi4,vc)	"\tclc\n\tlda [%0]\n\tadc #!%1\n\ttax\n\tldy #2\n\tlda [%0],y\n\tadc #^%1\n"
xareg:	ADDU4(xareg,vc) "\tclc\n\ttay\n\ttxa\n\tadc #!%1\n\ttax\n\ttya\n\tadc #^%1\n" 3
xareg:	ADDU4(xareg,con1) "\t.(\n\tinx\n\tbne skip\n\tinc\nskip\t.)\n" 
xareg:	ADDU4(mem4,omem4) "\tclc\n\tlda %0\n\tadc %1\n\ttax\n\tlda %0+2\n\tadc %1+2\n"
xareg:	ADDU4(mem4,omem16) "\tclc\n\tlda %0\n\tadc %1\n\ttax\n\tlda %0+2\n\tadc #0\n"
xareg:	ADDU4(xareg,omem4) "\tclc\n\ttay\n\ttxa\n\tadc %1\n\ttax\n\ttya\n\tadc %1+2\n"
xareg:	ADDU4(xastk,xareg) "\tclc\n\ttay\n\ttxa\n\tadc 1,s\n\ttax\n\ttya\n\tadc 3,s\n\tply\n\tply\n" 7

xareg:	ADDP4(areg,vc)	"\tclc\n\tadc #!%1\n\ttax\n\tlda #^%1\n\tadc #0\n"
xareg:	ADDP4(areg,omem4) "\tclc\n\tadc %1\n\ttax\n\tlda %1+2\n\tadc #0\n"
xareg:	ADDP4(areg,oyoff32) "\tclc\n\tadc [%1],y\n\ttax\n\tiny\n\tiny\n\tlda [%1],y\n\tadc #0\n"
xareg:	ADDP4(stack,xareg) "\tclc\n\tpha\n\ttxa\n\tadc 3,s\n\ttax\n\tpla\n\tadc #0\n\tply\n"
xareg:	ADDP4(mem4,vc)	"\tclc\n\tlda %0\n\tadc #!%1\n\ttax\n\tlda %0+2\n\tadc #^%1\n"
xareg:	ADDP4(mem4,omem4) "\tclc\n\tlda %0\n\tadc %1\n\ttax\n\tlda %0+2\n\tadc %1+2\n"
xareg:	ADDP4(mem4,omem16) "\tclc\n\tlda %0\n\tadc %1\n\ttax\n\tlda %0+2\n\tadc #0\n"
xareg:	ADDP4(omem4,xareg) "\tclc\n\ttay\n\ttxa\n\tadc %0\n\ttax\n\ttya\n\tadc %0+2\n"
xareg:	ADDP4(poi4,vc)	"\tclc\n\tlda [%0]\n\tadc #!%1\n\ttax\n\tldy #2\n\tlda [%0],y\n\tadc #^%1\n"
xareg:	ADDP4(xareg,vc) "\tclc\n\ttay\n\ttxa\n\tadc #!%1\n\ttax\n\ttya\n\tadc #^%1\n" 3
xareg:	ADDP4(xareg,con1) "\t.(\n\tinx\n\tbne skip\n\tinc\nskip\t.)\n" 
xareg:	ADDP4(xareg,omem4) "\tclc\n\ttay\n\ttxa\n\tadc %1\n\ttax\n\ttya\n\tadc %1+2\n"
xareg:	ADDP4(xastk,xareg) "\tclc\n\ttay\n\ttxa\n\tadc 1,s\n\ttax\n\ttya\n\tadc 3,s\n\tply\n\tply\n" 7

xareg:	SUBI4(mem4,vc)	"\tsec\n\tlda %0\n\tsbc #!%1\n\ttax\n\tlda %0+2\n\tsbc #^%1\n"
xareg:	SUBI4(mem4,omem4) "\tsec\n\tlda %0\n\tsbc %1\n\ttax\n\tlda %0+2\n\tsbc %1+2\n"
xareg:	SUBI4(omem4,xareg) "\tsec\n\tpha\n\tphx\n\tlda %0\n\tsbc 1,s\n\ttax\n\tply\n\tlda %0+2\n\tsbc 1,s\n\tply\n" 5
xareg:	SUBI4(xareg,vc) "\tsec\n\ttay\n\ttxa\n\tsbc #!%1\n\ttax\n\ttya\n\tsbc #^%1\n" 4
xareg:	SUBI4(xareg,oyoff32) "\tpha\n\tsec\n\ttxa\n\tsbc [%1],y\n\ttax\n\tpla\n\tiny\n\tiny\n\tsbc [%1],y\n" 
xareg:	SUBI4(xareg,omem4) "\tsec\n\ttay\n\ttxa\n\tsec\n\tsbc %1\n\ttax\n\ttya\n\tsbc %1+2\n" 5
xareg:	SUBI4(poi4,omem4) "\tsec\n\tlda [%0]\n\tsbc %1\n\ttax\n\tldy #2\n\tlda [%0],y\n\tsbc %1+2\n"
xareg:	SUBI4(xastk,mem4) "\tsec\n\tlda 1,s\n\tsbc %1\n\ttax\n\tlda 3,s\n\tsbc %1+2\n\tply\n\tply\n" 7

xareg:	SUBU4(mem4,vc)	"\tsec\n\tlda %0\n\tsbc #!%1\n\ttax\n\tlda %0+2\n\tsbc #^%1\n"
xareg:	SUBU4(mem4,omem4) "\tsec\n\tlda %0\n\tsbc %1\n\ttax\n\tlda %0+2\n\tsbc %1+2\n"
xareg:	SUBU4(omem4,xareg) "\tsec\n\tpha\n\tphx\n\tlda %0\n\tsbc 1,s\n\ttax\n\tply\n\tlda %0+2\n\tsbc 1,s\n\tply\n" 5
xareg:	SUBU4(xareg,vc) "\tsec\n\ttay\n\ttxa\n\tsbc #!%1\n\ttax\n\ttya\n\tsbc #^%1\n" 4
xareg:	SUBU4(xareg,oyoff32) "\tpha\n\tsec\n\ttxa\n\tsbc [%1],y\n\ttax\n\tpla\n\tiny\n\tiny\n\tsbc [%1],y\n" 
xareg:	SUBU4(xareg,omem4) "\tsec\n\ttay\n\ttxa\n\tsec\n\tsbc %1\n\ttax\n\ttya\n\tsbc %1+2\n" 5
xareg:	SUBU4(poi4,omem4) "\tsec\n\tlda [%0]\n\tsbc %1\n\ttax\n\tldy #2\n\tlda [%0],y\n\tsbc %1+2\n"
xareg:	SUBU4(xastk,mem4) "\tsec\n\tlda 1,s\n\tsbc %1\n\ttax\n\tlda 3,s\n\tsbc %1+2\n\tply\n\tply\n" 7

xareg:	SUBP4(mem4,vc)	"\tsec\n\tlda %0\n\tsbc #!%1\n\ttax\n\tlda %0+2\n\tsbc #^%1\n"
xareg:	SUBP4(mem4,omem4) "\tsec\n\tlda %0\n\tsbc %1\n\ttax\n\tlda %0+2\n\tsbc %1+2\n"
xareg:	SUBP4(omem4,xareg) "\tsec\n\tpha\n\tphx\n\tlda %0\n\tsbc 1,s\n\ttax\n\tply\n\tlda %0+2\n\tsbc 1,s\n\tply\n" 5
xareg:	SUBP4(xareg,vc) "\tsec\n\ttay\n\ttxa\n\tsbc #!%1\n\ttax\n\ttya\n\tsbc #^%1\n" 4
xareg:	SUBP4(xareg,oyoff32) "\tpha\n\tsec\n\ttxa\n\tsbc [%1],y\n\ttax\n\tpla\n\tiny\n\tiny\n\tsbc [%1],y\n" 
xareg:	SUBP4(xareg,omem4) "\tsec\n\ttay\n\ttxa\n\tsec\n\tsbc %1\n\ttax\n\ttya\n\tsbc %1+2\n" 5
xareg:	SUBP4(poi4,omem4) "\tsec\n\tlda [%0]\n\tsbc %1\n\ttax\n\tldy #2\n\tlda [%0],y\n\tsbc %1+2\n"
xareg:	SUBP4(xastk,mem4) "\tsec\n\tlda 1,s\n\tsbc %1\n\ttax\n\tlda 3,s\n\tsbc %1+2\n\tply\n\tply\n" 7

xareg:	BANDI4(mem4,vc)	"\tlda %0\n\tand #!%1\n\ttax\n\tlda %0+2\n\tand #^%1\n"
xareg:	BANDI4(xareg,vc) "\ttay\n\ttxa\n\tand #!%1\n\ttax\n\ttya\n\tand #^%1\n"
xareg:	BANDI4(xastk,xareg) "\ttay\n\ttxa\n\tand 1,s\n\ttax\n\ttya\n\tand 3,s\n\tply\n\tply\n" 7

xareg:	BANDU4(mem4,vc)	"\tlda %0\n\tand #!%1\n\ttax\n\tlda %0+2\n\tand #^%1\n"
xareg:	BANDU4(xareg,vc) "\ttay\n\ttxa\n\tand #!%1\n\ttax\n\ttya\n\tand #^%1\n"
xareg:	BANDU4(xastk,xareg) "\ttay\n\ttxa\n\tand 1,s\n\ttax\n\ttya\n\tand 3,s\n\tply\n\tply\n" 7

xareg:	BORI4(mem4,vc)	"\tlda %0\n\tora #!%1\n\ttax\n\tlda %0+2\n\tora #^%1\n"
xareg:	BORI4(xareg,vc) "\ttay\n\ttxa\n\tora #!%1\n\ttax\n\ttya\n\tora #^%1\n"
xareg:	BORI4(xastk,xareg) "\ttay\n\ttxa\n\tora 1,s\n\ttax\n\ttya\n\tora 3,s\n\tply\n\tply\n" 7

xareg:	BORU4(mem4,vc)	"\tlda %0\n\tora #!%1\n\ttax\n\tlda %0+2\n\tora #^%1\n"
xareg:	BORU4(xareg,vc) "\ttay\n\ttxa\n\tora #!%1\n\ttax\n\ttya\n\tora #^%1\n"
xareg:	BORU4(xastk,xareg) "\ttay\n\ttxa\n\tora 1,s\n\ttax\n\ttya\n\tora 3,s\n\tply\n\tply\n" 7

xareg:	BXORU4(mem4,vc)	"\tlda %0\n\teor #!%1\n\ttax\n\tlda %0+2\n\teor #^%1\n"
xareg:	BXORU4(xareg,vc) "\ttay\n\ttxa\n\teor #!%1\n\ttax\n\ttya\n\teor #^%1\n"
xareg:	BXORU4(xastk,xareg) "\ttay\n\ttxa\n\teor 1,s\n\ttax\n\ttya\n\teor 3,s\n\tply\n\tply\n" 7

xareg:	BXORI4(mem4,vc)	"\tlda %0\n\teor #!%1\n\ttax\n\tlda %0+2\n\teor #^%1\n"
xareg:	BXORI4(xareg,vc) "\ttay\n\ttxa\n\teor #!%1\n\ttax\n\ttya\n\teor #^%1\n"
xareg:	BXORI4(xastk,xareg) "\ttay\n\ttxa\n\teor 1,s\n\ttax\n\ttya\n\teor 3,s\n\tply\n\tply\n" 7

xareg:	NEGI4(mem4)	"\t.(\n\tlda %0\n\teor #$ffff\n\ttax\n\tlda %0+2\n\teor #$ffff\n\tinx\n\tbne skip\n\tinc\nskip\t.)\n"

xareg:	BCOMI4(xareg)	"\ttay\n\ttxa\n\teor #$ffff\n\ttax\n\ttya\n\teor #$ffff\n"
xareg:	BCOMU4(xareg)	"\ttay\n\ttxa\n\teor #$ffff\n\ttax\n\ttya\n\teor #$ffff\n"

xareg:	CALLI4(tglob)	"\tjsr @%0\n\t!POP %a\n\ttya\n" 3
xareg:	CALLI4(xareg)	"\tjsr @__callxa\n\t!POP %a\n\ttya\n" 4
xareg:	CALLU4(tglob)	"\tjsr @%0\n\t!POP %a\n\ttya\n" 3
xareg:	CALLU4(xareg)	"\tjsr @__callxa\n\t!POP %a\n\ttya\n" 4
xareg:	CALLP4(tglob)	"\tjsr @%0\n\t!POP %a\n\ttya\n" 3
xareg:	CALLP4(xareg)	"\tjsr @__callxa\n\t!POP %a\n\ttya\n" 4

xareg:	xalod		"%0" 

xareg:	CVII4(areg)	"\t.(\n\ttax\n\tlda #0\n\tcpx #$8000\n\tbcc skip\n\tdec\nskip\t.)\n"
xareg:	CVIU4(areg)	"\t.(\n\ttax\n\tlda #0\n\tcpx #$8000\n\tbcc skip\n\tdec\nskip\t.)\n"
xareg:	CVIU4(mem2)	"\t.(\n\tlda #0\n\tldx %0\n\tbpl skip\n\tdec\nskip\t.)\n" 
xareg:	CVUI4(mem2)	"\tldx %0\n\tlda #0\n"
xareg:	CVUU4(areg)	"\ttax\n\tlda #0\n"
xareg:	CVUP4(areg)	"\ttax\n\tlda #0\n"
xareg:	poi4		"\tlda [%0]\n\ttax\n\tldy #2\n\tlda [%0],y\n" 
xareg:	yoff32		"\tlda [%0],y\n\ttax\n\tiny\n\tiny\n\tlda [%0],y\n"
xareg:	xoff32		"\tlda %0,x\n\ttay\n\tlda %0+2,x\n\ttyx\n"

xareg:	CVII4(alod)	"\t.(\n\ttax\n\tlda #0\n\tcpx #$8000\n\tbcc skip\n\tdec\nskip\t.)\n"
xareg:	CVIU4(alod)	"\t.(\n\ttax\n\tlda #0\n\tcpx #$8000\n\tbcc skip\n\tdec\nskip\t.)\n"
xalod:	CVUU4(alod)	"\ttax\n\tlda #0\n"
xalod:	CVUP4(alod)	"\ttax\n\tlda #0\n"

xalod:	CVIU4(omem2)	"\t.(\n\tlda #0\n\tldx %0\n\tbpl skip\n\tdec\nskip\t.)\n" 
xalod:	CVUI4(omem2)	"\tldx %0\n\tlda #0\n"

xalod:	vc		"\tldx #!%0\n\tlda #^%0\n" 
xalod:	omem4		"\tlda %0+2\n\tldx %0\n" 
xalod:	varp		"\ttdc\n\tclc\n\tadc #%a\n\ttax\n\tlda #0\n" 
xalod:	opoi4		"\tlda [%0]\n\ttax\n\tldy #2\n\tlda [%0],y\n" 
xalod:  ADDRGP4		"\tldx #{%a\n\tlda #%a\n\tjsr @S_segptr\n\ttyx\n" segshare(a)
xalod:	oyoff32		"\tlda [%0],y\n\ttax\n\tiny\n\tiny\n\tlda [%0],y\n"
xalod:	oxoff32		"\tlda %0,x\n\ttay\n\tlda %0+2,x\n\ttyx\n"

mreg:	xareg		"%0\tsta mreg+2\n\tstx mreg\n" 5
mreg2:	xareg		"%0\tsta mreg2+2\n\tstx mreg2\n" 5
xastk:	xareg		"%0\tpha\n\tphx\n" 7
amreg2:	areg		"%0\tsta mreg2\n" 5
stack:	areg		"%0\tpha\n" 8

aflag:	NEGI2(areg)	"\teor #$ffff\n\tinc\n"

aflag:	BANDI2(areg,vc) "\tand #%1\n"
aflag:	BANDI2(areg,omem2) "\tand %1\n"
aflag:	BANDI2(stack,mem2) "\tpla\n\tand %1\n"
aflag:	BANDU2(areg,vc) "\tand #%1\n"
aflag:	BANDU2(areg,omem2) "\tand %1\n"
aflag:	BANDU2(stack,mem2) "\tpla\n\tand %1\n"

aflag:	BXORI2(areg,vc) "\teor #%1\n"
aflag:	BXORI2(areg,omem2) "\teor %1\n"
aflag:	BXORI2(stack,mem2) "\tpla\n\teor %1\n"

aflag:	BXORU2(areg,vc) "\teor #%1\n"
aflag:	BXORU2(areg,omem2) "\teor %1\n"
aflag:	BXORU2(stack,mem2) "\tpla\n\teor %1\n"

aflag:	BORI2(areg,vc) "\tora #%1\n"
aflag:	BORI2(areg,omem2) "\tora %1\n"
aflag:	BORI2(stack,mem2) "\tpla\n\tora %1\n"

aflag:	BORU2(areg,vc) "\tora #%1\n"
aflag:	BORU2(areg,omem2) "\tora %1\n"
aflag:	BORU2(stack,mem2) "\tpla\n\tora %1\n"

aflag:	BCOMI2(areg)	"\teor #$ffff\n"
aflag:	BCOMU2(areg)	"\teor #$ffff\n"

aflag:	CVUI2(aflag)	""
aflag:	CVUI2(areg1)	""
areg:	CVII2(areg1)	"\t.(\n\tcmp #$80\n\tbcc skip\n\tora #$ff00\nskip\t.)\n"

areg:	MULI2(amreg2,alod) "\tjsr @__muli16\n"
areg:	MULI2(stack,areg) "\tply\n\tsty mreg2\n\tjsr @__muli16\n"
areg:	MULU2(amreg2,alod) "\tjsr @__muli16\n"
areg:	MULU2(stack,areg) "\tply\n\tsty mreg2\n\tjsr @__muli16\n"

areg:	DIVI2(amreg2,alod) "\tjsr @__divi16\n"
areg:	DIVI2(stack,areg) "\tply\n\tsty mreg2\n\tjsr @__divi16\n"
areg:	DIVU2(amreg2,alod) "\tjsr @__divu16\n"
areg:	DIVU2(stack,areg) "\tply\n\tsty mreg2\n\tjsr @__divu16\n"

areg:	MODI2(amreg2,alod) "\tjsr @__modi16\n"
areg:	MODI2(stack,areg) "\tply\n\tsty mreg2\n\tjsr @__modi16\n"
areg:	MODU2(amreg2,alod) "\tjsr @__modu16\n"
areg:	MODU2(stack,areg) "\tply\n\tsty mreg2\n\tjsr @__modu16\n"

aflag:	LSHI2(areg,vc) "\t!LSH %1\n"
areg:	LSHI2(areg,omem2) "\t.(\n\tldy %1\n\tbeq skip\nblah\tasl\n\tdey\n\tbne blah\nskip\t.)\n" 3
areg:	LSHI2(stack,mem2) "\t.(\n\tpla\n\tldy %1\n\tbeq skip\nblah\tasl\n\tdey\n\tbne blah\nskip\t.)\n" 5

aflag:	LSHU2(areg,vc) "\t!LSH %1\n"
areg:	LSHU2(areg,omem2) "\t.(\n\tldy %1\n\tbeq skip\nblah\tasl\n\tdey\n\tbne blah\nskip\t.)\n" 3
areg:	LSHU2(stack,mem2) "\t.(\n\tpla\n\tldy %1\n\tbeq skip\nblah\tasl\n\tdey\n\tbne blah\nskip\t.)\n" 5

aflag:	RSHI2(areg,vc) "\t!ARSH %1\n"
areg:	RSHI2(mem2,omem2) "\t.(\n\tlda %0\n\tldy %1\n\tbeq skip\nblah\tcmp #$8000\n\tror\n\tdey\n\tbne blah\nskip\t.)\n" 3
areg:	RSHI2(stack,mem2) "\t.(\n\tpla\n\tldy %1\n\tbeq skip\nblah\tcmp #$8000\n\tror\n\tdey\n\tbne blah\nskip\t.)\n" 5

aflag:	RSHU2(areg,vc) "\t!RSH %1\n"
areg:	RSHU2(mem2,omem2) "\t.(\n\tlda %0\n\tldy %1\n\tbeq skip\nblah\tlsr\n\tdey\n\tbne blah\nskip\t.)\n" 3
areg:	RSHU2(stack,mem2) "\t.(\n\tpla\n\tldy %1\n\tbeq skip\nblah\tlsr\n\tdey\n\tbne blah\nskip\t.)\n" 5

aflag:	ADDI2(areg,vc) 	"\tclc\n\tadc #%1\n" 3
aflag:	ADDI2(areg,con1) "\tinc\n"
aflag:	ADDI2(areg,omem2) "\tclc\n\tadc %1\n"
aflag:	ADDI2(areg,oxoff) "\tclc\n\tadc %1,x\n"
aflag:	ADDI2(areg,oyoff) "\tclc\n\tadc [%1],y\n"
aflag:	ADDI2(omem2,areg) "\tclc\n\tadc %0\n"
aflag:	ADDI2(stack,mem2) "\tclc\n\tpla\n\tadc %1\n" 7

aflag:	ADDU2(areg,vc) 	"\tclc\n\tadc #%1\n" 3
aflag:	ADDU2(areg,con1) "\tinc\n"
aflag:	ADDU2(areg,omem2) "\tclc\n\tadc %1\n"
aflag:	ADDU2(areg,oxoff) "\tclc\n\tadc %1,x\n"
aflag:	ADDU2(areg,oyoff) "\tclc\n\tadc [%1],y\n"
aflag:	ADDU2(omem2,areg) "\tclc\n\tadc %0\n"
aflag:	ADDU2(stack,mem2) "\tclc\n\tpla\n\tadc %1\n" 7

aflag:	SUBI2(areg,vc)	"\tsec\n\tsbc #%1\n" 3
aflag:	SUBI2(areg,con1) "\tdec\n"
aflag:	SUBI2(areg,omem2) "\tsec\n\tsbc %1\n"
aflag:	SUBI2(areg,oxoff) "\tsec\n\tsbc %1,x\n"
aflag:	SUBI2(areg,oyoff) "\tsec\n\tsbc [%1],y\n"
aflag:	SUBI2(stack,mem2) "\tsec\n\tpla\n\tsbc %1\n" 7

aflag:	SUBU2(areg,vc)	"\tsec\n\tsbc #%1\n" 3
aflag:	SUBU2(areg,con1) "\tdec\n"
aflag:	SUBU2(areg,omem2) "\tsec\n\tsbc %1\n"
aflag:	SUBU2(areg,oxoff) "\tsec\n\tsbc %1,x\n"
aflag:	SUBU2(areg,oyoff) "\tsec\n\tsbc [%1],y\n"
aflag:	SUBU2(stack,mem2) "\tsec\n\tpla\n\tsbc %1\n" 7

aflag1:	LOADI1(areg)	"\tand #$ff\n"
aflag1:	LOADU1(areg)	"\tand #$ff\n"
aflag1:	LOADI1(xareg)	"\ttxa\n\tand #$ff\n"
aflag1:	LOADU1(xareg)	"\ttxa\n\tand #$ff\n"

areg:	LOADI2(areg)	""
aflag:	LOADI2(xareg)	"\ttxa\n"
areg:	LOADU2(areg)	""
aflag:	LOADU2(xareg)	"\ttxa\n"
aflag:	LOADU2(poi4)	"\tlda [%0]\n"

aflag:	xoff		"\tlda %0,x\n"
aflag:	yoff		"\tlda [%0],y\n"
aflag:	areg		"%0\ttay\n" 1
aflag:  aflag1		"%0"

areg:	aflag		"%0"
aflag:	alod		"%0"
aflag:	areg1		"%0"
aflag:	poi2		"\tlda [%0]\n"
aflag:	mem2		"\tlda %0\n"
aflag:	CALLI2(tglob)	"\tjsr @%0\n\t!POP %atxa\n" 3
aflag:	CALLI2(xareg)	"\tjsr @__callxa\n\t!POP %a\n\ttxa\n" 4
aflag:	CALLU2(tglob)	"\tjsr @%0\n\t!POP %a\n\ttxa\n" 3
aflag:	CALLU2(xareg)	"\tjsr @__callxa\n\t!POP %a\n\ttxa\n" 4

alod:	omem2		"\tlda %0\n"
alod:	acon		"\tlda #%0\n" 
alod:	opoi2		"\tlda [%0]\n"
alod:	INDIRI2(con)	"\tlda @%0\n"
alod:	alod1		"%0"

alod1:	omem1		"\tlda %0\n\tand #$ff\n"
alod1:	opoi1		"\tlda [%0]\n\tand #$ff\n"
alod1:	LOADU1(alod)	"\tand #$ff\n"
alod1:	LOADI1(alod)	"\tand #$ff\n"

areg1:	alod1		"%0"
areg1:	aflag1		"%0"
areg1:	poi1		"\tlda [%0]\n\tand #$ff\n" 7
areg1:	xoff1		"\tlda %0,x\n\tand #$ff\n"
areg1:	yoff1		"\tlda [%0],y\n\tand #$ff\n"
areg1:	LOADU1(areg)	"\tand #$ff\n" 3
areg1:	LOADI1(areg)	"\tand #$ff\n" 3

stmt:	call2 ""
stmt:	call4 ""
stmt:	callv ""

ayoff:	areg		"%0\ttay\n" 100
ayoff:	mem2		"\tldy %0\n"
ayoff:	oayoff		"%0"

oayoff:	con16b		"\tldy #%0\n"
oayoff:	omem2		"\tldy %0\n"

axoff:	areg		"%0\ttax\n" 2
axoff:	mem2		"\tldx %0\n"
axoff:	oaxoff		"%0"

oaxoff:	con16b		"\tldx #%0\n"
oaxoff:	omem2		"\tldx %0\n"

indy:	ADDP4(memp,ayoff) "%0"
indy:	ADDP4(ayoff,memp) "%1"
indx:	ADDP4(var,axoff) "%0"
indx:	ADDP4(axoff,var) "%1"
oindy:	ADDP4(oayoff,memp) "%1"
oindy:	ADDP4(memp,oayoff) "%0"
oindx:	ADDP4(var,oaxoff) "%0"
oindx:	ADDP4(oaxoff,var) "%1"

yoff32:	LOADU4(yoff32)	"%0"
yoff32:	INDIRI4(indy)	"%0"
yoff32:	INDIRU4(indy)	"%0"
yoff32:	INDIRP4(indy)	"%0"
yoff:	INDIRI2(indy)	"%0"
yoff:	INDIRU2(indy)	"%0"
yoff1:	INDIRI1(indy)	"%0"
yoff1:	INDIRU1(indy)	"%0"

oyoff32: LOADU4(oyoff32) "%0"
oyoff32: INDIRI4(oindy)	"%0"
oyoff32: INDIRU4(oindy)	"%0"
oyoff32: INDIRP4(oindy)	"%0"
oyoff:	INDIRI2(oindy)	"%0"
oyoff:	INDIRU2(oindy)	"%0"

xoff32:	INDIRI4(indx) "%0"
xoff32:	INDIRU4(indx) "%0"
xoff32:	INDIRP4(indx) "%0"
xoff:	INDIRI2(indx) "%0"
xoff:	INDIRU2(indx) "%0"
xoff1:	INDIRI1(indx) "%0"
xoff1:	INDIRU1(indx) "%0"

oxoff32: INDIRI4(oindx) "%0"
oxoff32: INDIRU4(oindx) "%0"
oxoff32: INDIRP4(oindx) "%0"
oxoff:	INDIRI2(oindx) "%0"
oxoff:	INDIRU2(oindx) "%0"

aomode:	vc	" #%0"
aomode:	omem2	"%0"
aomode:	opoi2	"[%0]"
aomode:	oyoff	"[%0],y"

stmt:	ASGNI1(var,areg) "\t!AS\n\tsta %0\n\t!AL\n"
stmt:	ASGNI1(poiad,areg) "\t!AS\n\tsta [%0]\n\t!AL\n" 5
stmt:	ASGNI1(indx,alod) "\t!AS\n\tsta %0,x\n\t!AL\n"
stmt:	ASGNI1(indy,alod) "\t!AS\n\tsta [%0],y\n\t!AL\n"

stmt:	ASGNU1(var,areg) "\t!AS\n\tsta %0\n\t!AL\n"
stmt:	ASGNU1(poiad,areg) "\t!AS\n\tsta [%0]\n\t!AL\n" 5
stmt:	ASGNU1(indx,alod) "\t!AS\n\tsta %0,x\n\t!AL\n"
stmt:	ASGNU1(indy,alod) "\t!AS\n\tsta [%0],y\n\t!AL\n"

stmt:	ASGNI2(var,areg) "\tsta %0\n" 4
stmt:	ASGNI2(var,call2) "\tstx %0\n"
stmt:	ASGNI2(var,con0) "\tstz %0\n"
stmt:	ASGNI2(poiad,areg) "\tsta [%0]\n"
stmt:	ASGNI2(indy,alod) "\tsta [%0],y\n" 
stmt:	ASGNI2(indx,alod) "\tsta %0,x\n" 
stmt:	ASGNI2(ADDP4(con16b,memp),areg) "\tldy #%0\n\tsta [%1],y\n" 
stmt:	ASGNI2(ADDP4(memp,con16b),areg) "\tldy #%1\n\tsta [%0],y\n" 
stmt:	ASGNI2(var,ADDI2(omem2,con1)) "\tinc %1\n" memop(a)
stmt:	ASGNI2(var,ADDU2(omem2,con1)) "\tinc %1\n" memop(a)
stmt:	ASGNI2(var,SUBI2(omem2,con1)) "\tdec %1\n" memop(a)
stmt:	ASGNI2(var,SUBU2(omem2,con1)) "\tdec %1\n" memop(a)
stmt:	ASGNI2(con,areg) "\tsta @%0\n"

stmt:	ASGNU2(var,areg) "\tsta %0\n" 4
stmt:	ASGNU2(var,call2) "\tstx %0\n"
stmt:	ASGNU2(var,con0) "\tstz %0\n"
stmt:	ASGNU2(poiad,areg) "\tsta [%0]\n"
stmt:	ASGNU2(indy,alod) "\tsta [%0],y\n" 
stmt:	ASGNU2(indx,alod) "\tsta %0,x\n" 
stmt:	ASGNU2(ADDP4(con16b,memp),areg) "\tldy #%0\n\tsta [%1],y\n" 
stmt:	ASGNU2(ADDP4(memp,con16b),areg) "\tldy #%1\n\tsta [%0],y\n" 
stmt:	ASGNU2(var,ADDI2(omem2,con1)) "\tinc %1\n" memop(a)
stmt:	ASGNU2(var,ADDU2(omem2,con1)) "\tinc %1\n" memop(a)
stmt:	ASGNU2(var,SUBI2(omem2,con1)) "\tdec %1\n" memop(a)
stmt:	ASGNU2(var,SUBU2(omem2,con1)) "\tdec %1\n" memop(a)
stmt:	ASGNU2(con,areg) "\tsta @%0\n"

fac4:	INDIRF4(var) "\tlda %0\n\tldx %0+2\n"
fac4:	ADDF4(facm2,fac4) "\tjsr @__addf32\n"
fac4:	SUBF4(facm2,fac4) "\tjsr @__addf32\n"
fac4:	MULF4(facm2,fac4) "\tjsr @__mulf32\n"
fac4:	DIVF4(facm2,fac4) "\tjsr @__divf32\n"

facm2:	fac4 "\tsta mreg2\n\tstx mreg2+2\n"

stmt:	ASGNF4(var, fac4) "\tstx %0\n\tsta %0+2\n"

stmt:	ASGNI4(var,call4) "\tstx %0\n\tsty %0+2\n"
stmt:	ASGNI4(var,vc) "\tlda #!%1\n\tsta %0\n\tlda #^%1\n\tsta %0+2\n" 4
stmt:	ASGNI4(var,mem4) "\tldx %1\n\tlda %1+2\n\tsta %0+2\n\tstx %0\n" 
stmt:	ASGNI4(var,xareg) "\tstx %0\n\tsta %0+2\n" 5
stmt:	ASGNI4(var,con0) "\tstz %0\n\tstz %0+2\n"
stmt:	ASGNI4(var,con16b) "\tlda #%1\n\tsta %0\n\tstz %0+2\n" 2
stmt:	ASGNI4(poiad,vc) "\tlda #!%1\n\tsta [%0]\n\tldy #2\n\tlda #^%1\n\tsta [%0],y\n" 5
stmt:	ASGNI4(poiad,mem4) "\tlda %1\n\tsta [%0]\n\tlda %1+2\n\tldy #2\n\tsta [%0],y\n" 5
stmt:	ASGNI4(poiad,xareg) "\tldy #2\n\tsta [%0],y\n\ttxa\n\tsta [%0]\n" 5

stmt:	ASGNI4(var,ADDI4(omem4,con1)) "\t!INP %1\n" memop(a)
stmt:	ASGNI4(var,ADDU4(omem4,con1)) "\t!INP %1\n" memop(a)
stmt:	ASGNI4(var,ADDP4(omem4,con1)) "\t!INP %1\n" memop(a)
stmt:	ASGNI4(var,ADDI4(omem4,con16b)) "\t.(\n\tclc\n\tlda %1\n\tadc #!%2\n\tsta %1\n\tbcc skip\n\tinc %1+2\nskip\t.)\n" memop(a)
stmt:	ASGNI4(var,ADDU4(omem4,con16b)) "\t.(\n\tclc\n\tlda %1\n\tadc #!%2\n\tsta %1\n\tbcc skip\n\tinc %1+2\nskip\t.)\n" memop(a)
stmt:	ASGNI4(var,ADDP4(omem4,con16b)) "\t.(\n\tclc\n\tlda %1\n\tadc #!%2\n\tsta %1\n\tbcc skip\n\tinc %1+2\nskip\t.)\n" memop(a)
stmt:	ASGNI4(var,ADDI4(omem4,vc)) "\tclc\n\tlda %1\n\tadc #!%2\n\tsta %0\n\tlda %1+2\n\tadc #^%2\n\tsta %0+2\n" 4
stmt:	ASGNI4(var,ADDU4(omem4,vc)) "\tclc\n\tlda %1\n\tadc #!%2\n\tsta %0\n\tlda %1+2\n\tadc #^%2\n\tsta %0+2\n" 4
stmt:	ASGNI4(var,ADDP4(omem4,vc)) "\tclc\n\tlda %1\n\tadc #!%2\n\tsta %0\n\tlda %1+2\n\tadc #^%2\n\tsta %0+2\n" 4
stmt:	ASGNI4(var,LSHI4(omem4,con)) "\t!LSH32M %2,%1\n" memop(a)
stmt:	ASGNI4(var,LSHU4(omem4,con)) "\t!LSH32M %2,%1\n" memop(a)
stmt:	ASGNI4(var,RSHU4(omem4,con)) "\t!RSH32M %2,%1\n" memop(a)
stmt:	ASGNI4(indy,omem4) "\tlda %1\n\tsta [%0],y\n\tiny\n\tiny\n\tlda %1+2\n\tsta [%0],y\n" 
stmt:	ASGNI4(ADDP4(memp,con16b),xareg) "\tldy #%1+2\n\tsta [%0],y\n\tdey\n\tdey\n\ttxa\n\tsta [%0],y\n" 
stmt:	ASGNI4(ADDP4(con16b,memp),xareg) "\tldy #%0+2\n\tsta [%1],y\n\tdey\n\tdey\n\ttxa\n\tsta [%1],y\n" 

stmt:	ASGNU4(var,call4) "\tstx %0\n\tsty %0+2\n"
stmt:	ASGNU4(var,vc) "\tlda #!%1\n\tsta %0\n\tlda #^%1\n\tsta %0+2\n" 4
stmt:	ASGNU4(var,mem4) "\tldx %1\n\tlda %1+2\n\tsta %0+2\n\tstx %0\n" 
stmt:	ASGNU4(var,xareg) "\tstx %0\n\tsta %0+2\n" 5
stmt:	ASGNU4(var,con0) "\tstz %0\n\tstz %0+2\n"
stmt:	ASGNU4(var,con16b) "\tlda #%1\n\tsta %0\n\tstz %0+2\n" 2
stmt:	ASGNU4(poiad,vc) "\tlda #!%1\n\tsta [%0]\n\tldy #2\n\tlda #^%1\n\tsta [%0],y\n" 5
stmt:	ASGNU4(poiad,mem4) "\tlda %1\n\tsta [%0]\n\tlda %1+2\n\tldy #2\n\tsta [%0],y\n" 5
stmt:	ASGNU4(poiad,xareg) "\tldy #2\n\tsta [%0],y\n\ttxa\n\tsta [%0]\n" 5

stmt:	ASGNU4(var,ADDI4(omem4,con1)) "\t!INP %1\n" memop(a)
stmt:	ASGNU4(var,ADDU4(omem4,con1)) "\t!INP %1\n" memop(a)
stmt:	ASGNU4(var,ADDP4(omem4,con1)) "\t!INP %1\n" memop(a)
stmt:	ASGNU4(var,ADDI4(omem4,con16b)) "\t.(\n\tclc\n\tlda %1\n\tadc #!%2\n\tsta %1\n\tbcc skip\n\tinc %1+2\nskip\t.)\n" memop(a)
stmt:	ASGNU4(var,ADDU4(omem4,con16b)) "\t.(\n\tclc\n\tlda %1\n\tadc #!%2\n\tsta %1\n\tbcc skip\n\tinc %1+2\nskip\t.)\n" memop(a)
stmt:	ASGNU4(var,ADDP4(omem4,con16b)) "\t.(\n\tclc\n\tlda %1\n\tadc #!%2\n\tsta %1\n\tbcc skip\n\tinc %1+2\nskip\t.)\n" memop(a)
stmt:	ASGNU4(var,ADDI4(omem4,vc)) "\tclc\n\tlda %1\n\tadc #!%2\n\tsta %0\n\tlda %1+2\n\tadc #^%2\n\tsta %0+2\n" 4
stmt:	ASGNU4(var,ADDU4(omem4,vc)) "\tclc\n\tlda %1\n\tadc #!%2\n\tsta %0\n\tlda %1+2\n\tadc #^%2\n\tsta %0+2\n" 4
stmt:	ASGNU4(var,ADDP4(omem4,vc)) "\tclc\n\tlda %1\n\tadc #!%2\n\tsta %0\n\tlda %1+2\n\tadc #^%2\n\tsta %0+2\n" 4
stmt:	ASGNU4(var,LSHI4(omem4,con)) "\t!LSH32M %2,%1\n" memop(a)
stmt:	ASGNU4(var,LSHU4(omem4,con)) "\t!LSH32M %2,%1\n" memop(a)
stmt:	ASGNU4(var,RSHU4(omem4,con)) "\t!RSH32M %2,%1\n" memop(a)
stmt:	ASGNU4(indy,omem4) "\tlda %1\n\tsta [%0],y\n\tiny\n\tiny\n\tlda %1+2\n\tsta [%0],y\n" 
stmt:	ASGNU4(ADDP4(memp,con16b),xareg) "\tldy #%1+2\n\tsta [%0],y\n\tdey\n\tdey\n\ttxa\n\tsta [%0],y\n" 
stmt:	ASGNU4(ADDP4(con16b,memp),xareg) "\tldy #%0+2\n\tsta [%1],y\n\tdey\n\tdey\n\ttxa\n\tsta [%1],y\n" 

stmt:	ASGNP4(var,call4) "\tstx %0\n\tsty %0+2\n"
stmt:	ASGNP4(var,vc) "\tlda #!%1\n\tsta %0\n\tlda #^%1\n\tsta %0+2\n" 4
stmt:	ASGNP4(var,mem4) "\tldx %1\n\tlda %1+2\n\tsta %0+2\n\tstx %0\n" 
stmt:	ASGNP4(var,xareg) "\tstx %0\n\tsta %0+2\n" 5
stmt:	ASGNP4(var,con0) "\tstz %0\n\tstz %0+2\n"
stmt:	ASGNP4(var,con16b) "\tlda #%1\n\tsta %0\n\tstz %0+2\n" 2
stmt:	ASGNP4(poiad,vc) "\tlda #!%1\n\tsta [%0]\n\tldy #2\n\tlda #^%1\n\tsta [%0],y\n" 5
stmt:	ASGNP4(poiad,mem4) "\tlda %1\n\tsta [%0]\n\tlda %1+2\n\tldy #2\n\tsta [%0],y\n" 5
stmt:	ASGNP4(poiad,xareg) "\tldy #2\n\tsta [%0],y\n\ttxa\n\tsta [%0]\n" 5

stmt:	ASGNP4(var,ADDI4(omem4,con1)) "\t!INP %1\n" memop(a)
stmt:	ASGNP4(var,ADDU4(omem4,con1)) "\t!INP %1\n" memop(a)
stmt:	ASGNP4(var,ADDP4(omem4,con1)) "\t!INP %1\n" memop(a)
stmt:	ASGNP4(var,ADDI4(omem4,con16b)) "\t.(\n\tclc\n\tlda %1\n\tadc #!%2\n\tsta %1\n\tbcc skip\n\tinc %1+2\nskip\t.)\n" memop(a)
stmt:	ASGNP4(var,ADDU4(omem4,con16b)) "\t.(\n\tclc\n\tlda %1\n\tadc #!%2\n\tsta %1\n\tbcc skip\n\tinc %1+2\nskip\t.)\n" memop(a)
stmt:	ASGNP4(var,ADDP4(omem4,con16b)) "\t.(\n\tclc\n\tlda %1\n\tadc #!%2\n\tsta %1\n\tbcc skip\n\tinc %1+2\nskip\t.)\n" memop(a)
stmt:	ASGNP4(var,ADDI4(omem4,vc)) "\tclc\n\tlda %1\n\tadc #!%2\n\tsta %0\n\tlda %1+2\n\tadc #^%2\n\tsta %0+2\n" 4
stmt:	ASGNP4(var,ADDU4(omem4,vc)) "\tclc\n\tlda %1\n\tadc #!%2\n\tsta %0\n\tlda %1+2\n\tadc #^%2\n\tsta %0+2\n" 4
stmt:	ASGNP4(var,ADDP4(omem4,vc)) "\tclc\n\tlda %1\n\tadc #!%2\n\tsta %0\n\tlda %1+2\n\tadc #^%2\n\tsta %0+2\n" 4
stmt:	ASGNP4(var,LSHI4(omem4,con)) "\t!LSH32M %2,%1\n" memop(a)
stmt:	ASGNP4(var,LSHU4(omem4,con)) "\t!LSH32M %2,%1\n" memop(a)
stmt:	ASGNP4(var,RSHU4(omem4,con)) "\t!RSH32M %2,%1\n" memop(a)
stmt:	ASGNP4(indy,omem4) "\tlda %1\n\tsta [%0],y\n\tiny\n\tiny\n\tlda %1+2\n\tsta [%0],y\n" 
stmt:	ASGNP4(ADDP4(memp,con16b),xareg) "\tldy #%1+2\n\tsta [%0],y\n\tdey\n\tdey\n\ttxa\n\tsta [%0],y\n" 
stmt:	ASGNP4(ADDP4(con16b,memp),xareg) "\tldy #%0+2\n\tsta [%1],y\n\tdey\n\tdey\n\ttxa\n\tsta [%1],y\n" 

stmt:	ASGNB(poiad,INDIRB(mem4)) "\t.(\n\t!AS\n\tldy #%a-1\nloop\tlda [%1],y\n\tsta [%0],y\n\tdey\n\tbpl loop\n\t.)\n\t!AL\n"

stmt:	RETI2(vc)	"\tldx #%0\n"
stmt:	RETI2(mem2)	"\tldx %0\n"
stmt:	RETI2(areg)	"\ttax\n"
stmt:	RETI2(poi2)	"\tlda [%0]\n\ttax\n"

stmt:	RETU2(vc)	"\tldx #%0\n"
stmt:	RETU2(mem2)	"\tldx %0\n"
stmt:	RETU2(areg)	"\ttax\n"
stmt:	RETU2(poi2)	"\tlda [%0]\n\ttax\n"

stmt:	RETU4(vc)	"\tldx #!%0\n\tldy #^%0\n"
stmt:	RETU4(mem4)	"\tldx %0\n\tldy %0+2\n"
stmt:	RETU4(poi4)	"\tlda [%0]\n\ttax\n\tldy #2\n\tlda [%0],y\n\ttay\n"
stmt:	RETU4(xareg)	"\ttay\n"

stmt:	RETI4(vc)	"\tldx #!%0\n\tldy #^%0\n"
stmt:	RETI4(mem4)	"\tldx %0\n\tldy %0+2\n"
stmt:	RETI4(poi4)	"\tlda [%0]\n\ttax\n\tldy #2\n\tlda [%0],y\n\ttay\n"
stmt:	RETI4(xareg)	"\ttay\n"

stmt:	RETP4(vc)	"\tldx #!%0\n\tldy #^%0\n"
stmt:	RETP4(mem4)	"\tldx %0\n\tldy %0+2\n"
stmt:	RETP4(poi4)	"\tlda [%0]\n\ttax\n\tldy #2\n\tlda [%0],y\n\ttay\n"
stmt:	RETP4(xareg)	"\ttay\n"

stmt:	LABELV		"%a\n"
stmt:	JUMPV(tglob)	"\t!jra %0\n"
stmt:	JUMPV(xareg)	"\t!AS\n\tpha\n\t!AL\n\tdex\n\tphx\n\trtl\n" 5


stmt:	ARGI2(vc)	"\tpea %0\n"
stmt:	ARGI2(memp)	"\tpei (%0)\n"
stmt:	ARGI2(areg)	"\tpha\n" 7

stmt:	ARGU2(vc)	"\tpea %0\n"
stmt:	ARGU2(memp)	"\tpei (%0)\n"
stmt:	ARGU2(areg)	"\tpha\n" 7

stmt:	ARGI4(vc)	"\tpea ^%0\n\tpea !%0\n"
stmt:	ARGI4(memp)	"\tpei (%0+2)\n\tpei (%0)\n"
stmt:	ARGI4(mem4)	"\tlda %0+2\n\tpha\n\tlda %0\n\tpha\n" 7
stmt:	ARGI4(xareg)	"\tpha\n\tphx\n" 
stmt:	ARGI4(poi4)	"\tldy #2\n\tlda [%0],y\n\tpha\n\tlda [%0]\n\tpha\n"

stmt:	ARGU4(vc)	"\tpea ^%0\n\tpea !%0\n"
stmt:	ARGU4(memp)	"\tpei (%0+2)\n\tpei (%0)\n"
stmt:	ARGU4(mem4)	"\tlda %0+2\n\tpha\n\tlda %0\n\tpha\n" 7
stmt:	ARGU4(xareg)	"\tpha\n\tphx\n"
stmt:	ARGU4(poi4)	"\tldy #2\n\tlda [%0],y\n\tpha\n\tlda [%0]\n\tpha\n"

stmt:	ARGP4(vc)	"\tpea ^%0\n\tpea !%0\n"
stmt:	ARGP4(memp)	"\tpei (%0+2)\n\tpei (%0)\n"
stmt:	ARGP4(mem4)	"\tlda %0+2\n\tpha\n\tlda %0\n\tpha\n" 7
stmt:	ARGP4(xareg)	"\tpha\n\tphx\n" 5
stmt:	ARGP4(varp)	"\tpea 0\n\ttdc\n\tclc\n\tadc #%0\n\tpha\n"
stmt:	ARGP4(poi4)	"\tldy #2\n\tlda [%0],y\n\tpha\n\tlda [%0]\n\tpha\n"

stmt:	ARGB(INDIRB(mem4))	"\t.(\n\t!AS\n\tldy #%a\nloop\tdey\n\tlda [%0],y\n\tpha\n\tcpy #0\n\tbne loop\n\t!AL\n\t.)\n"

stmt:	NEI2(areg,aomode) "\tcmp %1\n\t!jne %a\n" 2
stmt:	NEI2(aflag,con0) "\t!jne %a\n"
stmt:	NEI2(stack,mem2) "\tpla\n\tcmp %1\n\t!jne %a\n" 3

stmt:	NEU2(areg,aomode) "\tcmp %1\n\t!jne %a\n" 2
stmt:	NEU2(aflag,con0) "\t!jne %a\n"
stmt:	NEU2(stack,mem2) "\tpla\n\tcmp %1\n\t!jne %a\n" 3

stmt:	EQI2(areg,aomode) "\tcmp %1\n\t!jeq %a\n" 2
stmt:	EQI2(aflag,con0) "\t!jeq %a\n"
stmt:	EQI2(stack,mem2) "\tpla\n\tcmp %1\n\t!jeq %a\n" 3

stmt:	EQU2(areg,aomode) "\tcmp %1\n\t!jeq %a\n" 2
stmt:	EQU2(aflag,con0) "\t!jeq %a\n"
stmt:	EQU2(stack,mem2) "\tpla\n\tcmp %1\n\t!jeq %a\n" 3

stmt:	GTI2(areg,aomode) "\tsec\n\tsbc %1\n\t!jgt %a\n" 2
stmt:	GTI2(aflag,con16b) "\t.(\n\t!jmi skip\n\tcmp #%1\n\t!jgtu %a\nskip\t.)\n"
stmt:	GTI2(stack,mem2) "\tpla\n\tsec\n\tsbc %1\n\t!jgt %a\n" 3

stmt:	GTU2(areg,aomode) "\tcmp %1\n\t!jgtu %a\n" 2
stmt:	GTU2(stack,mem2) "\tpla\n\tcmp %1\n\t!jgtu %a\n" 3

stmt:	LEI2(areg,aomode) "\tsec\n\tsbc %1\n\t!jle %a\n" 2
stmt:	LEI2(aflag,con16b) "\t!jmi %a\n\tcmp #%1\n\t!jleu %a\n"
stmt:	LEI2(stack,mem2) "\tpla\n\tsec\n\tsbc %1\n\t!jleu %a\n" 3

stmt:	LEU2(areg,aomode) "\tcmp %1\n\t!jleu %a\n" 2
stmt:	LEU2(stack,mem2) "\tpla\n\tcmp %1\n\t!jleu %a\n" 3

stmt:	GEI2(areg,aomode)	"\tsec\n\tsbc %1\n\t!jge %a\n" 2
stmt:	GEI2(aflag,con16b) "\t.(\n\t!jmi skip\n\tcmp #%1\n\t!jpl %a\nskip\t.)\n"
stmt:	GEI2(stack,mem2) "\tpla\n\tsec\n\tsbc %1\n\t!jge %a\n" 3

stmt:	GEU2(areg,aomode) "\tcmp %1\n\t!jcs %a\n"
stmt:	GEU2(stack,mem2) "\tpla\n\tcmp %1\n\t!jcs %a\n" 3

stmt:	LTI2(areg,aomode) "\tsec\n\tsbc %1\n\t!jlt %a\n"
stmt:	LTI2(aflag,con16b) "\t!jmi %a\n\tcmp #%1\n\t!jmi %a\n"
stmt:	LTI2(stack,mem2) "\tpla\n\tsec\n\tsbc %1\n\t!jlt %a\n"

stmt:	LTU2(areg,aomode) "\tcmp %1\n\t!jcc %a\n"
stmt:	LTU2(stack,mem2) "\tpla\n\tcmp %1\n\t!jcc %a\n"

stmt:	LTI4(mem4,vc)	"\t.(\n\tlda %0+2\n\tcmp #^%1\n\tbmi made\n\tbne skip\n\tlda %0\n\tcmp #!%1\n\tbcs skip\nmade\tbrl %a\nskip\t.)\n"
stmt:	LTI4(mem4,omem4) "\t.(\n\tlda %0+2\n\tcmp %1+2\n\tbmi made\n\tbne skip\n\tlda %0\n\tcmp %1\n\tbcs skip\nmade\tbrl %a\nskip\t.)\n"
stmt:	LTI4(poi41,mem4) "\t.(\n\tldy #2\n\tlda [%0],y\n\tcmp %1+2\n\tbmi made\n\tbne skip\n\tlda [%0]\n\tcmp %1\n\tbcs skip\nmade\tbrl %a\nskip\t.)\n"
stmt:	LTI4(mem41,mem4) "\t.(\n\tlda %0+2\n\tcmp %1+2\n\tbmi made\n\tbne skip\n\tlda %0\n\tcmp %1\n\tbcs skip\nmade\tbrl %a\nskip\t.)\n"

stmt:	LTU4(mem4,vc)	"\tlda %0\n\tcmp #!%1\n\tlda %0+2\n\tsbc #^%1\n\t!jcc %a\n"
stmt:	LTU4(mem41,mem4) "\tlda %0\n\tcmp %1\n\tlda %0+2\n\tsbc %1+2\n\t!jcc %a\n"
stmt:	LTU4(xareg, omem4) "\tcpx %1\n\tsbc %1+2\n\t!jcc %a\n"

stmt:	LTU4(poi41,vc)	"\tlda [%0]\n\tcmp #!%1\n\tldy #2\n\tlda [%0],y\n\tsbc #^%1\n\t!jcc %a\n"
stmt:	LTU4(poi41,mem4) "\tlda [%0]\n\tcmp %1\n\tldy #2\n\tlda [%0],y\n\tsbc %1+2\n\t!jcc %a\n"
stmt:	LTU4(poi41,xareg) "\t.(\n\tldy #2\n\tcmp [%0],y\n\tbcc skip\n\tbne made\n\ttxa\n\tcmp [%0]\n\tbcc skip\n\tbeq skip\nmade\tbrl %a\nskip\t.)\n"

stmt:	LEI4(mem4,vc)	"\t.(\n\tlda %0+2\n\tcmp #^%1\n\tbmi made\n\tbne skip\n\tlda %0\n\tsbc #!%1\n\tbcc made\n\tbne skip\nmade\t!jra %a\nskip\t.)\n"
stmt:	LEI4(omem4,xareg) "\t.(\n\tcmp %0+2\n\tbmi skip\n\tbne made\n\tcpx %0\n\tbcc skip\nmade\t!jra %a\nskip\t.)\n"
stmt:	LEI4(mem41,mem4) "\t.(\n\tlda %0+2\n\tcmp %1+2\n\tbmi made\n\tbne skip\n\tlda %0\n\tcmp %1\n\tbeq made\n\tbcs skip\nmade\t!jra %a\nskip\t.)\n"

stmt:	LEU4(mem4,vc)	"\t.(\n\tlda %0+2\n\tcmp #^%1\n\tbcc made\n\tbne skip\n\tlda %0\n\tcmp #!%1\n\tbcc made\n\tbne skip\nmade\t!jra %a\nskip\t.)\n"
stmt:	LEU4(omem4,xareg) "\t.(\n\tcmp %0+2\n\tbcc skip\n\tbne made\n\tcpx %0\n\tbcc skip\nmade\t!jra %a\nskip\t.)\n"
stmt:	LEU4(mem41,mem4) "\t.(\n\tlda %0+2\n\tcmp %1+2\n\tbcc made\n\tbne skip\n\tlda %0\n\tcmp %1\n\tbeq made\n\tbcs skip\nmade\t!jra %a\nskip\t.)\n"

stmt:	GTI4(mem4,vc)	"\t.(\n\tlda %0+2\n\tcmp #^%1\n\tbmi skip\n\tbne made\n\tlda %0\n\tcmp #!%1\n\tbcc skip\n\tbeq skip\nmade\tbrl %a\nskip\t.)\n"
stmt:	GTI4(mem41,mem4) "\t.(\n\tlda %0+2\n\tcmp %1+2\n\tbmi skip\n\tbne made\n\tlda %0\n\tcmp %1\n\tbcc skip\n\tbeq skip\nmade\tbrl %a\nskip\t.)\n"
stmt:	GTI4(vc,mem4)	"\t.(\n\tlda #^%0+2\n\tcmp %1+2\n\tbmi skip\n\tbne made\n\tlda #!%0\n\tcmp %1\n\tbcc skip\n\tbeq skip\nmade\tbrl %a\nskip\t.)\n"
stmt:	GTI4(poi41,mem4) "\t.(\n\tldy #2\n\tlda [%0],y\n\tcmp %1+2\n\tbmi skip\n\tbne made\n\tlda [%0]\n\tcmp %1\n\tbcc skip\n\tbeq skip\nmade\tbrl %a\nskip\t.)\n"

stmt:	GTU4(mem4,vc)	"\t.(\n\tlda %0+2\n\tcmp #^%1\n\tbcc skip\n\tbne made\n\tlda %0\n\tcmp #!%1\n\tbcc skip\n\tbeq skip\nmade\tbrl %a\nskip\t.)\n"
stmt:	GTU4(mem41,mem4) "\t.(\n\tlda %0+2\n\tcmp %1+2\n\tbcc skip\n\tbne made\n\tlda %0\n\tcmp %1\n\tbcc skip\n\tbeq skip\nmade\tbrl %a\nskip\t.)\n"
stmt:	GTU4(vc,mem4)	"\t.(\n\tlda #^%0+2\n\tcmp %1+2\n\tbcc skip\n\tbne made\n\tlda #!%0\n\tcmp %1\n\tbcc skip\n\tbeq skip\nmade\tbrl %a\nskip\t.)\n"
stmt:	GTU4(poi41,mem4) "\t.(\n\tldy #2\n\tlda [%0],y\n\tcmp %1+2\n\tbcc skip\n\tbne made\n\tlda [%0]\n\tcmp %1\n\tbcc skip\n\tbeq skip\nmade\tbrl %a\nskip\t.)\n"

stmt:	GEI4(mem4,vc)	"\t.(\n\tlda %0+2\n\tcmp #^%1\n\tbmi skip\n\tbne made\n\tlda %0\n\tcmp #!%1\n\tbcc skip\nmade\tbrl %a\nskip\t.)\n"
stmt:	GEI4(mem41,mem4) "\t.(\n\tlda %0+2\n\tcmp %1+2\n\tbmi skip\n\tbne made\n\tlda %0\n\tcmp %1\n\tbcc skip\nmade\tbrl %a\nskip\t.)\n"




stmt:	GEU4(mem4,vc)	"\t.(\n\tlda %0\n\tcmp #!%1\n\tlda %0+2\n\tsbc #^%1\n\tbcc skip\n\tbrl %a\nskip\t.)\n"
stmt:	GEU4(mem41,mem4) "\t.(\n\tlda %0\n\tcmp %1\n\tlda %0+2\n\tsbc %1+2\n\tbcc skip\n\tbrl %a\nskip\t.)\n"

stmt:	NEI4(xareg,vc)	"\tcpx #!%1\n\t!jne %a\n\tcmp #^%1\n\t!jne %a\n" 5
stmt:	NEI4(omem4,xareg) "\tcpx %0\n\t!jne %a\n\tcmp %0+2\n\t!jne %a\n" 5
stmt:	NEI4(poi41,vc)	"\tlda [%0]\n\tcmp #!%1\n\t!jne %a\n\tldy #2\n\tlda [%0],y\n\tcmp #^%1\n\t!jne %a\n" 5
stmt:	NEI4(mem4,vc)	"\tlda %0\n\tcmp #!%1\n\t!jne %a\n\tlda %0+2\n\tcmp #^%1\n\t!jne %a\n" 5
stmt:	NEI4(mem41,mem4) "\tlda %0\n\tcmp %1\n\t!jne %a\n\tlda %0+2\n\tcmp %1+2\n\t!jne %a\n" 5
stmt:	NEI4(mem4,con0)	"\tlda %0\n\tora %0+2\n\t!jne %a\n"

stmt:	NEU4(xareg,vc)	"\tcpx #!%1\n\t!jne %a\n\tcmp #^%1\n\t!jne %a\n" 5
stmt:	NEU4(omem4,xareg) "\tcpx %0\n\t!jne %a\n\tcmp %0+2\n\t!jne %a\n" 5
stmt:	NEU4(poi41,vc)	"\tlda [%0]\n\tcmp #!%1\n\t!jne %a\n\tldy #2\n\tlda [%0],y\n\tcmp #^%1\n\t!jne %a\n" 5
stmt:	NEU4(mem4,vc)	"\tlda %0\n\tcmp #!%1\n\t!jne %a\n\tlda %0+2\n\tcmp #^%1\n\t!jne %a\n" 5
stmt:	NEU4(mem41,mem4) "\tlda %0\n\tcmp %1\n\t!jne %a\n\tlda %0+2\n\tcmp %1+2\n\t!jne %a\n" 5
stmt:	NEU4(mem4,con0)	"\tlda %0\n\tora %0+2\n\t!jne %a\n"

stmt:	EQI4(mem4,vc)	"\t.(\n\tlda %0\n\tcmp #!%1\n\tbne skip\n\tlda %0+2\n\tcmp #^%1\n\t!jeq %a\nskip\t.)\n" 5
stmt:	EQI4(poi41,vc)	"\t.(\n\tlda [%0]\n\tcmp #!%1\n\tbne skip\n\tldy #2\n\tlda [%0],y\n\tcmp #^%1\n\t!jeq %a\nskip\t.)\n" 5
stmt:	EQI4(omem4,xareg) "\t.(\n\tcpx %0\n\tbne skip\n\tcmp %0+2\n\t!jeq %a\nskip\t.)\n" 5
stmt:	EQI4(mem4,con0)	"\tlda %0\n\tora %0+2\n\t!jeq %a\n"
stmt:	EQI4(xareg,con0) "\t.(\n\ttxy\n\tbne skip\n\ttay\n\t!jeq %a\nskip\t.)\n" 5
stmt:	EQI4(mem41,mem4) "\t.(\n\tlda %0\n\tcmp %1\n\tbne skip\n\tlda %0+2\n\tcmp %1+2\n\t!jeq %a\nskip\t.)\n" 5

stmt:	EQU4(mem4,vc)	"\t.(\n\tlda %0\n\tcmp #!%1\n\tbne skip\n\tlda %0+2\n\tcmp #^%1\n\t!jeq %a\nskip\t.)\n" 5
stmt:	EQU4(poi41,vc)	"\t.(\n\tlda [%0]\n\tcmp #!%1\n\tbne skip\n\tldy #2\n\tlda [%0],y\n\tcmp #^%1\n\t!jeq %a\nskip\t.)\n" 5
stmt:	EQU4(omem4,xareg) "\t.(\n\tcpx %0\n\tbne skip\n\tcmp %0+2\n\t!jeq %a\nskip\t.)\n" 5
stmt:	EQU4(mem4,con0)	"\tlda %0\n\tora %0+2\n\t!jeq %a\n"
stmt:	EQU4(xareg,con0) "\t.(\n\ttxy\n\tbne skip\n\ttay\n\t!jeq %a\nskip\t.)\n" 5
stmt:	EQU4(mem41,mem4) "\t.(\n\tlda %0\n\tcmp %1\n\tbne skip\n\tlda %0+2\n\tcmp %1+2\n\t!jeq %a\nskip\t.)\n" 5
%%

static int shareseg(Node p) {
	Symbol sym=p->syms[0];
	
	if (sym->scope == GLOBAL && sym->type->op == 11) 
		return 1;
	if (shsegs)
		return LBURG_MAX;
	return 1;	
}

static int segshare(Node p) {
/*	Symbol sym=p->syms[0];
	
	if (sym->scope == GLOBAL) {
		fprintf(stderr,"name = %s\n",sym->name);
		fprintf(stderr,"seg = %d\n",sym->u.seg);
		fprintf(stderr,"scope = %d\n",sym->scope);
		fprintf(stderr,"sclass = %d\n",sym->sclass);
		fprintf(stderr,"op = %d\n",sym->type->op);
	} */
	if (shsegs)
		return 5;
	return LBURG_MAX;
}

static void progbeg(int argc, char *argv[]) {
        int i;

        {
                union {
                        char c;
                        int i;
                } u;
                u.i = 0;
                u.c = 1;
                swap = ((int)(u.i == 1)) != IR->little_endian;
        }
	shsegs=0;
	for (i=0;i<argc;i++) {
		if (!strcmp(argv[i],"-pic"))
			shsegs=1;
	}
        parseflags(argc, argv);
	for (i=0;i<32;i++)
	{
		intreg[i] = mkreg("%d", i, 0, IREG);
		shortreg[i] = mkreg("%d", i, 0, IREG);
		charreg[i] = mkreg("%d", i, 0, IREG);
		fltreg[i] = mkreg("%d", i, 0, FREG);
		
	}
/*        intreg[MREG] = mkreg("mreg", MREG, 1, IREG);
	intreg[MREG2] = mkreg("mreg2", MREG2, 1, IREG);
	intreg[MREG3] = mkreg("mreg3", MREG3, 1, IREG);
	intreg[M4] = mkreg("m4", M4, 1, IREG);
	intreg[M5] = mkreg("m5", M5, 1, IREG);
	intreg[M6] = mkreg("m6", M6, 1, IREG);
        shortreg[MREG] = mkreg("mreg", MREG, 1, IREG);
        shortreg[MREG2] = mkreg("mreg2", MREG2, 1, IREG);
        shortreg[MREG3] = mkreg("mreg3", MREG3, 1, IREG);
	shortreg[M4] = mkreg("m4", M4, 1, IREG);
	shortreg[M5] = mkreg("m5", M5, 1, IREG);
	shortreg[M6] = mkreg("m6", M6, 1, IREG);
        charreg[MREG] = mkreg("mreg", MREG, 1, IREG);
        charreg[MREG2] = mkreg("mreg2", MREG2, 1, IREG);
        charreg[MREG3] = mkreg("mreg3", MREG3, 1, IREG);
	charreg[M4] = mkreg("m4", M4, 1, IREG);
	charreg[M5] = mkreg("m5", M5, 1, IREG);
	charreg[M6] = mkreg("m6", M6, 1, IREG);
	fltreg[MREG] = mkreg("mreg", MREG, 1, FREG);
        fltreg[MREG2] = mkreg("mreg2", MREG2, 1, FREG);
        fltreg[MREG3] = mkreg("mreg3", MREG3, 1, FREG); */
	
        charregw = mkwildcard(charreg);
        shortregw = mkwildcard(shortreg);
        intregw = mkwildcard(intreg);
        fltregw = mkwildcard(fltreg);
        tmask[IREG] = (1<<MREG) | (1<<MREG2) | (1<<MREG3) | (1<<M4) | (1<<M5)
		| (1<<M6);
        tmask[FREG] = (1<<MREG) | (1<<MREG2) | (1<<MREG3) | (1<<M4) | (1<<M5)
		| (1<<M6);
        vmask[IREG] = 0;
        vmask[FREG] = 0;

        cseg = 0;
	print("\n\t.(\n\nmreg\t= 1\nmreg2 \t= 5\n\n\t.al\n\t.xl\n");
	print("\n\t.include <65816.i65>\n\t.include <lcc.i65>\n\n");
	if (shsegs) {
		print("\t.text\n\t.pic\n\n");
		cseg = CODE;
	}
}
static Symbol rmap(int opk) {
        switch (optype(opk)) {
        case B: case P:
                return intregw;
        case I: case U:
                if (opsize(opk) == 1)
                        return charregw;
                else if (opsize(opk) == 2)
                        return shortregw;
                else
                        return intregw;
        default:
                return 0;
        }
}

static void segment(int n) {
        if (n == cseg)
                return;
        cseg = n;
        if (cseg == CODE || (cseg == LIT && !shsegs))
                print("\t.text\n");
        else if (cseg == DATA || (cseg == LIT && shsegs))
                print("\t.data\n");
	else if (cseg == BSS)
		print("\t.bss\n");	
}
static void progend(void) {
	print("\t.)\n");
}

static void target(Node p) {
	return;
}

static void clobber(Node p) {
	return;
}

static void emit2(Node p) {
	return;
}

static void doarg(Node p) {
        assert(p && p->syms[0]);
        mkactual(2, p->syms[0]->u.c.v.i); 
}
static void blkfetch(int k, int off, int reg, int tmp) {}
static void blkstore(int k, int off, int reg, int tmp) {}
static void blkloop(int dreg, int doff, int sreg, int soff,
        int size, int tmps[]) {}
static void local(Symbol p) {
	debug(fprintf(stderr,"local\n"));
        if (isfloat(p->type))
                p->sclass = AUTO;
	if (1) {
		p->sclass=AUTO;
		assert(p->sclass == AUTO);
		p->x.offset = offset;
		p->x.name = stringf("RZ+%d",offset+1);
		offset = roundup(offset + p->type->size, p->type->align);
	}
}

static void function(Symbol f, Symbol caller[], Symbol callee[], int n) {
        int i;

        print("\n%s\t.(\n\n", f->x.name);
        usedmask[0] = usedmask[1] = 0;
        freemask[0] = freemask[1] = ~(unsigned)0;
	mreg2=mreg=0;
        offset = 5;
        for (i = 0; callee[i]; i++) {
                Symbol p = callee[i];
                Symbol q = caller[i];
                assert(q);
                p->x.offset = q->x.offset = offset;
                p->x.name = q->x.name = stringf("LZ+%d", p->x.offset+1);
                p->sclass = q->sclass = AUTO;
                offset += roundup(q->type->size, 2);
        }
        assert(caller[i] == 0);
        offset = maxoffset = 0;
        gencode(caller, callee);
	debug(fprintf(stderr,"gencode\n"));
        framesize = maxoffset;
	framesize +=8;
	if (framesize > 256) {
		fprintf(stderr, "Framesize >256 (%d) for \"%s!\"\n", framesize, f->x.name);
		exit(1);
	}
	print("RZ \t= %d\n",8);
	print("LZ \t= %d\n\n\t!PRO LZ\n", framesize);
        emitcode();
	print("\t!POST LZ\n\trtl\n\t.)\n");
}
static void defsymbol(Symbol p) {
        if (p->scope >= LOCAL && p->sclass == STATIC)
                p->x.name = stringf("L%d", genlabel(1));
        else if (p->generated)
                p->x.name = stringf("L%s", p->name);
        else if (p->scope == GLOBAL || p->sclass == EXTERN)
                p->x.name = stringf("_%s", p->name);
        else if (p->scope == CONSTANTS
        && (isint(p->type) || isptr(p->type))
        && p->name[0] == '0' && p->name[1] == 'x')
                p->x.name = stringf("$%s", &p->name[2]);
        else
                p->x.name = p->name;
}
static void address(Symbol q, Symbol p, long n) {
        if (p->scope == GLOBAL
        || p->sclass == STATIC || p->sclass == EXTERN)
                q->x.name = stringf("%s%s%D",
                        p->x.name, n >= 0 ? "+" : "", n);
        else {
                assert(n <= INT_MAX && n >= INT_MIN);
                q->x.offset = p->x.offset + n;
                q->x.name = stringf("%s%s%D",
                        p->x.name, n >= 0 ? "+" : "", n);
        }
}
static void defconst(int suffix, int size, Value v) {
        if (suffix == I && size == 1)
                print("\t.byte %d\n",   v.u);
        else if (suffix == I && size == 2)
                print("\t.word %d\n",   v.i);
        else if (suffix == I && size == 4)
                print("\t.long %d\n",   v.i);
        else if (suffix == U && size == 1)
                print("\t.byte %d\n", v.u);
        else if (suffix == U && size == 2)
                print("\t.word %d\n", v.u);
        else if (suffix == U && size == 4)
                print("\t.long $%x\n", v.u);
        else if (suffix == P && size == 4)
                print("\t.long $%x\n", v.p);
        else if (suffix == F && size == 4) {
                float f = v.d;
                print("\t.long $%x\n", *(unsigned *)&f);
        }
        else if (suffix == F && size == 8) {
                double d = v.d;
                unsigned *p = (unsigned *)&d;
                print("\t.long $%x\n\t.long $%x\n", p[swap], p[!swap]);
        }
        else assert(0);
}
static void defaddress(Symbol p) {
        print("\t.long %s\n", p->x.name);
}
static void defstring(int n, char *str) {
        char *s;
	int mode=0;
	int val;
	int line=0;

	print("\t.asc \"");
        for (s = str; s < str + n; s++) {
		val = (*s)&0377;
		if (val>=0x20 && val<=0x7f && val !='\\' && val !='\"') {
			if (mode) {
				print(",\"");
				mode=0;
			}
	                print("%c", val);
		} else {
			if (!mode) {
				print("\"");
				mode=1;
			}
			print(",%d", val);
		}
		if (++line >= 60) {
			line=0;
			if (!mode)
				print("\"");
			print("\n\t.asc \"");
			mode=0;
		}
		
	}
	if (!mode)
		print("\"");
	print("\n");
}
static void export(Symbol p) {
	print("\t.export %s\n", p->x.name);
}

static void import(Symbol p) {
}

static void global(Symbol p) {
        print("%s", p->x.name);
        if (p->u.seg == BSS)
                print("\t.dsb %d\n", p->type->size);
}
static void space(int n) {
        if (cseg != BSS)
                print("\t.dsb %d\n", n);
}
Interface mipsebIR = {
        1, 1, 0,  /* char */
        2, 1, 0,  /* short */
        2, 1, 0,  /* int */
        4, 1, 0,  /* long */
        4, 1, 0,  /* long long */
        4, 1, 1,  /* float */
        8, 1, 1,  /* double */
        8, 1, 0,  /* long double */
        4, 1, 0,  /* T * */
        0, 1, 0,  /* struct; so that ARGB keeps stack aligned */
        1,        /* little_endian */
        0,        /* mulops_calls */
        0,        /* wants_callb */
        1,        /* wants_argb */
        0,        /* left_to_right */
        0,        /* wants_dag */
        0,        /* unsigned_char */
        address,
        blockbeg,
        blockend,
        defaddress,
        defconst,
        defstring,
        defsymbol,
        emit,
        export,
        function,
        gen,
        global,
        import,
        local,
        progbeg,
        progend,
        segment,
        space,
        0, 0, 0, 0, 0, 0, 0,
        {1, rmap,
            blkfetch, blkstore, blkloop,
            _label,
            _rule,
            _nts,
            _kids,
            _string,
            _templates,
            _isinstruction,
            _ntname,
            emit2,
            doarg,
            target,
            clobber,
}
};

Interface mipselIR = {
        1, 1, 0,  /* char */
        2, 1, 0,  /* short */
        2, 1, 0,  /* int */
        4, 1, 0,  /* long */
        4, 1, 0,  /* long long */
        4, 1, 1,  /* float */
        8, 1, 1,  /* double */
        8, 1, 1,  /* long double */
        4, 1, 0,  /* T * */
        0, 1, 0,  /* struct; so that ARGB keeps stack aligned */
        1,        /* little_endian */
        0,        /* mulops_calls */
        0,        /* wants_callb */
        1,        /* wants_argb */
        0,        /* left_to_right */
        0,        /* wants_dag */
        0,        /* unsigned_char */
        address,
        blockbeg,
        blockend,
        defaddress,
        defconst,
        defstring,
        defsymbol,
        emit,
        export,
        function,
        gen,
        global,
        import,
        local,
        progbeg,
        progend,
        segment,
        space,
        0, 0, 0, 0, 0, 0, 0,
        {1, rmap,
            blkfetch, blkstore, blkloop,
            _label,
            _rule,
            _nts,
            _kids,
            _string,
            _templates,
            _isinstruction,
            _ntname,
            emit2,
            doarg,
            target,
            clobber,
}
};
static char rcsid[] = "$Id: mips.md,v 1.4 2004/02/18 11:50:27 jmaginni Exp $";

