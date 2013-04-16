
#pragma once

#include "assert.h"
#define _DEBUG_BREAK_IF( _CONDITION_ ) assert( !(_CONDITION_) );


#ifdef _MSC_VER
typedef unsigned __int32	u32;
#else
typedef unsigned int		u32;
#endif

#ifdef _MSC_VER
typedef __int32			s32;
#else
#endif

//! Very simple allocator implementation, containers using it can be used across dll boundaries
template<typename T>
class Allocator_
{
public:

	//! Destructor
	virtual ~Allocator_() {}

	//! Allocate memory for an array of objects
	T* allocate(size_t cnt)
	{
		return (T*)internal_new(cnt* sizeof(T));
	}

	//! Deallocate memory for an array of objects
	void deallocate(T* ptr)
	{
		internal_delete(ptr);
	}

	//! Construct an element
	void construct(T* ptr, const T&e)
	{
		new ((void*)ptr)T(e);

	}

	//! Destruct an element
	void destruct(T* ptr)
	{
		ptr->~T();
	}

protected:

	virtual void* internal_new(size_t cnt)
	{
		return operator new(cnt);
	}

	virtual void internal_delete(void* ptr)
	{
		operator delete(ptr);
	}

};

namespace heap_
{
	//! Sinks an element into the heap.
	template<class T>
		inline void heapsink(T*array, s32 element, s32 max)
	{
		while ((element<<1) < max) // there is a left child
		{
			s32 j = (element<<1);

			// ���� �������� �� ũ�ٸ�, j ���� �������� ����Ű�� �Ѵ�.
			if (j+1 < max && array[j] < array[j+1])
				j = j+1; // take right child

			// element ���� ������ j������ ������ �����Ѵ�.
			// ��, ū������ �������� ä�� ����
			if (array[element] < array[j])
			{
				T t = array[j]; // swap elements
				array[j] = array[element];
				array[element] = t;
				element = j;
			}
			else
				return;
		}
	}

	//! Sinks an element into the heap sort by key.
	template<class T>
		inline void heapsinkbykey(T*array, s32 element, s32 max)
	{
		while ((element<<1) < max) // there is a left child
		{
			s32 j = (element<<1);

			// ���� �������� �� ũ�ٸ�, j ���� �������� ����Ű�� �Ѵ�.
			if (j+1 < max && array[j].key < array[j+1].key)
				j = j+1; // take right child

			// element ���� ������ j������ ������ �����Ѵ�.
			// ��, ū������ �������� ä�� ����
			if (array[element].key < array[j].key)
			{
				T t = array[j]; // swap elements
				array[j] = array[element];
				array[element] = t;
				element = j;
			}
			else
				return;
		}
	}

	// heapsort
	template<class T>
		inline void heapsort(T* array_, s32 size)
	{
		// for heapsink we pretent this is not c++, where
		// arrays start with index 0. So we decrease the array pointer,
		// the maximum always +2 and the element always +1

		T* virtualArray = array_ - 1;
		s32 virtualSize = size + 2;
		s32 i;

		// build heap
		for (i=((size-1)/2); i>=0; --i)
			heapsink(virtualArray, i+1, virtualSize-1);

		for (i=size-1; i>=0; --i)
		{
			T t = array_[0];
			array_[0] = array_[i];
			array_[i] = t;
			heapsink(virtualArray, 1, i + 1);
		}
	}

	// heapsort
	template<class T>
		inline void heapsortbykey(T* array_, s32 size)
	{
		// for heapsink we pretent this is not c++, where
		// arrays start with index 0. So we decrease the array pointer,
		// the maximum always +2 and the element always +1

		T* virtualArray = array_ - 1;
		s32 virtualSize = size + 2;
		s32 i;

		// build heap
		for (i=((size-1)/2); i>=0; --i)
			heapsinkbykey(virtualArray, i+1, virtualSize-1);

		for (i=size-1; i>=0; --i)
		{
			T t = array_[0];
			array_[0] = array_[i];
			array_[i] = t;
			heapsinkbykey(virtualArray, 1, i + 1);
		}
	}
}

template <class T, typename TAlloc = Allocator_<T> >
class array
{

public:

	//! Default constructor for empty array.
	array()
		: data(0), allocated(0), used(0),
		free_when_destroyed(true), is_sorted(true)
	{
	}

	//! Constructs an array and allocates an initial chunk of memory.
	/** \param start_count Amount of elements to pre-allocate. */
	array(u32 start_count)
		: data(0), allocated(0), used(0),
		free_when_destroyed(true), is_sorted(true)
	{
		reallocate(start_count);
	}


