
//===============================================
// Include Guards
#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

//===============================================
// Levels of debuging Guards
#ifdef DEBUGLVL2 // All Debugging calls
#define DEBUGLVL1
#define TEST_RANGE // deprecated
#define TEST_ARRAYSTRUCT

#endif

#ifdef DEBUGLVL1 // Debugging of new features.
#ifndef SAFE_ACCESS
#define SAFE_ACCESS
#endif
#endif

//=================================
// forward declared dependencies
// 		class foo; //when you only need a pointer not the actual object
// 		and to avoid circular dependencies


//=================================
// included dependencies
#ifdef DBG_MEMLEAK
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>
#endif //DBG_MEMLEAK

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <functional>
#include <cmath>

#include "arraystructures.hpp"

//==================================
// Code
// NOTE: function in a class definition are IMPLICITELY INLINED 
//       ie replaced by their code at compile time
using namespace std;


class meshpart;
class mesh;
class meshdependence; 
class ConnecRemv;

class volu;
class surf;
class vert;
class edge;
class coordvec;

//typedef ArrayStruct<surf> surfarray;
typedef ModiftrackArray<surf> surfarray;
typedef ArrayStruct<volu> voluarray;
typedef ModiftrackArray<edge> edgearray;
typedef ArrayStruct<vert> vertarray;

// Forward declared templated functions

// Base class



class coordvec {
	// Handles the use and norm of a vector for which the norm and the unit 
	// value might be needed
protected:
	vector<double> elems;
	double norm;
	int isuptodate;
	

public:
	double CalcNorm();
	double GetNorm();
	void PrepareForUse();
	coordvec Unit() const ;
	double Unit(const int a) const;
	void assign(double a, double b, double c);
	double& operator[](int a);
	double operator()(int a) const;
	void disp() const;
	bool isready() const {return(bool(isuptodate));};
	const vector<double>& usedata() const {return(elems);}
	const vector<double>* retPtr() const {return(&elems);}
	// Math and logical operations (element wise)
	void max(const vector<double> &vecin);
	void min(const vector<double> &vecin);
	void add(const vector<double> &vecin);
	void substract(const vector<double> &vecin);
	void substractfrom(const vector<double> &vecin);
	void div(const vector<double> &vecin);
	void div(double scalin);
	void mult(const vector<double> &vecin);
	void mult(double scalin);
	vector<double> cross(const vector<double> &vecin);
	double dot(const vector<double> &vecin);

	coordvec(){
		elems.reserve(3); // reserves 3 as this is the size of the array
		elems.assign(3,0);
		norm=0;
		isuptodate=0;
		#ifdef TEST_SNAKSTRUCT
		cout << "constructor called for coordvec" << endl;
		#endif
	}
	void operator=(const vector<double> &a){
		if(int(a.size())!=3){
			cout << "Warning : Coordinate vector is being a vecetor other than 3 long" << endl;
		}
		elems=a;
		isuptodate=0;
	}
};


class meshpart : public ArrayStructpart { // Abstract class to ensure interface is correct
	public : 
	virtual void disptree(const mesh &meshin, int n) const =0 ;

};

// Derived Classes
class volu: public meshpart {
public:
	
	double fill,target,error, volume;
	vector<int> surfind;

	void ChangeIndices(int nVert,int nEdge,int nSurf,int nVolu);
	void disp() const;
	void disptree(const mesh &meshin, int n) const;
	void PrepareForUse(){};
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-parameter"
	bool isready(bool isInMesh) const {return(true);}
	#pragma GCC diagnostic pop
	void read(FILE * fid);
	void write(FILE * fid) const;
	void TightenConnectivity() {sort(surfind);unique(surfind);};

