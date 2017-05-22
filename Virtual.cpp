// Virtual.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cassert>

#include <algorithm>
#include <vector>
#include <iostream>
#include <omp.h>

class Point {
public:
	virtual void move(int x, int y) {  };
	virtual void draw() {  };
private:
	int x, y;
};
class Colored {
public:
	virtual void setcolor(int c) {  };
private:
	int color;
};
class ColorPnt : public Point, public Colored {
public:
	virtual void draw() {  };
};

void display(int x) // function
{
	std::cout << x << ' ';
}
class Increment // functor
{
public:
	Increment(int i = 1) : step(i) {}
	void operator()(int& x) { x += step; }
private:
	int step;
};
# define CHUNKSIZE 10
# define N 100
using namespace std;

int main()
{
	Point * p = new Point();
	ColorPnt * cp = new ColorPnt();
	std::cout << "p:" << p << std::endl;
	std::cout << "cp:" << cp << std::endl;
	Colored * c = cp;
	std::cout << "c:" << c << std::endl;
	if (c == cp)
		std::cout << " equal !" << std::endl;
	else
		std::cout << " not   equal !" << std::endl;
	assert(sizeof(int) == sizeof(Point *));
	int ic = (int)c;
	int icp = (int)cp;
	if (ic == icp)
		std::cout << " equal !" << std::endl;
	else
		std::cout << " not   equal !" << std::endl;


	std::vector<int> v(5);
	for_each(v.begin(), v.end(), display);
	std::cout << std::endl;
	// with Increment constructor
	for_each(v.begin(), v.end(), Increment(2));
	for_each(v.begin(), v.end(), display);
	std::cout << std::endl;
	// with local Increment object
	Increment anIncrement(7);
	for_each(v.begin(), v.end(), anIncrement);
	for_each(v.begin(), v.end(), display);
	std::cout << std::endl;


	int nthreads, tid;
	omp_set_num_threads(4);
	/* Fork a team of threads giving them their own copies
	* of variables */
	# pragma omp parallel private ( nthreads , tid )
	{
		/* Obtain thread number */
		tid = omp_get_thread_num();
		cout << " Hello   parallel   world  -  from   thread  = " << tid
			<< endl;
		/* Only master thread does this */
		if (tid == 0) {
			nthreads = omp_get_num_threads();
			cout << " Number  of  threads  = " << nthreads << endl;
		}
	} /* All threads join master thread and disband */

	int i, chunk;
	float a[N], b[N], d[N];
	/* Some initializations */
	for (i = 0; i < N; i++)
		a[i] = b[i] = i * 1.0f;
	chunk = CHUNKSIZE;

	
	omp_set_num_threads(4); // Use 4 threads for all consecutive parallel regions
# pragma omp parallel shared (a,b,c, nthreads , chunk ) private (i, tid ) ... num_threads(4)
	{
		tid = omp_get_thread_num();
		if (tid == 0) {
			nthreads = omp_get_num_threads();
			cout << " Number  of  threads  = " << nthreads << endl;
		}
		cout << " Thread  " << tid << "  starting ... " << endl;
# pragma omp for schedule ( dynamic , chunk )... num_threads(4)
		for (i = 0; i<N; i++) {
			d[i] = a[i] + b[i];
			cout << " Thread  " << tid << ": c[" << i << "]= " << d[i] << endl;
		}
	} /* end of parallel section */

	int n;
	float sum;
	/* Some initializations */
	n = 100;
	for (i = 0; i < n; i++)
		a[i] = b[i] = i * 1.0f;
	sum = 0.0;
	# pragma omp parallel for reduction (+: sum )
	for (i = 0; i < n; i++)
		sum = sum + (a[i] * b[i]);
	std::cout << "    Sum = " << sum << std::endl;


	std::cout << "Number of Processors = " << omp_get_num_procs();

}

