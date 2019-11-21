/*
Implementation of color lattice boltzmann model
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <fstream>

#include "common/Communication.h"
#include "common/MPI_Helpers.h"
#include "common/Database.h"
#include "common/ScaLBL.h"
#include "ProfilerApp.h"
#include "threadpool/thread_pool.h"

class ScaLBL_GreyscaleModel{
public:
	ScaLBL_GreyscaleModel(int RANK, int NP, MPI_Comm COMM);
	~ScaLBL_GreyscaleModel();	
	
	// functions in they should be run
	void ReadParams(string filename);
	void ReadParams(std::shared_ptr<Database> db0);
	void SetDomain();
	void ReadInput();
	void Create();
	void Initialize();
	void Run();
	void WriteDebug();
	
	bool Restart,pBC;
	int timestep,timestepMax;
	int BoundaryCondition;
	double tau;
	double tolerance;
	double Fx,Fy,Fz,flux;
	double din,dout;
	
	int Nx,Ny,Nz,N,Np;
	int rank,nprocx,nprocy,nprocz,nprocs;
	double Lx,Ly,Lz;

	std::shared_ptr<Domain> Dm;   // this domain is for analysis
	std::shared_ptr<Domain> Mask; // this domain is for lbm
	std::shared_ptr<ScaLBL_Communicator> ScaLBL_Comm;
    
    // input database
    std::shared_ptr<Database> db;
    std::shared_ptr<Database> domain_db;
    std::shared_ptr<Database> greyscale_db;
    std::shared_ptr<Database> analysis_db;
    std::shared_ptr<Database> vis_db;

    IntArray Map;
    DoubleArray SignDist;
    signed char *id;    
	int *NeighborList;
	int *dvcMap;
	double *fq;
	double *Permeability;
	double *Porosity;
	double *Velocity;
	double *Pressure;
		
private:
	MPI_Comm comm;
    
	int dist_mem_size;
	int neighborSize;
	// filenames
    char LocalRankString[8];
    char LocalRankFilename[40];
    char LocalRestartFile[40];
   
    void AssignComponentLabels(double *Porosity, double *Permeablity);
    
};
