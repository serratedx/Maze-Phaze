#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

template<class Datatype>class Array;
template<class Datatype>class Array2D;
template<class Datatype>class Array3D;
class Bitvector;
template<class Datatype>class SListNode;
template<class Datatype>class SLinkedList;
template<class Datatype>class SListIterator;
template<class Datatype>class DListNode;
template<class Datatype>class DLinkedList;
template<class Datatype>class DListIterator;
template<class Datatype>class LStack;
template<class Datatype>class AStack;
template<class Datatype>class LQueue;
template<class Datatype>class AQueue;
template<class Keytype, class Datatype>class HashEntry;
template<class Keytype, class Datatype>class HashTable;
class String;

//
//	Array template
//	
//	An array that supports any datatype
//	and some functionality
//
template<class Datatype>
class Array
{
private:
	Datatype* m_array;
	int m_size;

public:

	Array(int p_size) : m_array(new Datatype[p_size]), m_size(p_size) 
	{
	}

	~Array()
	{ 
		if (m_array != NULL) 
			delete[] m_array; 
		m_array = 0; 
	}

	void Resize(int p_size)
	{
		Datatype* newarray = new Datatype[p_size];
		if (newarray == NULL)
			return;
		int min;
		if (p_size < m_size)
			min = p_size;
		else
			min = m_size;
		for (int i = 0; i < min; i++)
			newarray[i] = m_array[i];
		m_size = p_size;
		if (m_array != NULL)
			delete[] m_array;
		m_array = newarray;
	}

	void Insert(Datatype p_item, int p_index)
	{
		for (int i = m_size - 1; i > p_index; i--)
			m_array[i] = m_array[i-1];
		m_array[p_index] = p_item;
	}

	void Remove(int p_index)
	{
		for (int i = p_index + 1; index < m_size; index++)
			m_array[i - 1] = m_array[i];
	}

	int Size()
	{
		return m_size;
	}

	bool WriteFile(const char* p_filename)
	{
		FILE* outfile = 0;
		int written = 0;
		outfile = fopen(p_filename, "wb");
		if (outfile == 0)
			return false;
		written = fwrite(m_array, sizeof(Datatype), m_size, outfile);
		fclose(outfile);
		if (written != m_size)
			return false;
		return true;
	}

	bool ReadFile(const char* p_filename)
	{
		FILE* infile = 0;
		int read = 0;
		infile = fopen(p_filename, "rb");
		if (infile == 0)
			return false;
		read = fread(m_array, sizeof(Datatype), m_size, infile);
		fclose(infile);
		if (read != m_size)
			return false;
		return true;
	}

	Datatype& operator[] (int p_index) { return m_array[p_index]; }
	operator Datatype* () { return m_array; }

};

//
//	2D Array template
//	
//	A 2D array that supports any datatype
//	and some functionality
//
template<class Datatype>
class Array2D
{
private:
	Datatype*	m_array;
	int			m_width;
	int			m_height;

public:
	Array2D()
		: m_array(new Datatype[1]),
		m_width(1), m_height(1)
	{
	}
	Array2D(int p_width, int p_height) 
		: m_array(new Datatype[p_width*p_height]),
		m_width(p_width), m_height(p_height) 
	{
	}

	~Array2D()
	{
		if (m_array != NULL)
			delete[] m_array;
		m_array = NULL;
	}

	Datatype& Get(int p_x, int p_y)
	{
		return m_array[p_y * m_width + p_x];
	}

	void Resize(int p_width, int p_height)
	{
		Datatype* newarray = new Datatype[p_width * p_height];
		if (newarray == NULL)
			return;
		int x, y, t1, t2;
		int minx = (p_width < m_width ? p_width : m_width);
		int miny = (p_height < m_height ? p_height : m_height);

		for (y = 0; y < miny; y++)
		{
			t1 = y * p_width;
			t2 = y * m_width;
			for (x = 0; x < minx; x++)
			{
				newarray[t1 + x] = m_array[t2 + x];
			}
		}
		if (m_array != NULL)
			delete[] m_array;
		m_array = newarray;
		m_width = p_width;
		m_height = p_height;
	}

	int Width() { return m_width; }
	int Height() { return m_height; }
	int Size() { return m_width * m_height; }
};

