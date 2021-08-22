#ifndef GLOBAL_H_
#define GLOBAL_H_

class GlobalList
{
	struct Elem
	{
		unsigned int id;
		GlobalList* elem;
		Elem* next,*prev;
		Elem(unsigned int a, GlobalList* b);
	};
	
	static Elem* first, *last;
	
protected:
	static unsigned int UkID;
	
public:
	void static put(GlobalList *c, unsigned int d);
	static GlobalList* get(unsigned int i);
	static GlobalList* remove(unsigned int i);
};



#endif
