/*
 * CN_Grid Library (C++ Version)
 * 
 * Version 0.1.0 (Last Updated 2016-06-23)
 * 
 * Description:
 *     Implements a custom "grid-like"  data type for C++ users.
 *     Uses functions a user is already familiar with using in STL.
 *     Treated as a vector with two dimensions instead of one.
 *     The size of the vector is (m * n). Constant time lookups.
 *
 *     Changelog of library is located at the bottom of this file.
 *
 * Author:
 *     Clara Van Nguyen
 *
 * For documentation and details on every function in this library, visit the following URL:
 * http://web.eecs.utk.edu/~ssmit285/lib/cn_grid/cpp/index.html
 */

#ifndef __CN_GRID_CPP__
#define __CN_GRID_CPP__

#include <vector>
#include <utility>

using namespace std;

template<typename T>
class grid {
	public:
		//Constructors/Initialization Lists
		grid():
			xsize(0), ysize(0) {};
		grid(unsigned int a, unsigned int b):
			xsize(a), ysize(b) { resize(); };

		//Resize Functions
		void resizex(unsigned int);
		void resizey(unsigned int);

		//Get Functions
		unsigned int sizex();
		unsigned int sizey();
		T& at(unsigned int, unsigned int);

		//Set Functions
		void clear();

		//Child Classes
		class __tmp_ct {
			//You should not use this class at ANY cost. It is a hack to allow
			//operator overloading of the [][] operator.
			public:
				__tmp_ct() {};
				unsigned int pos;
				grid<T>* ptr;

				//Operator Overloads
				T& operator[](int);
		};
		
		//Operator Overloads
		grid::__tmp_ct& operator[](int);


	private:
		unsigned int xsize, ysize;
		void resize();

	protected:
		vector<T> data;
		T& at_ext(grid::__tmp_ct*, unsigned int, unsigned int);
};

//A little hack to make the compiler shut up
#include "cn_grid.tpp"

#endif

/***************************************\
    * ** ***    CHANGELOG    *** ** *
\***************************************/

/*
    2016-06-23 (0.1.0)
      - Started writing CN_Grid (C++ Version). Initial Release.
*/