//
//	3D Array template
//	
//	A 3D array that supports any datatype
//	and some functionality
//
template<class Datatype>
class Array3D
{
private:
	Datatype*	m_array;
	int			m_width;
	int			m_height;
	int			m_depth;

public:
	Array3D(int p_width, int p_height, int p_depth) 
		: m_array(new Datatype[p_width*p_height*p_depth]),
		m_width(p_width), m_height(p_height), m_depth(p_depth) 
	{
	}

	~Array3D()
	{
		if (m_array != NULL)
			delete[] m_array;
		m_array = NULL;
	}

	Datatype& Get(int p_x, int p_y, int p_z)
	{
		return m_array[p_z * m_width * m_height 
			+ p_y * m_width 
			+ p_x];
	}

	void Resize(int p_width, int p_height, int p_depth)
	{
		Datatype* newarray = new Datatype[p_width * p_height * p_depth];
		if (newarray == NULL)
			return;
		int x, y, z, t1, t2, t3, t4;
		int minx = (p_width < m_width ? p_width : m_width);
		int miny = (p_height < m_height ? p_height : m_height);
		int minz = (p_depth < m_depth ? p_depth : m_depth);
		for (z = 0; z < minz; z++)
		{
			t1 = z * p_width * p_height;
			t2 = z * m_width * m_height;
			for (y = 0; y < miny; y++)
			{
				t3 = y * p_width;
				t4 = y * m_width;
				for (x = 0; x < minx; x++)
				{
					newarray[t1 + t3 + x] = m_array[t2 + + t4 + x];
				}
			}
		}

		if (m_array != NULL)
			delete[] m_array;
		m_array = newarray;
		m_width = p_width;
		m_height = p_height;
		m_depth = p_depth;
	}

	int Width() { return m_width; }
	int Height() { return m_height; }
	int Depth() { return m_depth; }
	int Size() { return m_width * m_height * m_depth; }
};

//
//	Bitvector
//	
//	Slower runtime, but more memory efficient
//	vs. boolean arrays
//
class Bitvector
{
private:
	unsigned long int*	m_array;
	int					m_size;

public:
	Bitvector(int p_size) : m_array(0), m_size(0) { Resize(p_size); }
	~Bitvector()
	{
		if (m_array != NULL)
			delete[] m_array;
		m_array = NULL;
	}

	void Resize(int p_size)
	{
		unsigned long int* newvector = NULL;
		if (p_size % 32 == 0)
			p_size = p_size / 32;
		else
			p_size = (p_size / 32) + 1;
		newvector = new unsigned long int[p_size];
		if (newvector == NULL)
			return;
		int min;
		if (p_size < m_size)
			min = p_size;
		else
			min = m_size;
		for (int i = 0; i < min; i++)
			newvector[i] = m_array[i];
		m_size = p_size;
		if (m_array != NULL)
			delete[] m_array;
		m_array = newvector;
	}

	void Set(int p_index, bool p_value)
	{
		int cell = p_index / 32;
		int bit = p_index % 32;
		if (p_value == true)
			m_array[cell] = (m_array[cell] | (1 << bit));
		else
			m_array[cell] = (m_array[cell] & (~(1 << bit)));
	}

	void ClearAll()
	{
		for (int i = 0; i < m_size; i++)
			m_array[i] = 0;
	}

	void SetAll()
	{
		for (int i = 0; i < m_size; i++)
			m_array[i] = 0xFFFFFFFF;
	}

	bool WriteFile(const char* p_filename)
	{
		FILE* outfile = 0;
		int written = 0;
		outfile = fopen(p_filename, "wb");
		if (outfile == NULL)
			return false;
		written = fwrite(m_array, sizeof(unsigned long int), m_size, outfile);
		fclose(outfile);
		if (written != m_size)
			return false;
		return true;
	}

	bool ReadFile(const char* p_filename)
	{
		FILE* infile = 0;
		int read = 0;
		infile = fopen(p_filename, "rb");
		if (infile == NULL)
			return false;
		read = fread(m_array, sizeof(unsigned long int), m_size, infile);
		fclose(infile);
		if (read != m_size)
			return false;
		return true;
	}

	bool operator[] (int p_index)
	{
		int cell = p_index / 32;
		int bit = p_index % 32;
		return (((m_array[cell] & (1 << bit)) >> bit) != 0);
	}

};

