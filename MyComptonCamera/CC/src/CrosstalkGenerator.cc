/*! @file CrosstalkGenerator.cc
  @brief Define crosstalk simulation between the different detectors.
  @date September 2010, March 2015
  @author Pico (L.G. Sarmiento lgsarmientop@unal.edu.co)
  @author Modified bye D. Flechas (dcflechasg@unal.edu.co)
  @warning It is not currently implemented in the running version.
  @bug None so far, but this has not been implemented yet.
 */

#include "CrosstalkGenerator.hh"
/*  units  and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

CrosstalkGenerator::CrosstalkGenerator(const double& xtalk , const int& dim) :
	firstNearXtalk(xtalk) ,
	dimension(dim)// ,
	//	xtalk()
{
	Init();
}

void CrosstalkGenerator::Init() {
	//! Construct the crosstalk symmetric matrix.\n
	//! If \c dimension is 1 (no of strips) or <tt>xtalk==0</tt> exit as soon as possible.
  /*	if ( dimension == 1 ) return;
	xtalk = CLHEP::HepSymMatrix(dimension,1);
	if ( firstNearXtalk == 0 ) return;
	CLHEP::HepSymMatrix diag(dimension,1);
	diag *= (-2*firstNearXtalk );
	xtalk += diag;
	CLHEP::HepSymMatrix ofdiag(dimension,0);
	for ( int i = 0;i<dimension-1;++i) ofdiag[i][i+1] = firstNearXtalk;
	xtalk += ofdiag;*/
}

