
#ifndef THREED_H
#define THREED_H
#include <cstdio>
class ThreeD{
public:
	virtual void readpar() = 0;
	virtual void alloc_mem() = 0;
	virtual void write(FILE* out) = 0;

};

#endif