//
//	Singly Linked List Node
//	
//	A singly linked list node that supports
//	any datatype and some functionality
//
template<class Datatype>
class SListNode
{
public:
	Datatype				m_data;
	SListNode<Datatype>*	m_next;

	SListNode() : m_next(NULL), m_data(0)
	{
	}

	void InsertAfter(Datatype p_data)
	{
		SListNode<Datatype>* newnode = new SListNode<Datatype>;
		newnode->m_data = p_data;
		newnode->m_next = m_next;
		m_next = newnode;
	}
};

//
//	Singly Linked List
//	
//	A singly linked list that supports
//	any datatype and some functionality
//
template<class Datatype>
class SLinkedList
{
public:
	SListNode<Datatype>*	m_head;
	SListNode<Datatype>*	m_tail;
	int						m_count;

	SLinkedList() : m_head(NULL), m_tail(NULL), m_count(0)
	{
	}

	~SLinkedList()
	{
		SListNode<Datatype>* itr = m_head;
		SListNode<Datatype>* next = NULL;
		while (itr != NULL)
		{
			next = itr->m_next;
			delete itr;
			itr = next;
		}
	}

	// add new node to the end (tail) of the list
	void Append(Datatype p_data)
	{
		if (m_head == NULL)
		{
			m_head = m_tail = new SListNode<Datatype>;
			m_head->m_data = p_data;
		}
		else
		{
			m_tail->InsertAfter(p_data);
			m_tail = m_tail->m_next;
		}
		m_count++;
	}

	//	add new node to the beginning (head) of the list
	void Prepend(Datatype p_data)
	{
		SListNode<Datatype>* newnode = new SListNode<Datatype>;
		newnode->m_data = p_data;
		newnode->m_next = m_head;

		m_head = newnode;
		if (m_tail == NULL)
			m_tail = m_head;
		m_count++;
	}

	void RemoveHead()
	{
		SListNode<Datatype>* node = NULL;
		if (m_head != NULL)
		{
			node = m_head->m_next;
			delete m_head;
			m_head = node;
			if (m_head == NULL)
				m_tail = NULL;
			m_count--;
		}
	}

	void RemoveTail()
	{
		SListNode<Datatype>* node = m_head;
		if (m_head != NULL)
		{
			if (m_head == m_tail)
			{
				delete m_head;
				m_head = m_tail = NULL;
			}
			else
			{
				while (node->m_next != m_tail)
				{
					node = node->m_next;
				}
				m_tail = node;
				delete node->m_next;
				node->m_next = NULL;
			}
			m_count--;
		}
	}

	SListIterator<Datatype> GetIterator() { return SListIterator<Datatype>(this, m_head); }

	void Insert(SListIterator<Datatype>& p_iterator, Datatype p_data)
	{
		if (p_iterator.m_list != this)
			return;
		if (p_iterator.m_node != NULL)
		{
			p_iterator.m_node->InsertAfter(p_data);
			if (p_iterator.m_node == m_tail)
			{
				m_tail = p_iterator._m_node->m_next;
			}
			m_count++;
		}
		else
		{
			Append(p_data);
		}
	}

	void Remove(SListIterator<Datatype>& p_iterator)
	{
		SListNode<Datatype>* node = m_head;
		if (p_iterator.m_list != this)
			return;
		if (p_iterator.m_node == NULL)
			return;
		if (p_iterator.m_node == m_head)
		{
			p_iterator.Forth();
			RemoveHead();
		}
		else
		{
			while (node->m_next != p_iterator.m_node)
				node = node->m_next;
			p_iterator.Forth();
			if (node->m_next == m_tail)
				m_tail = node;
			delete node->m_next;
			node->m_next = p_iterator.m_node;
		}
		m_count--;
	}

	bool SaveToDisk(char* p_filename)
	{
		FILE* outfile = NULL;
		SListNode* itr = m_head;
		outfile = fopen(p_filename, "wb");
		if (outfile == NULL)
			return false;
		fwrite(&m_count, sizeof(int), 1, outfile);
		while (itr != NULL)
		{
			fwrite(&(itr->m_data), sizeof(Datatype), 1, outfile);
			itr = itr->m_next;
		}
		fclose(outfile);
		return true;
	}