	volu(){ // Constructor
		index=0;
		fill=0;
		target=1;
		error=1;

		#ifdef TEST_ARRAYSTRUCT
		cout << "volu #" << index << " Was created " << surfind.size() << endl;
		#endif
	}
	volu(const volu& oldVolu){ // Copy-Constructor
		index=oldVolu.index;
		fill=oldVolu.fill;
		target=oldVolu.target;
		error=oldVolu.error;
		surfind=oldVolu.surfind;

		#ifdef TEST_ARRAYSTRUCT
		cout << "copyvolu #" << index << " Was created " 
			<< surfind.size() << endl;
		#endif
	}
	~volu(){ // Destructor
		surfind.clear();

		#ifdef TEST_ARRAYSTRUCT
		cout << "volu #" << index << " Was deleted " << surfind.size() << endl;
		#endif

	}
	void operator=(const volu* other){
		index=other->index;
		fill=other->fill;
		target=other->target;
		error=other->error;
		surfind=other->surfind;

		#ifdef TEST_ARRAYSTRUCT
		cout << "OTHER: " ; other->disp();
		#endif
	}

	int Key() const {return(index);}
};



class surf: public meshpart , public modiftrackpart {
protected:
	bool isordered;
	//bool isModif=true;
public:
	friend class mesh;
	friend surfarray;
	//friend void mesh::SwitchIndex(int typeInd, int oldInd, int newInd, vector<int> scopeInd);
	//friend void mesh::RemoveIndex(int typeInd, int oldInd);

	double fill,target,error,area;
	vector<int> edgeind;
	vector<int> voluind;
	 // reserves 2 as this is the size of the array

	void disp() const;
	void disptree(const mesh &meshin, int n) const;
	void ChangeIndices(int nVert,int nEdge,int nSurf,int nVolu);
	void PrepareForUse(){};	
	bool isready(bool isInMesh) const {return(isInMesh? isordered : true);}
	void read(FILE * fid);
	void write(FILE * fid) const;
	int OrderEdges(mesh *meshin);
	int SplitSurface(mesh &meshin, const vector<int> &fullEdgeInd);
	void OrderedVerts(const mesh *meshin, vector<int> &vertList) const;
	void TightenConnectivity() {sort(voluind);unique(voluind);
		sort(edgeind);unique(edgeind);isordered=false;};
	void FlipVolus();
	bool edgeconneq(const surf &other, bool recurse=true) const;
	//bool returnIsModif() const {return(isModif);}

	surf(){ // Constructor
		index=0;
		fill=1;
		target=1;
		error=1;
		voluind.reserve(2); // reserves 2 as this is the size of the array
		voluind.assign(2,0);
		isordered=false;
	}
	~surf(){ // Destructor

		edgeind.clear();
		voluind.clear();

	}
	surf(const surf& oldSurf){ // Copy-Constructor
		index=oldSurf.index;
		fill=oldSurf.fill;
		target=oldSurf.target;
		error=oldSurf.error;
		edgeind=oldSurf.edgeind;
		voluind=oldSurf.voluind;
		isordered=oldSurf.isordered;
	}
	void operator=(const surf* other){
		index=other->index;
		fill=other->fill;
		error=other->error;
		target=other->target;
		edgeind=other->edgeind;
		voluind=other->voluind;
		isordered=other->isordered;
	}

	int Key() const {return(index);}

};


class edge: public meshpart , public modiftrackpart {
public:
	friend class mesh;
	friend edgearray;
	

	vector<int> vertind;
	vector<int> surfind;
	 // reserves 2 as this is the size of the array

