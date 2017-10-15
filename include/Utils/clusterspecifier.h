#pragma once
//#define LACONIA
//#define ABEL
#define OFFICE

#ifdef OFFICE
#define OUT_PREFIX "conf"
#endif
#ifdef LACONIA
#define OUT_PREFIX "$SCRATCH/pgfg/conf"
#endif