	bool ReadFromDisk(char* p_filename)
	{
		FILE* infile = NULL;
		Datatype buffer;
		int count = 0;
		infile = fopen(p_filename, "rb");
		if (infile == NULL)
			return false;
		fread(&count, sizeof(int), 1, infile);
		while (count != 0)
		{
			fread(&buffer, sizeof(Datatype), 1, infile);
			Append(buffer);
			count--;
		}
		fclose(infile);
		return true;
	}

};

//
//	Singly Linked List Iterator
//
//	An iterator for a singly linked list
//
template<class Datatype>
class SListIterator
{
public:
	SListNode<Datatype>*	m_node;
	SLinkedList<Datatype>*	m_list;

	SListIterator(SLinkedList<Datatype>* p_list = NULL, SListNode<Datatype>* p_node = NULL)
		: m_list(p_list), m_node(p_node)
	{
	}

	void Start()
	{
		if (m_list != NULL)
			m_node = m_list->m_head;
	}

	void Forth()
	{
		if (m_node != NULL)
			m_node = m_node->m_next;
	}

	Datatype& Item() { return m_node->m_data; }
	bool Valid() { return (m_node != NULL); }

};

//
//	Doubly Linked List Node
//	
//	A doubly linked list node that supports
//	any datatype and some functionality
//
template<class Datatype>
class DListNode
{
public:
	Datatype				m_data;
	DListNode<Datatype>*	m_next;
	DListNode<Datatype>*	m_previous;

	DListNode() : m_next(NULL), m_previous(NULL)
	{
	}

	void InsertAfter(Datatype p_data)
	{
		// step 1
		DListNode<Datatype>* newnode = new DListNode<Datatype>;
		newnode->m_data = p_data;
		// step 2
		newnode->m_previous = this;
		// step 3
		newnode->m_next = m_next;
		// step 4
		if(newnode->m_previous != NULL)
			newnode->m_previous->m_next = newnode;
		// step 5
		if (newnode->m_next != NULL)
			newnode->m_next->m_previous = newnode;
	}

	void InsertBefore(Datatype p_data)
	{
		// step 1
		DListNode<Datatype>* newnode = new DListNode<Datatype>;
		newnode->m_data = p_data;
		// step 2
		newnode->m_previous = m_previous;
		// step 3
		newnode->m_next = this;
		// step 4
		if (m_previous != NULL)
			m_previous->m_next = newnode;
		// step 5
		if (m_next != NULL)
			m_next->m_previous = newnode;

	}
};

//
//	Doubly Linked List
//	
//	A doubly linked list that supports
//	any datatype and some functionality
//
template<class Datatype>
class DLinkedList
{
public:
	DListNode<Datatype>*	m_head;
	DListNode<Datatype>*	m_tail;
	int						m_count;

	DLinkedList() : m_head(NULL), m_tail(NULL), m_count(0)
	{
	}

	~DLinkedList()
	{
		DListNode<Datatype>* itr = m_head;
		DListNode<Datatype>* next = NULL;
		while (itr != NULL)
		{
			next = itr->m_next;
			delete itr;
			itr = next;
		}
	}

	// add new node to the end (tail) of the list
	void Append(Datatype p_data)
	{
		if (m_head == NULL)
		{
			m_head = m_tail = new DListNode<Datatype>;
			m_head->m_data = p_data;
		}
		else
		{
			m_tail->InsertAfter(p_data);
			m_tail = m_tail->m_next;
		}
		m_count++;
	}

	//	add new node to the beginning (head) of the list
	void Prepend(Datatype p_data)
	{
		//DListNode<Datatype>* newnode = new DListNode<Datatype>;
		//newnode->m_data = p_data;
		//newnode->m_next = m_head;

		//m_head = newnode;
		//if (m_tail == NULL)
		//	m_tail = m_head;
		//m_count++;
		if (m_head == NULL)
		{
			m_head = m_tail = new DListNode<Datatype>;
			m_head->m_data = p_data;
		}
		else
		{
			m_head->InsertBefore(p_data);
			m_head = m_head->m_previous;
		}
		m_count++;
	}

	void RemoveHead()
	{
		DListNode<Datatype>* node = NULL;
		if (m_head != NULL)
		{
			node = m_head->m_next;
			delete m_head;
			m_head = node;
			if (m_head == NULL)
				m_tail = NULL;
			m_count--;
		}
	}

