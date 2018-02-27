
//===============================================
// Include Guards
#ifndef ARRAYSTRUCTS_H_INCLUDED
#define ARRAYSTRUCTS_H_INCLUDED

//===============================================
// Levels of debuging Guards
#ifdef DEBUGLVL2 // All Debugging calls
#define DEBUGLVL1
#endif

#ifdef DEBUGLVL1 // Debugging of new features.
#define TEST_ARRAYSTRUCT
#endif

//=================================
// forward declared dependencies
// 		class foo; //when you only need a pointer not the actual object
// 		and to avoid circular dependencies


//=================================
// included dependencies
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <unordered_map>

//==================================
// Code
// NOTE: function in a class definition are IMPLICITELY INLINED 
//       ie replaced by their code at compile time
using namespace std;

class meshpart;
class mesh;

class volu;
class surf;
class vert;
class edge;

template <class T> class ArrayStruct;
typedef unsigned int unsigned_int;

typedef ArrayStruct<volu> voluarray;
typedef ArrayStruct<surf> surfarray;
typedef ArrayStruct<edge> edgearray;
typedef ArrayStruct<vert> vertarray;

// Templates
template <class T> // Template for array of structures
class ArrayStruct { 
    public: 
		vector<T> elems;    // vector of elements (structures) 
		unordered_map<int,int> hashTable; // Hash Table of indexed elements
		void HashArray();
		void disp();
		

		T* operator[](const int& a){ // [] Operator returns a pointer to the corresponding elems.
			#ifdef TEST_ARRAYSTRUCT // adds a check in debug mode
				if ((unsigned_int(a)>=elems.size()) | (0>a)){
					throw range_error ("Index is out of range");
				}
			#endif
			return(&(elems[a]));
		}

}; 

// Base class
class mesh {
public:
	class vertarray;
	class edgearray;
	class surfarray;
	class voluarray;

};

class meshpart{ // Abstract class to ensure interface is correct
	public : 
	int index;
	virtual void disp() =0 ;
	virtual int Key() =0 ;

};

// Derived Classes
class volu: public meshpart {
public:
	int index;
	double fill;
	vector<int> surfind;

	void disp();

	volu(){ // Constructor
		index=0;
		fill=0;

		#ifdef TEST_ARRAYSTRUCT
			cout << "volu #" << index << " Was created " << surfind.size() << endl;
		#endif
	}
	volu(const volu& oldVolu){ // Copy-Constructor
		index=oldVolu.index;
		fill=oldVolu.fill;
		surfind=oldVolu.surfind;

		#ifdef TEST_ARRAYSTRUCT
			cout << "copyvolu #" << index << " Was created " << surfind.size() << endl;
		#endif
	}
	~volu(){ // Destructor
		surfind.clear();

		#ifdef TEST_ARRAYSTRUCT
			cout << "volu #" << index << " Was deleted " << surfind.size() << endl;
		#endif
	
	}
	void operator=( volu* other){
		index=other->index;
		fill=other->fill;
		surfind=other->surfind;

		#ifdef TEST_ARRAYSTRUCT
			cout << "OTHER: " ; other->disp();
		#endif
	}

	int Key() {return(index);}
};



class surf: public meshpart {
public:
	int index;
	double fill;
	vector<int> edgeind;
	vector<int> voluind;
	 // reserves 2 as this is the size of the array

	void disp();

	surf(){ // Constructor
		index=0;
		fill=1;
		voluind.reserve(2); // reserves 2 as this is the size of the array

	}
	~surf(){ // Destructor

		edgeind.clear();
		voluind.clear();
	
	}
	void operator=( surf* other){
		index=other->index;
		fill=other->fill;
		edgeind=other->edgeind;
		voluind=other->voluind;
	}

	int Key() {return(index);}

};


class edge: public meshpart {
public:
	int index;

	vector<int> vertind;
	vector<int> surfind;
	 // reserves 2 as this is the size of the array

	void disp();

	edge(){ // Constructor
		index=0;
		vertind.reserve(2); // reserves 2 as this is the size of the array

	}
	~edge(){ // Destructor

		vertind.clear();
		surfind.clear();
	
	}
	void operator=( edge* other){
		index=other->index;

		vertind=other->vertind;
		surfind=other->surfind;
	}

	int Key() {return(index);}
};

class vert: public meshpart {
public:
	int index;

	vector<int> edgeind;
	vector<double> coord;
	 // reserves 2 as this is the size of the array

	void disp();

	vert(){ // Constructor
		index=0;
		coord.reserve(2); // reserves 2 as this is the size of the array

	}
	~vert(){ // Destructor

		edgeind.clear();
		coord.clear();
	
	}
	void operator=( vert* other){
		index=other->index;

		edgeind=other->edgeind;
		coord=other->coord;
	}

	int Key() {return(index);}
};

// functions
int Test_ArrayStructures();
int Test_Volu();
int Test_Surf();
int Test_Vert();
int Test_Edge();

// member function definition template <class T> : "ArrayStruct"
template<class T> void ArrayStruct <T>::HashArray(){
   hashTable.reserve(elems.size());
   for (int i = 0; i < int(elems.size()); ++i)
   {
      hashTable.emplace(elems[i].Key(),i);
   }
   cout << "Array Struct Succesfully Hashed" << endl;
}

template<class T> void ArrayStruct <T>::disp(){
   for (int ii ; unsigned_int(ii)<elems.size();ii++){
      cout << "Array " << ii << " " ;
      elems[ii].disp();
   }
}

#endif