#ifndef CROSSTALKGENERATOR_HH_
#define CROSSTALKGENERATOR_HH_

/*! @file   CrosstalkGenerator.hh
  @brief Define crosstalk simulation between the different detectors.
  @date September, 2010
  @author Pico (L.G. Sarmiento lgsarmientop@unal.edu.co)
  @version 0.9
  @warning It is not currently implemented in the running version.
  @bug None so far, but this has not been implemented yet.
 */

/*
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Matrix/Vector.h"
*/
/*! @brief Crosstalk generator
 *
 * This class simulates the crosstalk between strips -or in principle any pair of electronic channels-.
 * In the default implementation the crosstalks is applied in first neighbor
 * approximation.
 * For a strip i the charge is modified to be:
 * \f$Q'(i) = (1-2f)Q(i) + fQ(i+1) + fQ(i-1)\f$
 */
class CrosstalkGenerator
{
public:
	/*! @brief Default constructor
	 *
	 * Creates a Crosstalk generator object
	 * For a strip i the charge is modified to be:
	 * \f$Q'(i) = (1-2f)Q(i) + fQ(i+1) + fQ(i-1)\f$
	 * @param xtalk : the fraction of charge leaking
	 * @param dimension : the number of elements (Si strips)
	 */
	CrosstalkGenerator(const double& xtalk , const int& dimension);
	//! Default destructor
	virtual ~CrosstalkGenerator() {};
	/*! @brief Simulate crosstalk
	 *
	 * The crosstalk is applied to the input.
	 * Note that the input vector has to be ordered.
	 */
  //	virtual CLHEP::HepVector operator()( const CLHEP::HepVector& input ) const { return xtalk*input; }
private:
	//! Crosstalk parameter for first neighbors
	double firstNearXtalk;
	//! Parameter defining the number of elements
	int dimension;
	//! Crosstalk matrix representation
  //	CLHEP::HepSymMatrix xtalk;
	/*! @brief Initializes \sa xtalk
	 *
	 * This method can be overwritten to simulate
	 * more complex crosstalk patterns.
	 */
	virtual void Init();
};

#endif /* CROSSTALKGENERATOR_HH_ */