	void RemoveTail()
	{
		//DListNode<Datatype>* node = m_head;
		//if (m_head != NULL)
		//{
		//	if (m_head == m_tail)
		//	{
		//		delete m_head;
		//		m_head = m_tail = NULL;
		//	}
		//	else
		//	{
		//		while (node->m_next != m_tail)
		//		{
		//			node = node->m_next;
		//		}
		//		m_tail = node;
		//		delete node->m_next;
		//		node->m_next = NULL;
		//	}
		//	m_count--;
		//}
		DListNode<Datatype>* node = NULL;
		if (m_tail != NULL)
		{
			node = m_tail->m_previous;
			delete m_tail;
			m_tail = node;
			if (m_tail == NULL)
				m_head = NULL;
			m_count--;
		}
	}

	DListIterator<Datatype> GetIterator() { return DListIterator<Datatype>(this, m_head); }
	DListIterator<Datatype> GetIteratorAtHead() { return GetIterator(); }
	DListIterator<Datatype> GetIteratorAtTail() { return DListIterator<Datatype>(this, m_tail); }

	void Insert(DListIterator<Datatype>& p_iterator, Datatype p_data)
	{
		if (p_iterator.m_list != this)
			return;
		if (p_iterator.m_node != NULL)
		{
			p_iterator.m_node->InsertAfter(p_data);
			if (p_iterator.m_node == m_tail)
			{
				m_tail = p_iterator._m_node->m_next;
			}
			m_count++;
		}
		else
		{
			Append(p_data);
		}
	}

	void Remove(DListIterator<Datatype>& p_iterator)
	{
		//DListNode<Datatype>* node = m_head;
		//if (p_iterator.m_list != this)
		//	return;
		//if (p_iterator.m_node == NULL)
		//	return;
		//if (p_iterator.m_node == m_head)
		//{
		//	p_iterator.Forth();
		//	RemoveHead();
		//}
		//else
		//{
		//	while (node->m_next != p_iterator.m_node)
		//		node = node->m_next;
		//	p_iterator.Forth();
		//	if (node->m_next == m_tail)
		//		m_tail = node;
		//	delete node->m_next;
		//	node->m_next = p_iterator.m_node;
		//}
		//m_count--;

		if (p_iterator.m_node == m_head)
		{
			p_iterator.Forth();
			RemoveHead();
		}
		else
		{
			if (p_iterator.m_node->m_previous != NULL)
				p_iterator.m_node->m_previous->m_next = p_iterator.m_node->m_next;
			if (p_iterator.m_node->m_next != NULL)
				p_iterator.m_node->m_next->m_previous = p_iterator.m_node->m_previous;
		}
		m_count--;
	}

	bool SaveToDisk(char* p_filename)
	{
		FILE* outfile = NULL;
		DListNode* itr = m_head;
		outfile = fopen(p_filename, "wb");
		if (outfile == NULL)
			return false;
		fwrite(&m_count, sizeof(int), 1, outfile);
		while (itr != NULL)
		{
			fwrite(&(itr->m_data), sizeof(Datatype), 1, outfile);
			itr = itr->m_next;
		}
		fclose(outfile);
		return true;
	}

	bool ReadFromDisk(char* p_filename)
	{
		FILE* infile = NULL;
		Datatype buffer;
		int count = 0;
		infile = fopen(p_filename, "rb");
		if (infile == NULL)
			return false;
		fread(&count, sizeof(int), 1, infile);
		while (count != 0)
		{
			fread(&buffer, sizeof(Datatype), 1, infile);
			Append(buffer);
			count--;
		}
		fclose(infile);
		return true;
	}

};

//
//	Double Linked List Iterator
//
//	An iterator for a doubly linked list
//
template<class Datatype>
class DListIterator
{
public:
	DListNode<Datatype>*	m_node;
	DLinkedList<Datatype>*	m_list;

	DListIterator(DLinkedList<Datatype>* p_list = NULL, DListNode<Datatype>* p_node = NULL)
		: m_list(p_list), m_node(p_node)
	{
	}

	void Start()
	{
		if (m_list != NULL)
			m_node = m_list->m_head;
	}

	void Forth()
	{
		if (m_node != NULL)
			m_node = m_node->m_next;
	}