	//! Copy constructor
	array(const array<T>& other)
		: data(0)
	{
		*this = other;
	}



	//! Destructor.
	/** Frees allocated memory, if set_free_when_destroyed was not set to
	false by the user before. */
	~array()
	{
		if (free_when_destroyed)
		{
			for (u32 i=0; i<used; ++i)
				allocator.destruct(&data[i]);

			allocator.deallocate(data);
		}
	}


	//! Reallocates the array, make it bigger or smaller.
	/** \param new_size New size of array. */
	void reallocate(u32 new_size)
	{
		T* old_data = data;

		data = allocator.allocate(new_size); //new T[new_size];
		allocated = new_size;

		// copy old data
		s32 end = used < new_size ? used : new_size;

		for (s32 i=0; i<end; ++i)
		{
			// data[i] = old_data[i];
			//allocator.construct(&data[i], old_data[i]);
			data[i] = old_data[i];
		}

		// destruct old data
		for (u32 j=0; j<used; ++j)
			allocator.destruct(&old_data[j]);

		if (allocated < used)
			used = allocated;

		allocator.deallocate(old_data); //delete [] old_data;
	}


	//! Adds an element at back of array.
	/** If the array is too small to add this new element it is made bigger.
	\param element: Element to add at the back of the array. */
	void push_back(const T& element)
	{
		if (used + 1 > allocated)
		{
			// reallocate(used * 2 +1);	// �Է� �ε����� �ʰ� �Ѱ�� ���� 2�� +1 ���� ��ŭ buffer ����.
			// this doesn't work if the element is in the same array. So
			// we'll copy the element first to be sure we'll get no data
			// corruption

			T e(element);
			reallocate(used * 2 +1); // increase data block

			//allocator.construct(&data[used++], e); // data[used++] = e; // push_back
			data[used++] = e;
		}
		else
		{
			//data[used++] = element;
			// instead of using this here, we copy it the safe way:
			//allocator.construct(&data[used++], element);
			data[used++] = element;
		}

		is_sorted = false;
	}


	//! Adds an element at the front of the array.
	/** If the array is to small to add this new element, the array is
	made bigger. Please note that this is slow, because the whole array
	needs to be copied for this.
	\param element Element to add at the back of the array. */
	void push_front(const T& element)
	{
		insert(element);
	}


	//! Insert item into array at specified position.
	/** Please use this only if you know what you are doing (possible
	performance loss). The preferred method of adding elements should be
	push_back().
	\param element: Element to be inserted
	\param index: Where position to insert the new element. */
	void insert(const T& element, u32 index=0)
	{
		_DEBUG_BREAK_IF(index>used) // access violation

			if (used + 1 > allocated)
				reallocate(used +1);

		for (u32 i=used; i>index; --i)
		{
			if (i<used)
			{
				allocator.destruct(&data[i]);
			}
			//allocator.construct(&data[i], data[i-1]); // data[i] = data[i-1];
			data[i] = data[i-1];
		}

		if (used > index)
		{
			allocator.destruct(&data[index]);
		}
		//allocator.construct(&data[index], element); // data[index] = element;
		data[index] = element;
		is_sorted = false;
		++used;
	}


	//! Clears the array and deletes all allocated memory.
	void clear()
	{
		for (u32 i=0; i<used; ++i)
			allocator.destruct(&data[i]);

		allocator.deallocate(data); // delete [] data;
		data = 0;
		used = 0;
		allocated = 0;
		is_sorted = true;
	}


	//! Sets pointer to new array, using this as new workspace.
	/** \param newPointer: Pointer to new array of elements.
	\param size: Size of the new array. */
	void set_pointer(T* newPointer, u32 size)
	{
		for (u32 i=0; i<used; ++i)
			allocator.destruct(&data[i]);

		allocator.deallocate(data); // delete [] data;
		data = newPointer;
		allocated = size;
		used = size;
		is_sorted = false;
	}


	//! Sets if the array should delete the memory it uses upon destruction.
	/** \param f If true, the array frees the allocated memory in its
	destructor, otherwise not. The default is true. */
	void set_free_when_destroyed(bool f)
	{
		free_when_destroyed = f;
	}


	//! Sets the size of the array and allocates new elements if necessary.
	/** Please note: This is only secure when using it with simple types,
	because no default constructor will be called for the added elements.
	\param usedNow Amount of elements now used. */
	void set_used(u32 usedNow)
	{
		if (allocated < usedNow)
			reallocate(usedNow);

		used = usedNow;
	}


