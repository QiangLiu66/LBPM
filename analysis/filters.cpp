#include "analysis/filters.h"

#include "ProfilerApp.h"


void Med3D( const Array<float> &Input, Array<float> &Output )
{
	PROFILE_START("Med3D");
	// Perform a 3D Median filter on Input array with specified width
	int i,j,k,ii,jj,kk;
	int imin,jmin,kmin,imax,jmax,kmax;

	float *List;
	List=new float[27];

	int Nx = int(Input.size(0));
	int Ny = int(Input.size(1));
	int Nz = int(Input.size(2));

	for (k=1; k<Nz-1; k++){
		for (j=1; j<Ny-1; j++){
			for (i=1; i<Nx-1; i++){

				// Just use a 3x3x3 window (hit recursively if needed)
				imin = i-1;
				jmin = j-1;
				kmin = k-1;
				imax = i+2;
				jmax = j+2;
				kmax = k+2;

				// Populate the list with values in the window
				int Number=0;
				for (kk=kmin; kk<kmax; kk++){
					for (jj=jmin; jj<jmax; jj++){
						for (ii=imin; ii<imax; ii++){
							List[Number++] = Input(ii,jj,kk);
						}
					}
				}
				// Sort the first 5 entries and return the median
				for (ii=0; ii<14; ii++){
					for (jj=ii+1; jj<27; jj++){
						if (List[jj] < List[ii]){
							float tmp = List[ii];
							List[ii] = List[jj];
							List[jj] = tmp;
						}
					}
				}
				// Return the median
				Output(i,j,k) = List[13];
			}
		}
	}
	PROFILE_STOP("Med3D");
}


int NLM3D( const Array<float> &Input, Array<float> &Mean, 
    const Array<float> &Distance, Array<float> &Output, const int d, const float h)
{
	PROFILE_START("NLM3D");
	// Implemenation of 3D non-local means filter
	// 		d determines the width of the search volume
	// 		h is a free parameter for non-local means (i.e. 1/sigma^2)
	// 		Distance is the signed distance function
	// 		If Distance(i,j,k) > THRESHOLD_DIST then don't compute NLM

	float THRESHOLD_DIST = float(d);
	float weight, sum;
	int i,j,k,ii,jj,kk;
	int imin,jmin,kmin,imax,jmax,kmax;
	int returnCount=0;

	int Nx = int(Input.size(0));
	int Ny = int(Input.size(1));
	int Nz = int(Input.size(2));

	// Compute the local means
	for (k=1; k<Nz-1; k++){
		for (j=1; j<Ny-1; j++){
			for (i=1; i<Nx-1; i++){

				imin = std::max(0,i-d);
				jmin = std::max(0,j-d);
				kmin = std::max(0,k-d);
				imax = std::min(Nx-1,i+d);
				jmax = std::min(Ny-1,j+d);
				kmax = std::min(Nz-1,k+d);

				// Populate the list with values in the window
				sum = 0; weight=0;
				for (kk=kmin; kk<kmax; kk++){
					for (jj=jmin; jj<jmax; jj++){
						for (ii=imin; ii<imax; ii++){
							sum += Input(ii,jj,kk);
							weight++;
						}
					}
				}

				Mean(i,j,k) = sum / weight;
			}
		}
	}

	// Compute the non-local means
	for (k=1; k<Nz-1; k++){
		for (j=1; j<Ny-1; j++){
			for (i=1; i<Nx-1; i++){


				if (fabs(Distance(i,j,k)) < THRESHOLD_DIST){
					// compute the expensive non-local means
					sum = 0; weight=0;

					imin = std::max(0,i-d);
					jmin = std::max(0,j-d);
					kmin = std::max(0,k-d);
					imax = std::min(Nx-1,i+d);
					jmax = std::min(Ny-1,j+d);
					kmax = std::min(Nz-1,k+d);

					for (kk=kmin; kk<kmax; kk++){
						for (jj=jmin; jj<jmax; jj++){
							for (ii=imin; ii<imax; ii++){
								float tmp = Mean(i,j,k) - Mean(ii,jj,kk);
								sum += exp(-tmp*tmp*h)*Input(ii,jj,kk);
								weight += exp(-tmp*tmp*h);
							}
						}
					}

					returnCount++;
					//Output(i,j,k) = Mean(i,j,k);
					Output(i,j,k) = sum / weight;
				}
				else{
					// Just return the mean
					Output(i,j,k) = Mean(i,j,k);
				}
			}
		}
	}
	// Return the number of sites where NLM was applied
	PROFILE_STOP("NLM3D");
	return returnCount;
}