	void Back()
	{
		if (m_node != NULL)
			m_node = m_node->m_previous;
	}

	Datatype& Item() { return m_node->m_data; }
	bool Valid() { return (m_node != NULL); }

};

//
//	Linked Stack
//
//	A linked stack of data
//
template<class Datatype>
class LStack : public DLinkedList<Datatype>
{
public:
	void Push(Datatype p_data) { Append(p_data); }
	void Pop() { RemoveTail(); }
	Datatype Top() { return m_tail->m_data; }
	int Count() { return m_count; }
};

//
//	Array Stack
//
//	An arrayed stack of data
//
template<class Datatype>
class AStack : public Array<Datatype>
{
public:
	int		m_top;

	AStack(int p_size) : Array<Datatype>(p_size), m_top(NULL)
	{
	}

	bool Push(Datatype p_data)
	{
		if (m_size != m_top)
		{
			m_array[m_top] = p_data;
			m_top++;
			return true;
		}
		return false;
	}

	void Pop()
	{
		if (m_top > 0)
			m_top--;
	}

	Datatype Top() { return m_array[m_top - 1]; }
	int Count()	{ return m_top; }

};

//
//	Linked Queue
//
//	A linked queue of data
//
template<class Datatype>
class LQueue : public DLinkedList<Datatype>
{
public:
	void Enqueue(Datatype p_data) { Append(p_data); }
	void Dequeue() { RemoveHead(); }
	Datatype Front() { return m_head->m_data; }
	int Count() { return m_count; }
};

//
//	Arrayed Queue
//
//	An arrayed queue of data
//
template<class Datatype>
class AQueue : public Array<Datatype>
{
	int		m_front;
	int		m_count;

	AQueue(int p_size) : Array<Datatype>(p_size), m_front(0), m_count(0)
	{
	}

	bool Enqueue(Datatype p_data)
	{
		if (m_size != m_count)
		{
			m_array[(m_count + m_front)%m_size] = p_data;
			m_count++;
			return true;
		}
		return false;
	}

	bool Dequeue()
	{
		if (m_count > 0)
		{
			m_count--;
			m_front++;
			if (m_front == m_size)
				m_front = 0;
		}
	}

	Datatype Front() { return m_array[m_front] }
	Datatype& operator[] (int p_index) { return m_array[(p_index + m_front)%m_size]; }

};

//
//	Hash Entry
//
//	An an entry in a hash table
//
template<class Keytype, class Datatype>
class HashEntry
{
public:
	Keytype		m_key;
	Datatype	m_data;
};

//
//	Hash Table
//
//	A table of hashed data
//
template<class Keytype, class Datatype>
class HashTable
{
public:
	typedef HashEntry<Keytype, Datatype>	Entry;
	int										m_size;
	int										m_count;
	Array<DLinkedList<Entry>>				m_table;
	unsigned long int						(*m_hash)(Keytype);
	
	HashTable(int p_size, unsigned long int (*p_hash)(Keytype))
		: m_table(p_size)
	{
		m_size = p_size;
		m_hash = p_hash;
		m_count = 0;
	}

	void Insert(Keytype p_key, Datatype p_data)
	{
		Entry entry;
		entry.m_data = p_data;
		entry.m_key = p_key;
		int index = m_hash(p_key) % m_size;
		m_table[index].Append(entry);
		m_count++;
	}

	Entry* Find(Keytype p_key)
	{
		int index = m_hash(p_key) % m_size;
		DListIterator<Entry> itr = m_table[index].GetIterator();
		while (itr.Valid())
		{
			if (itr.Item().m_key == p_key)
				return &(itr.Item());
			itr.Forth();
		}
		return 0;
	}

	bool Remove(Keytype p_key)
	{
		int index = m_hash(p_key) % m_size;
		DListIterator<Entry> itr = m_table[index].GetIterator();
		while (itr.Valid())
		{
			if (itr.Item().m_key == p_key)
			{
				m_table[index].Remove(itr);
				m_count--;
				return true;
			}
			itr.Forth();
		}
		return false;
	}
};

class String
{
public:
	char	m_string[64];
	String() { strcpy(m_string, ""); }
	String(char* p_string) { strcpy(m_string, p_string); }
	bool operator== (String& p_right) { return !strcmp(m_string, p_right.m_string); }
};

#endif