	//! Assignment operator
	void operator=(const array<T>& other)
	{
		if (data)
		{
			for (u32 i=0; i<used; ++i)
				allocator.destruct(&data[i]);

			allocator.deallocate(data); // delete [] data;
		}

		//if (allocated < other.allocated)
		if (other.allocated == 0)
			data = 0;
		else
			data = allocator.allocate(other.allocated); // new T[other.allocated];

		used = other.used;
		free_when_destroyed = other.free_when_destroyed;
		is_sorted = other.is_sorted;
		allocated = other.allocated;

		for (u32 i=0; i<other.used; ++i)
		{
			//allocator.construct(&data[i], other.data[i]); // data[i] = other.data[i];
			data[i] = other.data[i];
		}
	}


	//! Equality operator
	bool operator == (const array<T>& other) const
	{
		if (used != other.used)
			return false;

		for (u32 i=0; i<other.used; ++i)
			if (data[i] != other[i])
				return false;
		return true;
	}

	//! Inequality operator
	bool operator != (const array<T>& other) const
	{
		return !(*this==other);
	}


	//! Direct access operator
	T& operator [](u32 index)
	{
		_DEBUG_BREAK_IF(index>=used) // access violation

			return data[index];
	}


	//! Direct const access operator
	const T& operator [](u32 index) const
	{
		_DEBUG_BREAK_IF(index>=used) // access violation

			return data[index];
	}


	//! Gets last element.
	T& getLast()
	{
		_DEBUG_BREAK_IF(!used) // access violation

			return data[used-1];
	}


	//! Gets last element
	const T& getLast() const
	{
		_DEBUG_BREAK_IF(!used) // access violation

			return data[used-1];
	}


	//! Gets a pointer to the array.
	/** \return Pointer to the array. */
	T* pointer()
	{
		return data;
	}


	//! Gets a const pointer to the array.
	/** \return Pointer to the array. */
	const T* const_pointer() const
	{
		return data;
	}


	//! Get size of array.
	/** \return Size of elements used in the array. */
	u32 size() const
	{
		return used;
	}


	//! Get amount of memory allocated.
	/** \return Amount of memory allocated. The amount of bytes
	allocated would be allocated_size() * sizeof(ElementsUsed); */
	u32 allocated_size() const
	{
		return allocated;
	}


	//! Check if array is empty.
	/** \return True if the array is empty false if not. */
	bool empty() const
	{
		return used == 0;
	}


	//! Sorts the array using heapsort.
	/** There is no additional memory waste and the algorithm performs
	O(n*log n) in worst case. */
	void sort()
	{
		if (is_sorted || used<2)
			return;

		heap_::heapsort(data, used);
		is_sorted = true;
	}


	// �߰� : binary search�� T Data�� ������ key�� �������� ��ȯ
	void sortbykey()
	{
		if (is_sorted || used<2)
			return;

		heap_::heapsortbykey(data, used);
		is_sorted = true;
	}


	//! Performs a binary search for an element, returns -1 if not found.
	/** The array will be sorted before the binary search if it is not
	already sorted.
	\param element Element to search for.
	\return Position of the searched element if it was found,
	otherwise -1 is returned. */
	s32 binary_search(const T& element)
	{
		sort();
		return binary_search(element, 0, used-1);
	}

	// �߰� : binary search�� T Data�� ������ key�� �������� ��ȯ
	template<typename K>
		s32 binary_search_bykey_returnindex(const K key)
	{
		sortbykey();
		return binary_search_bykey_core(key, 0, used-1);
	}

	// �߰� : binary search�� T Data�� ������ key�� �������� ��ȯ
	template<typename K>
		T& binary_search_bykey_returnvalue(const K key )
	{
		sortbykey();
		s32 ikey = binary_search_bykey_core(key, 0, used-1);
		_DEBUG_BREAK_IF( ikey == -1 )
		
		return data[ikey];
	}

	//! Performs a binary search for an element, returns -1 if not found.
	/** The array must be sorted prior
	\param element Element to search for.
	\return Position of the searched element if it was found, otherwise -1
	is returned. */
	s32 binary_search_const(const T& element) const
	{
		return binary_search_bykey_core(element, 0, used-1);
	}


	//! Performs a binary search for an element, returns -1 if not found.
	/** \param element: Element to search for.
	\param left First left index
	\param right Last right index.
	\return Position of the searched element if it was found, otherwise -1
	is returned. */
	s32 binary_search(const T& element, s32 left, s32 right) const
	{
		if (!used)
			return -1;

		s32 m;

		do
		{
			m = (left+right)>>1;

			if (element < data[m])
				right = m - 1;
			else
				left = m + 1;

		} while((element < data[m] || data[m] < element) && left<=right);

		// this last line equals to:
		// " while((element != array[m]) && left<=right);"
		// but we only want to use the '<' operator.
		// the same in next line, it is "(element == array[m])"

		if (!(element < data[m]) && !(data[m] < element))
			return m;

		return -1;
	}