	void ChangeIndices(int nVert,int nEdge,int nSurf,int nVolu);
	void disp() const;
	void disptree(const mesh &meshin, int n) const;
	void PrepareForUse(){};
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-parameter"
	bool isready(bool isInMesh) const {return(true);}
	#pragma GCC diagnostic pop
	void read(FILE * fid);
	void write(FILE * fid) const;
	void TightenConnectivity() {
		if(vertind.size()>2){
			RSVS3D_ERROR_ARGUMENT("vertind should be size 2");
		}
		sort(surfind);unique(surfind);
	};
	void GeometricProperties(const mesh *meshin, coordvec &centre, double &length) const;
	bool vertconneq(const edge &other) const;
	edge(){ // Constructor
		index=0;
		vertind.reserve(2);
		vertind.assign(2,0); // reserves 2 as this is the size of the array

	}
	edge(const edge& oldEdge){ // Copy-Constructor
		index=oldEdge.index;
		vertind=oldEdge.vertind;
		surfind=oldEdge.surfind;
	}
	~edge(){ // Destructor

		vertind.clear();
		surfind.clear();

	}
	void operator=(const edge* other){
		index=other->index;

		vertind=other->vertind;
		surfind=other->surfind;
	}

	int Key() const {return(index);}
};

class vert: public meshpart {
public:
	

	vector<int> edgeind;
	vector<double> coord;
	 // reserves 2 as this is the size of the array

	void disp() const;
	void disptree(const mesh &meshin, int n) const;
	void ChangeIndices(int nVert,int nEdge,int nSurf,int nVolu);
	void PrepareForUse(){};
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-parameter"
	bool isready(bool isInMesh) const {return(true);}
	#pragma GCC diagnostic pop
	void read(FILE * fid);
	void write(FILE * fid) const;
	void TightenConnectivity() {sort(edgeind);unique(edgeind);};


	vert(){ // Constructor
		index=0;
		coord.reserve(3); // reserves 2 as this is the size of the array
		coord.assign(3,0);
	}
	vert(const vert& oldEdge){ // Copy-Constructor
		index=oldEdge.index;
		edgeind=oldEdge.edgeind;
		coord=oldEdge.coord;
	}
	~vert(){ // Destructor

		edgeind.clear();
		coord.clear();

	}
	void operator=(const vert* other){
		index=other->index;

		edgeind=other->edgeind;
		coord=other->coord;
	}

	int Key() const {return(index);}
};



class ConnecRemv {
public:
	int keepind;
	int typeobj;
	vector<int> rmvind;
	vector<int> scopeind;
	void disp();
	
};

/*
class surfarray : public ArrayStruct<surf>{
	using ArrayStruct<surf>::elems;
	friend class mesh;
	friend class snake;

public:
	void SetNoModif();
	void ReturnModifInd(vector<int> &vecind);
	void ReturnModifLog(vector<bool> &modiflog);
	surf& operator[](const int a){ 
		#ifdef SAFE_ACCESS 
		ArrayStruct<surf>::issafeaccess(a);
		#endif
		elems[a].isModif=true;
		return(ArrayStruct<surf>::operator[](a));
	}
};*/

class meshdependence {
	// Class for connecting meshes
	// Stores a vector of mesh references for parent and children
	// Needs to support partial meshes for constraint handling
protected:
	friend class mesh;
	int nParents = 0;
	vector<int> elemind; // active element index
	vector<mesh*> parentmesh; // use references as cannot be null
	vector<mesh*> childmesh;
	vector<HashedVectorSafe<int,int>> parentconn;
	// These methods are protected to avoid broken/uni-directional connectivities being generated
	int AddParent(mesh* meshin);
	int AddChild(mesh* meshin);
	void AddParent(mesh* meshin, vector<int> &parentind);
	void RemoveChild(mesh* meshin);
	void RemoveParent(mesh* meshin);

public:

};


class mesh {
private:
	bool borderIsSet=false;
	bool meshDepIsSet=false;
	int meshDim=0;
	void SetLastIndex();
	
	void OrientSurfaceVolume();
	void OrientEdgeSurface();
	int OrientRelativeSurfaceVolume(vector<int> &surfOrient);
	void ArraysAreHashed();
	friend class snake;
public:
	vertarray verts;
	edgearray edges;
	surfarray surfs;
	voluarray volus;

