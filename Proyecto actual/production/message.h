
#ifndef PORT
#define PORT 8889
#endif

#define MAXNAME 64

struct sync_message 
{
	char filename[MAXNAME];
	time_t mtime;
	int size;
};