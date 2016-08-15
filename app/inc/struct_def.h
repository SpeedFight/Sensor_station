#ifndef _struct_def_h
#define _struct_def_h

typedef struct 
{
	void (*init)();
	void (*send)(char *message);
}comm_typedef;


#endif