	meshdependence meshtree;
	// Mesh Lineage
	void RemoveFromFamily();
	void AddChild(mesh* meshin);
	void AddParent(mesh* meshin);
	void AddParent(mesh* meshin, vector<int> &parentind);
	void AddChild(mesh* meshin, vector<int> &parentind);
	void SetMeshDepElm();
	void MaintainLineage();// Method needed to robustly maintain lineage through the family.
	int CountParents() const;
	int SurfInParent(int surfind) const;
	void SurfInParent(vector<int> &listInParent) const;
	void ElmOnParentBound(vector<int> &listInParent, vector<int> &voluInd,
		bool isBorderBound=true,
		bool outerVolume=true) const;
	void SurfOnParentBound(vector<int> &listInParent, vector<int> &voluInd,
		bool isBorderBound,
		bool outerVolume) const;
	void EdgeOnParentBound(vector<int> &listInParent, vector<int> &voluInd,
		bool isBorderBound,
		bool outerVolume) const;
	int CountVoluParent() const ;
	void ReturnParentMap(vector<int> &currind, vector<int> &parentpos,
		vector<pair<int,int>> &parentcases, vector<double> &voluVals) const;
	void MapVolu2Parent(const vector<double> &fillIn,
		const vector<pair<int,int>> &parentcases, double volu::*mp=&volu::fill);
	void MapVolu2Self(const vector<double> &fillIn, 
		const vector<int> &elms, double volu::*mp=&volu::fill);
	void VoluValuesofParents(int elmInd, vector<double> &vals,
		int volType=0) const;
	void VoluValuesofParents(int elmInd, vector<double> &vals,
		double volu::*mp) const;
	void SurfValuesofParents(int elmInd, vector<double> &vals,
		int volType=0) const;
	void SurfValuesofParents(int elmInd, vector<double> &vals,
		double surf::*mp) const;
	// Mesh property
	int WhatDim() const {return(meshDim);}
	// basic operations grouped from each field
	void HashArray();
	void SetMaxIndex();
	void GetMaxIndex(int *nVert,int *nEdge,int *nSurf,int *nVolu) const;
	void Init(int nVe,int nE, int nS, int nVo);
	void reserve(int nVe,int nE, int nS, int nVo);
	void PrepareForUse(bool needOrder=true);
	void disp() const;
	void displight() const;
	void Concatenate(const mesh &other);
	bool isready() const;
	void PopulateIndices();
	void TightenConnectivity();
	int TestConnectivity(const char *strRoot="") const;
	int TestConnectivityBiDir(const char *strRoot="",
		bool emptyIsErr=true) const;
	// File I/o
	void write(FILE *fid) const;
	void read(FILE *fid);
	int write(const char *str) const;
	int read(const char *str);
	// Mesh merging
	void MakeCompatible_inplace(mesh &other) const;
	mesh MakeCompatible(mesh other) const;
	void ChangeIndices(int nVert,int nEdge,int nSurf,int nVolu);
	void SwitchIndex(int typeInd, int oldInd, int newInd,
		const vector<int> &scopeInd={0});
	void RemoveIndex(int typeInd, int oldInd);
	int ConnectedVertex(vector<int> &vertBlock) const;
	int ConnectedVolumes(vector<int> &volBlock, 
		const vector<bool> &boundaryFaces={}) const;
	void ForceCloseContainers();
	void RemoveSingularConnectors(const std::vector<int> &rmvVertInds={},
		bool voidError=true);
	std::vector<int> MergeGroupedVertices(HashedVector<int, int> &closeVert,
		bool delVerts=true);
	// Mesh Quality
	vector<int> OrderEdges();
	void SetBorders();
	void OrientFaces();
	// Mesh component comparison
	void GetOffBorderVert(vector<int> &vertList, vector<int> &voluInd,
		int outerVolume=-1);
	void GetOffBorderVert(vector<int> &vertList, vector<int> &voluInd,
		int outerVolume=-1) const;
	void GetOffBorderVert3D(vector<int> &vertList, vector<int> &voluInd,
		int outerVolume=-1) const;
	void GetOffBorderVert2D(vector<int> &vertInd, vector<int> &surfind,
		int outerVolume=-1) const;
	// Mesh calculations
	coordvec CalcCentreVolu(int ind) const;
	coordvec CalcPseudoNormalSurf(int ind) const;
	// Mesh size and position
	void Scale();
	void Scale(const std::array<std::array<double, 2>,3> &domain);
	void LoadTargetFill(const std::string &fileName);
	void ReturnBoundingBox(std::array<double,3> &lowerB, 
		std::array<double,3> &upperB);
	// Mesh Splitting and cropping
	void Crop(vector<int> indList, int indType=1);
	vector<int> AddBoundary(const vector<double> &lb, const vector<double> &ub);
	void CropAtBoundary(const vector<double> &lb, const vector<double> &ub);
	~mesh(){
		RemoveFromFamily();
	}
};

