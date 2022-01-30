
/*************************************************************************
 * Agent.C:
 * Generates prime numbers.  Pass the numbers through a chain of objects,
 * each of which takes in numbers and puts out only the numbers it 
 * considers relevant.
 ************************************************************************/

#include <CC/iostream.h>

class Agent {
/*
 * An Agent has a source from which it obtains numbers.  You can obtain
 * the next number in the sequence represented by an Agent by calling its
 * out member.
 */
public:
    Agent* source;                      // chain of agents
    Agent(Agent* a) { source = a; }     // constructor
    virtual int out() = 0;
};

class Counter: public Agent {
/*
 * A Counter is a kind of agent that generates an infinite sequence of 
 * values starting at a given pointer (default is 1).
 */
private:
    int value;
public:
    Counter (int v=1): Agent(0), value(v) {}
    int out() { return value++; }
};

class Filter: public Agent {
/*
 * A Filter is a kind of agent that passes only numbers not divisible by a 
 * value that is fixed when the Filter is created.
 */
private:
    int factor;
public:
    Filter(Agent* src, int f): Agent(src), factor(f) {}
    int out();
};

int 
Filter::out() {
    for (;;) {                      // infinite loop
	int n = source->out();
	if ( n % factor != 0 )      // if n is not divisible by factor, return n
	     return n;
    }
}

class Sieve: public Agent {
/*
 * A Sieve is a kind of agent that generates prime numbers.  For each number
 * it inserts a new filter to screen out multiples of that number in the 
 * future.
 */
public:
    Sieve(Agent* src): Agent(src) {}
    int out();
};

int 
Sieve::out() {
    int n = source->out();
    source = new Filter(source, n);
    return n;
}

/*--------------------------------------------------------------------------
 * Main program.
 *------------------------------------------------------------------------*/

main()
{
    Counter c(2);
    Sieve   s(&c);
    int next;

    do {
	next = s.out();
	cout << next << "\n";
    } while (next < 100);
}
