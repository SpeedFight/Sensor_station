#ifndef _struct_def_h
#define _struct_def_h

typedef struct 
{
	void (*init)();
	void (*send)(char *message);
	char *received;
}comm_typedef;


#endif
