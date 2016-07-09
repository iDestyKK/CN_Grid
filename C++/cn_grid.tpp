/*
 * CN_Grid Library (C++ Version)
 * 
 * Version 0.1.4 (Last Updated 2016-06-26)
 * 
 * Description:
 *     Implements a custom "grid-like" data type for C++ users.
 *     Uses functions a user is already familiar with using in STL.
 *     Treated as a vector with two dimensions instead of one.
 *     The size of the vector is (m * n). Constant time lookups.
 *
 *     This is a template definitions file (*.tpp). Appended to the
 *     end of cn_grid.hpp. Do not add it to the compiler as if it
 *     was a *.cpp file. The g++ compiler will do it on its own.
 *
 *     Changelog of library is located at the bottom of "cn_grid.hpp"
 *
 * Author:
 *     Clara Van Nguyen
 *
 * For documentation and details on every function in this library, visit the following URL:
 * http://web.eecs.utk.edu/~ssmit285/lib/cn_grid/cpp/index.html
 */

#ifndef __CN_GRID_TPP__
#define __CN_GRID_TPP__

#include "cn_grid.hpp"

//Destructor
template <typename T>
grid<T>::~grid() {
	_data.clear();
}

//Resize Functions
template <typename T>
void grid<T>::resizex(unsigned int i) {
	xsize = i;
	resize();
}

template <typename T>
void grid<T>::resizey(unsigned int i) {
	ysize = i;
	resize();
}

template <typename T>
void grid<T>::resize(unsigned int a, unsigned int b) {
	xsize = a;
	ysize = b;
	resize();
}

//Get Functions
template <typename T>
unsigned int grid<T>::sizex() {
	return xsize;
}

template <typename T>
unsigned int grid<T>::sizey() {
	return ysize;
}

template <typename T>
unsigned int grid<T>::size() {
	return (xsize * ysize);
}

template <typename T>
T& grid<T>::at(unsigned int a, unsigned int b) {
	return _data[(b * xsize) + a]; //Constant time lookup
}

template <typename T>
bool grid<T>::empty() {
	return (xsize * ysize) == 0;
}

template <typename T>
T& grid<T>::front() {
	return _data[0];
}

template <typename T>
T& grid<T>::back() {
	return _data[_data.size() - 1];
}

template <typename T>
vector<T>& grid<T>::get_vector() {
	return _data;
}

template <typename T>
T* grid<T>::data() {
	return _data.data();
}

//Set Functions
template <typename T>
void grid<T>::clear() {
	xsize = 0;
	ysize = 0;
	_data.clear();
}

//Iteration Functions
template <typename T>
typename grid<T>::iterator grid<T>::begin() {
	return iterator(&_data[0]);
}

template <typename T>
typename grid<T>::iterator grid<T>::end() {
	return iterator(&_data[_data.size()]);
}

template <typename T>
typename grid<T>::reverse_iterator grid<T>::rbegin() {
	return reverse_iterator(&_data[_data.size() - 1]);
}

template <typename T>
typename grid<T>::reverse_iterator grid<T>::rend() {
	return reverse_iterator(&_data[-1]);
}

//Memory Save/Read Functions
template <typename T>
void grid<T>::memory_write(const char* fpath) {
	//A method of dumping the grid to a file.
	FILE* fp = fopen(fpath, "wb");
	unsigned int __sz;
	__sz = sizeof(T);

	//File Header
	fprintf(fp, "GRID");

	//Size information
	fwrite((unsigned int *) &__sz , sizeof(unsigned int), 1, fp);
	fwrite((unsigned int *) &xsize, sizeof(unsigned int), 1, fp);
	fwrite((unsigned int *) &ysize, sizeof(unsigned int), 1, fp);

	//Memory Dump the entire grid
	fwrite((char *) &_data[0], sizeof(T), xsize * ysize, fp);
	fclose(fp);
}

template <typename T>
void grid<T>::memory_read(const char* fpath) {
	FILE* fp = fopen(fpath, "rb");
	
	//Header Check
	char header[5];
	header[4] = '\0';
	fread(header, sizeof(char), 4, fp);
	
	if (strcmp(header, "GRID") != 0) {
		//You are a failure
		fprintf(stderr, "[WARNING] Grid loaded does not have matching header! Will still attempt to load.\n");
	}

	unsigned int __sz, __xs, __ys;
	fread(&__sz, sizeof(unsigned int), 1, fp);
	fread(&__xs, sizeof(unsigned int), 1, fp);
	fread(&__ys, sizeof(unsigned int), 1, fp);

	if (__sz != sizeof(T)) {
		fprintf(stderr, "[ ERROR ] Grid file has data size %d (Required %d).\n", __sz, sizeof(T));
		fclose(fp);
		return;
	}
	
	//Prepare the grid
	_data.clear();
	_data.resize(xsize * ysize);
	xsize  = __xs;
	ysize  = __ys;
	oxsize = xsize;
	oysize = ysize;
	
	//Read the file into memory, directly into the vector's data.
	fread((char *) &_data[0], __sz, xsize * ysize, fp);

	fclose(fp);
}

