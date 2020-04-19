#pragma once
#include <cassert>

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

	// --------------------
	// Constructor and destructor
	// --------------------
	Vector() :_size(0), _capacity(0), _data(nullptr) {}
	~Vector();
	Vector(const Vector& v);
	template<typename T2>
	Vector(const Vector<T2>& v);

	// --------------------
	// Member operator
	// --------------------
	SizeType size() const { return _size; }
	T& operator[](Rank r) const { assert(r < _size); return _data[r]; }
	T& front() const { return _data[0]; }
	T& back() const { return _data[_size-1]; }
	VectorIterator begin() const { return _data; }
	VectorIterator end() const { return _data + _size; }

	void push_back(const T& v);
	void pop_back();

	// --------------------
	// Algorithms
	// --------------------

private:
	SizeType _size;
	SizeType _capacity;	
	T* _data;

	void expand(SizeType n);
	SizeType getIncreasedCapacity()
	{
		SizeType newSize;
		if (_capacity == 0) newSize = 1;
		else newSize = _capacity * 2;
		return newSize;
	}
};

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
	_data = new T[_size];
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
	_data = new T[_size];
	for (SizeType i = 0; i < _size; ++i)
	{
		_data[i] = v[i];
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
	if (_size == _capacity) expand(getIncreasedCapacity());
	*(_data + _size) = v;
	++_size;
}

template<typename T>
inline void Vector<T>::pop_back()
{
	*(_data + _size - 1) = T(); // This should call destructor of original element
	--_size;
}