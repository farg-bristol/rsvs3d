#ifndef PARAMETERS_H_INCLUDED 
#define PARAMETERS_H_INCLUDED 



//=================================
// forward declared dependencies
// 		class foo; //when you only need a pointer not the actual object
// 		and to avoid circular dependencies

//=================================
// included dependencies

#include <cstdlib>
#include <array>
#include <string>

//=================================
// Code
// 
// This file defines parameters to be used in other parts of the 
// RSVS snaking framework. Default values are defined in "parameters.cpp"
//
// Substructure names are all 4-5 letters


namespace param {
	typedef std::array<double,2> realbounds; 

	/*
	Parameters related to the Velocity calculation and VOS steps
	*/
	class rsvs
	{
	public:
		// Algorithm used by Eigen to solve the SQP system.
		int solveralgorithm;

		rsvs();
		~rsvs();
	};

	/*
	Parameters controlling tuning parameters for the stepping of
	the restricted surface.
	*/
	class snaking
	{
	public:
		// Distance along edge at which a vertex is considered
		// arrived regardless of "d" and "v"
		double arrivaltolerance;
		// Distance along edge at which converging snaxels are
		// considered arrived
		double multiarrivaltolerance;
		// Initialisation boundary (either 0 or 1)
		int initboundary;
		// maximum number of steps
		int maxsteps;

		snaking();
		~snaking();
		
	};

	/*
	Paramters controlling grid properties
	*/
	class voxel
	{
	public:
		std::array<realbounds, 3>  domain;
		std::array<int, 3>  gridsizebackground;
		std::array<int, 3> gridsizesnake;

		voxel();
		~voxel();
	};

	class grid
	{
	public:
		voxel voxel;
		// grid();
		// ~grid();
		
	};

	class parameters
	{
	public:
		rsvs rsvs;
		snaking snak;
		grid grid;		
	};

	namespace io {
		void read(const std::string &fileName, parameters &p);
		void readflat(const std::string &fileName, parameters &p);
		void write(const std::string &fileName, const parameters &p);
		void writeflat(const std::string &fileName, const parameters &p);

		void defaultconf();
	}
	namespace test {

		int base();
		int io();
		int ioflat();
		int ipartialread();
	}
}

#endif