//Operator Overloads
template <typename T>
typename grid<T>::__tmp_ct& grid<T>::operator[] (int a) {
	grid<T>::__tmp_ct* __to = new grid<T>::__tmp_ct;
	__to->ptr = this;
	__to->pos = a;

	return *__to;
}

template <typename T>
T& grid<T>::__tmp_ct::operator[] (int b) {
	//Surprisingly, this works
	return ptr->at_ext(this, pos, b);
}

//Private Functions
template <typename T>
void grid<T>::resize() {
	if (oysize == oxsize && oxsize == 0 && xsize > 0 && ysize > 0) {
		_data.resize(xsize * ysize);
		oxsize = xsize;
		oysize = ysize;
		return;
	}
	
	//Vertical first. It's the easiest.
	if (oysize != ysize)
		_data.resize(oxsize * ysize),
		oysize = ysize;
	
	//Horizontal next. This is where it gets a little tricky.
	//We are going to abuse the fact that a vector's memory
	//is contagious...
	if (oxsize != xsize) {
		typedef unsigned long long ullb;
		ullb pos1, pos2;
		unsigned int __sz;
		__sz = sizeof(T);

		if (xsize > oxsize) {
			//If the size is going up, resize first.
			_data.resize(xsize * ysize);

			pos1 = (ullb)&_data[0] + __sz * (xsize  * (ysize - 1));
			pos2 = (ullb)&_data[0] + __sz * (oxsize * (ysize - 1));
			for (unsigned int i = 1; i < ysize; i++) {
				memmove((char*)pos1, (char*)pos2, __sz * oxsize);
				memset((char*)pos2, 0, (xsize - oxsize));
				pos1 -= (__sz * xsize);
				pos2 -= (__sz * oxsize);
			}
		}
		else
		if (xsize < oxsize) {
			//If the size is going down, slide data over first
			pos1 = (ullb)&_data[0] + __sz * xsize;
			pos2 = (ullb)&_data[0] + __sz * oxsize;
			for (unsigned int i = 1; i < ysize; i++) {
				//Old-fashioned C code right here
				//Use 'memcpy' if you can guarantee it goes left-to-right (or if you are daring).
				memmove((char*)pos1, (char*)pos2, __sz * xsize); 
				pos1 += (__sz * xsize);
				pos2 += (__sz * oxsize);
			}
			
			//Resize last.
			_data.resize(xsize * ysize);
		}
		oxsize = xsize;
	}
}

//Protected Funcions
template <typename T>
T& grid<T>::at_ext(grid<T>::__tmp_ct* __t, unsigned int a, unsigned int b) {
	//This function is a HACK to overload [][] operator.
	delete __t; //Free memory
	return _data[(b * xsize) + a];
}

//Iterator Subclass
template <typename T>
grid<T>::iterator::iterator(T* rhs) {
	ptr = rhs;
}

template <typename T>
grid<T>::iterator::~iterator() {
	//delete this; //Suicide I say!
}

template <typename T>
typename grid<T>::iterator& grid<T>::iterator::operator=(const grid<T>::iterator& rhs) {
	ptr	= &rhs;
	return *this;
}

template <typename T>
typename grid<T>::iterator& grid<T>::iterator::operator++(int i) {
	ptr++;
	return *this;
}

template <typename T>
typename grid<T>::iterator& grid<T>::iterator::operator--(int i) {
	ptr--;
	return *this;
}

template <typename T>
bool grid<T>::iterator::operator!=(const grid<T>::iterator& rhs) const {
	return (ptr != rhs.ptr);
}

template <typename T>
bool grid<T>::iterator::operator==(const grid<T>::iterator& rhs) const {
	return (ptr == rhs.ptr);
}

template <typename T>
T& grid<T>::iterator::operator*() {
	return *ptr;
}

//Reverse Iterator Subclass
template <typename T>
grid<T>::reverse_iterator::reverse_iterator(T* rhs) {
	ptr = rhs;
}

template <typename T>
grid<T>::reverse_iterator::~reverse_iterator() {
	//delete this; //Suicide I say!
}

template <typename T>
typename grid<T>::reverse_iterator& grid<T>::reverse_iterator::operator=(const grid<T>::reverse_iterator& rhs) {
	ptr	= &rhs;
	return *this;
}

template <typename T>
typename grid<T>::reverse_iterator& grid<T>::reverse_iterator::operator++(int i) {
	ptr--;
	return *this;
}

template <typename T>
typename grid<T>::reverse_iterator& grid<T>::reverse_iterator::operator--(int i) {
	ptr++;
	return *this;
}

template <typename T>
bool grid<T>::reverse_iterator::operator!=(const grid<T>::reverse_iterator& rhs) const {
	return (ptr != rhs.ptr);
}

template <typename T>
bool grid<T>::reverse_iterator::operator==(const grid<T>::reverse_iterator& rhs) const {
	return (ptr == rhs.ptr);
}

template <typename T>
T& grid<T>::reverse_iterator::operator*() {
	return *ptr;
}

#endif
