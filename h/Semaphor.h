#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_

class KernelSem;

class Semaphore
{
public:
	Semaphore(int init = 1);
	virtual ~Semaphore();

	virtual void wait();
	virtual void signal();

	int val() const;
private:
	friend class KernelSem;
	unsigned ID;
};



#endif /* SEMAPHOR_H_ */