// Function declarations

void ConnVertFromConnEdge(const mesh &meshin, const vector<int> &edgeind,
	vector<int> &vertind);
int OrderMatchLists(const vector<int> &vec1, const vector<int> &vec2,
	int p1, int p2);
void CropMeshGreedy(mesh &meshin, const std::vector<double> &lb,
		const std::vector<double> &ub);
int OrderEdgeList(vector<int> &edgeind, const mesh &meshin, bool warn=true,
	bool errout=true, const vector<int>* edgeIndOrigPtr=NULL,
	const surf* surfin=NULL);

namespace meshhelp {
	template<class T, class V, class W>
	double ProjectRay(int count, const W &&boundBox,
		const T &dir, const V &orig, double minDist=0.0);

	void PlaceBorderVertex(const std::vector<double> &coordIn, 
		const std::vector<double> &coordOut,
		const std::vector<double> &lb,
		const std::vector<double> &ub, std::vector<double> &coordTarg);

	void SplitBorderSurfaceEdgeind(const mesh &meshin,
		const std::vector<bool> &edgeOut, 
		std::vector<int> &vecconnIn, std::vector<int> &vecconnOut);
	void SplitBorderVolumeSurfind(const mesh &meshin, 
		const std::vector<bool> &edgeOut, 
		std::vector<int> &vecconnIn, std::vector<int> &vecconnOut);
	void HandleMultiSurfaceSplit(mesh &meshin, 
		vector<int> &edgeindOld, vector<int> &edgeindNew,
		vector<int> &vertindNew);
	std::vector<int> FindVertInFromEdgeOut(const mesh &meshin, 
		const std::vector<bool> &vertOut,
		const std::vector<int> &edgeList, 
		const std::vector<int> &edgeListCheck);
	std::vector<int> FindEdgeInFromSurfOut(const mesh &meshin, 
		const std::vector<bool> &edgeOut,
		std::vector<int> surfList);
}
//test functions
int Test_ArrayStructures();
int Test_Volu();
int Test_Surf();
int Test_Vert();
int Test_Edge();
int Test_Mesh();
int Test_Crop();
void PopulateIndices(mesh *meshin);
//template <class T> bool CompareDisp(T *mesh1,T *mesh2);
//bool CompareFuncOut(function<void()> mesh1, function<void()> mesh2);

  
template<class T, class V, class W>
double meshhelp::ProjectRay(int count, const W &&boundBox,
	const T &dir, const V &orig, double minDist){
	/*
	Calculates the distance to project a single ray to reach the bounding box

	It is templated to accept any types of containers with `count` elements, 
	accessible by operator[];

	W is an object of size [2][count] accessible by operators [][]
	*/

	double l=-INFINITY;

	for (int i = 0; i < count; ++i)
	{
		l = std::max(l,
				std::min(
					(boundBox[0][i]-orig[i])/dir[i] ,
				(boundBox[1][i]-orig[i])/dir[i] ));
	}
	return std::min(l, minDist);
}

#endif // MESH_H_INCLUDED