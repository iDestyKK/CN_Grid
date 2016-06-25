/*
 * CN_Grid Library (C++ Version)
 * 
 * Version 0.1.2 (Last Updated 2016-06-24)
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
	return data[(b * xsize) + a]; //Constant time lookup
}

template <typename T>
bool grid<T>::empty() {
	return (xsize * ysize) == 0;
}

//Set Functions
template <typename T>
void grid<T>::clear() {
	xsize = 0;
	ysize = 0;
	data.clear();
}

template <typename T>
typename grid<T>::iterator& grid<T>::begin() {
	grid<T>::iterator *__tmp = new grid<T>::iterator;
	
	__tmp->ptr = &data[0];
	return *__tmp;
}

template <typename T>
typename grid<T>::iterator& grid<T>::end() {
	grid<T>::iterator *__tmp = new grid<T>::iterator;
	
	__tmp->ptr = &data[data.size()];
	return *__tmp;
}

template <typename T>
typename grid<T>::reverse_iterator& grid<T>::rbegin() {
	grid<T>::reverse_iterator *__tmp = new grid<T>::reverse_iterator;
	
	__tmp->ptr = &data[data.size() - 1];
	return *__tmp;
}

template <typename T>
typename grid<T>::reverse_iterator& grid<T>::rend() {
	grid<T>::reverse_iterator *__tmp = new grid<T>::reverse_iterator;
	
	__tmp->ptr = &data[-1]; //Oh wow this should be good
	return *__tmp;
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
	return ptr->at_ext(this, b, pos);
}

//Private Functions
template <typename T>
void grid<T>::resize() {
	//TODO: Resize without messing up the data
	//Lazy way out
	data.resize(xsize * ysize);
}

//Protected Funcions
template <typename T>
T& grid<T>::at_ext(grid<T>::__tmp_ct* __t, unsigned int a, unsigned int b) {
	//This function is a HACK to overload [][] operator.
	delete __t; //Free memory
	return data[(b * xsize) + a];
}

//Iterator Subclass
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
