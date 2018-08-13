#ifndef RSVSINTERFACE_H_INCLUDED 
#define RSVSINTERFACE_H_INCLUDED 


//=================================
// forward declared dependencies
// 		class foo; //when you only need a pointer not the actual object
// 		and to avoid circular dependencies


//=================================
// included dependencies
#include <vector> 
#include "vectorarray.hpp" 
#include "RSVSmath.hpp"
#include "mesh.hpp"
#include "snake.hpp"
#include "snakevel.hpp"
#include <Eigen>

using namespace std; 
using namespace Eigen; 

class SQPcalc {
protected:
	int nDv;
	int nConstr;
public :
	MatrixXd dConstr,HConstr, HObj;
	RowVectorXd dObj;
	VectorXd constr, lagMult;
	double obj=0.0;


	HashedVector<int, int> dvMap;
	HashedVector<int,int> constrMap;

	void BuildMathArrays(int nDv, int nConstr);
	void BuildConstrMap(const vector<int> &vecin);
	void BuildDVMap(const vector<int> &vecin);
	void CalcTriangle(const triangle& triIn, const triangulation &triRSVS);
	void CalculateTriangulation(const triangulation &triRSVS);
};


void Deriv1stChainScalar(const MatrixXd &dSdc,const MatrixXd &dcdd, MatrixXd &dSdd);
void Deriv2ndChainScalar(const MatrixXd &dSdc,const MatrixXd &dcdd,const MatrixXd &HSc,const MatrixXd &Hcd,MatrixXd &HSd);
void VecBy3DimArray(const MatrixXd &vec, const MatrixXd &arr3dim, MatrixXd &retArray);
void ArrayVec2MatrixXd(const ArrayVec<double> &arrayIn, MatrixXd &matOut);
//==================================
// Code
// NOTE: function in a class definition are IMPLICITELY INLINED 
//       ie replaced by their code at compile time
	

#endif