	// �߰� : binary search�� T Data�� ������ key�� �������� ��ȯ
	template<typename K>
		s32 binary_search_bykey_core(const K key, s32 left, s32 right) const
	{
		if (!used)
			return -1;

		s32 m;

		do
		{
			m = (left+right)>>1;

			if (key < data[m].key)
				right = m - 1;
			else
				left = m + 1;

		} while((key < data[m].key || data[m].key < key) && left<=right);

		// this last line equals to:
		// " while((element != array[m]) && left<=right);"
		// but we only want to use the '<' operator.
		// the same in next line, it is "(element == array[m])"

		if (!(key < data[m].key) && !(data[m].key < key))
			return m;

		return -1;
	}


	//! Finds an element in linear time, which is very slow.
	/** Use binary_search for faster finding. Only works if ==operator is
	implemented.
	\param element Element to search for.
	\return Position of the searched element if it was found, otherwise -1
	is returned. */
	s32 linear_search(const T& element) const
	{
		for (u32 i=0; i<used; ++i)
			if (element == data[i])
				return (s32)i;

		return -1;
	}


	//! Finds an element in linear time, which is very slow.
	/** Use binary_search for faster finding. Only works if ==operator is
	implemented.
	\param element: Element to search for.
	\return Position of the searched element if it was found, otherwise -1
	is returned. */
	s32 linear_reverse_search(const T& element) const
	{
		for (s32 i=used-1; i>=0; --i)
			if (data[i] == element)
				return i;

		return -1;
	}


	//! Erases an element from the array.
	/** May be slow, because all elements following after the erased
	element have to be copied.
	\param index: Index of element to be erased. */
	void erase(u32 index)
	{
		_DEBUG_BREAK_IF(index>=used) // access violation

			for (u32 i=index+1; i<used; ++i)
			{
				allocator.destruct(&data[i-1]);
				//allocator.construct(&data[i-1], data[i]); // data[i-1] = data[i];
				data[i-1] = data[i];
			}

			allocator.destruct(&data[used-1]);

			--used;
	}


	//! Erases some elements from the array.
	/** May be slow, because all elements following after the erased
	element have to be copied.
	\param index: Index of the first element to be erased.
	\param count: Amount of elements to be erased. */
	void erase(u32 index, s32 count)
	{
		_DEBUG_BREAK_IF(index>=used || count<1 || index+count>used) // access violation

			u32 i;
		for (i=index; i<index+count; ++i)
			allocator.destruct(&data[i]);

		for (i=index+count; i<used; ++i)
		{
			if (i > index+count)
				allocator.destruct(&data[i-count]);

			//allocator.construct(&data[i-count], data[i]); // data[i-count] = data[i];
			data[i-count] = data[i];

			if (i >= used-count)
				allocator.destruct(&data[i]);
		}

		used-= count;
	}


	//! Sets if the array is sorted
	void set_sorted(bool _is_sorted)
	{
		is_sorted = _is_sorted;
	}

private:

	T* data;
	u32 allocated;
	u32 used;
	bool free_when_destroyed;
	bool is_sorted;
	TAlloc allocator;
};


//#include <math.h>
#include <stdlib.h>

inline int	GetRandNumber_( int iMin, int iMax, int iExceptNumber )
{	
	int iResultNumber_ = 0;
	do 
	{
		iResultNumber_ = (rand() % (iMax - iMin)) + iMin;
	}while( iResultNumber_ == iExceptNumber );

	return iResultNumber_;
}

template<typename T>
void	ItemSwap(T &TA, T &TB)
{
	T TC;
	int iSize = sizeof( T );
	memcpy( &TC, &TB, iSize );
	memcpy( &TB, &TA, iSize );
	memcpy( &TA, &TC, iSize );
}

template<typename T>
void	itemSwap(T *TA, T*TB)
{
	T TC;
	TC = (*TB);
	(*TB) = (*TA);
	(*TA) = TC;
}

template<typename T>
void	RandomMixArray_( array<T> &array__, int iMixTime = 1 )
{
	int iCnt = array__.size();

	for( int iTime = 0; iTime < iMixTime; ++iTime )
	{
		for( int i = 0; i < iCnt; ++i )
		{
			T &tA = array__[i];
			T &tB = array__[GetRandNumber_( 0, iCnt-1,i )];
			itemSwap( &tA, &tB );
		}
	}
}

