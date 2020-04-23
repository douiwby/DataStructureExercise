#pragma once
#include <cassert>

#include "Algorithm.h"

template<typename T>
class Vector
{
public:
	// --------------------
	// Type declaration
	// --------------------
	typedef unsigned SizeType;
	typedef unsigned Rank;
	typedef T* VectorIterator;  //Todo: use iterator class
	typedef ptrdiff_t DifferenceType;

	// --------------------
	// Constructor and destructor
	// --------------------
	Vector() :_size(0), _capacity(0), _data(nullptr) {}
	Vector(SizeType n, const T& v);
	~Vector();
	Vector(const Vector& v);
	template<typename T2>
	Vector(const Vector<T2>& v);
	Vector(VectorIterator b, const VectorIterator& e);

	// --------------------
	// Member operator
	// --------------------
	SizeType size() const { return _size; }
	SizeType capacity() const { return _capacity; }
	T& operator[](Rank r) const { assert(r < _size); return _data[r]; }
	T& front() const { return _data[0]; }
	T& back() const { return _data[_size-1]; }
	VectorIterator begin() const { return _data; }
	VectorIterator end() const { return _data + _size; }

	void push_back(const T& v);
	void pop_back();

	void shrink_to_fit();

	VectorIterator insert(VectorIterator p, const T& t);
	VectorIterator insert(VectorIterator p,  VectorIterator b, const VectorIterator& e);
	VectorIterator erase(const VectorIterator& p);
	VectorIterator erase(const VectorIterator& b, const VectorIterator& e);

	// --------------------
	// Algorithms
	// --------------------
	VectorIterator find(const T& v) const { return Algorithm::find(begin(), end(), v); }

private:
	SizeType _size;
	SizeType _capacity;	
	T* _data;

	void expand(SizeType n);
	SizeType getIncreasedCapacity(SizeType currentCapacity)
	{
		SizeType newSize;
		const SizeType defaultSize = 4;
		if (currentCapacity == 0) newSize = defaultSize;
		else newSize = currentCapacity * 2;
		return newSize;
	}
};

template<typename T>
Vector<T>::Vector(SizeType n, const T& v)
{
	_size = _capacity = n;
	_data = new T[n];
	for (SizeType i = 0; i < n; ++i)
	{
		_data[i] = v;
	}
}

template<typename T>
Vector<T>::~Vector()
{
	if(_data) delete[] _data;
	_size = _capacity = 0;
}

template<typename T>
Vector<T>::Vector(const Vector& v)
{
	_size = _capacity = v.size();
	_data = new T[_capacity];
	for (SizeType i = 0; i < _size; ++i)
	{
		_data[i] = v[i];
	}
}

template<typename T>
template<typename T2>
Vector<T>::Vector(const Vector<T2>& v)
{
	_size = _capacity = v.size();
	_data = new T[_capacity];
	for (SizeType i = 0; i < _size; ++i)
	{
		_data[i] = v[i];
	}
}

template<typename T>
Vector<T>::Vector(VectorIterator b, const VectorIterator& e)
{
	if (b == e) return;
	DifferenceType diff = e - b;
	assert(diff >= 0);
	_capacity = diff;
	_data = new T[_capacity];
	_size = 0;
	while (b != e)
	{
		push_back(*b++);
	}
}

template<typename T>
inline void Vector<T>::expand(SizeType n)
{
	if (n <= _capacity) return;
	_capacity = n;
	T* oldData = _data;
	_data = new T[_capacity];
	for (SizeType i = 0; i < _size; ++i)
	{
		_data[i] = oldData[i];
	}
	if(oldData)	delete[] oldData;
}

template<typename T>
inline void Vector<T>::push_back(const T& v)
{
	if (_size == _capacity) expand(getIncreasedCapacity(_capacity));
	*(_data + _size) = v;
	++_size;
}

template<typename T>
inline void Vector<T>::pop_back()
{
	*(_data + _size - 1) = T(); // This should call destructor of original element
	--_size;
}

template<typename T>
inline void Vector<T>::shrink_to_fit()
{
	_capacity = 0;
	expand(_size);
}

template<typename T>
typename Vector<T>::VectorIterator Vector<T>::insert(VectorIterator p, const T& t)
{
	if (_size + 1 > _capacity)
	{
		// expend will make iterator unavailable, record it's position
		DifferenceType diff = p - begin();
		expand(getIncreasedCapacity(_capacity));
		p = begin() + diff;
	}
	VectorIterator last(_data + _size);
	VectorIterator beforeLast = last - 1;
	while (last != p)
	{
		*(last--) = *(beforeLast--);
	}
	*p = t;
	++_size;
	return p;
}

template<typename T>
typename Vector<T>::VectorIterator Vector<T>::insert(VectorIterator p, VectorIterator b, const VectorIterator& e)
{
	DifferenceType diff = e - b;
	if (diff == 0) return p;
	assert(diff >= 0);

	DifferenceType increasedCapacity = _capacity;
	while (increasedCapacity - _size < diff)
	{
		increasedCapacity = getIncreasedCapacity(increasedCapacity);
	}
	// expend will make iterator unavailable, record it's position
	DifferenceType diff2 = p - begin();
	expand(increasedCapacity);
	p = begin() + diff2;

	VectorIterator srcIt = p;
	VectorIterator destIt = p + diff;
	while (srcIt != end())
	{
		*destIt++ = *srcIt++;
	}

	VectorIterator it = p;
	while (b != e)
	{
		*it++ = *b++;
	}
	_size += diff;

	return p;
}

template<typename T>
inline typename Vector<T>::VectorIterator Vector<T>::erase(const VectorIterator& p)
{
	return erase(p, p + 1);
}

template<typename T>
typename Vector<T>::VectorIterator Vector<T>::erase(const VectorIterator& b, const VectorIterator& e)
{
	assert(b != end());
	DifferenceType diff = e - b;
	assert(diff >= 0);
	VectorIterator destIt = b;
	VectorIterator srcIt = e;
	while (srcIt != end())
	{
		*destIt++ = *srcIt++;
	}
	while (destIt != srcIt)
	{
		*destIt++ = T();
	}
	assert(diff <= _size);
	_size -= diff;
	return b;
}