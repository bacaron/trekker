#include "config_tracker.h"
#include "../base/tractogram.h"

#include "../math/sphericalHarmonics.h"
#include "../base/algorithm.h"

#include "../tracker/algorithms/ptt/algorithm_ptt.h"
#include "../tracker/algorithms/ptt/tractogram_ptt.h"
#include "../tracker/algorithms/local_probabilistic/algorithm_local_probabilistic.h"
#include "../tracker/algorithms/local_probabilistic/tractogram_local_probabilistic.h"

using namespace GENERAL;
using namespace SH;

namespace TRACKER {

FOD_Image* img_FOD            		    = new FOD_Image;
Algorithm      		   algorithm        = ALGORITHM_NOTSET;
bool 				   defaultsSet 		= false;
TractographyAlgorithm *method 			= NULL;
Tractogram            *tractogram 		= NULL;

// Common tracking parameters
float stepSize       					= NOTSET;
float minRadiusOfCurvature   			= NOTSET;
float minFODamp      					= NOTSET;
float minLength      					= NOTSET;
float maxLength      					= NOTSET;
AtMaxLength atMaxLength 				= ATMAXLENGTH_NOTSET;
Directionality directionality 			= DIRECTIONALITY_NOTSET;
int   writeInterval  					= NOTSET;
int	  initMaxEstTrials 					= NOTSET;
int   propMaxEstTrials    				= NOTSET;

// PTT parameters
float probeLength    					= NOTSET;
float probeRadius    					= NOTSET;
int   probeQuality   					= NOTSET;
int   probeCount 						= NOTSET;


// Derived parameters
float maxCurvature   					= NOTSET;
float writeStepSize                  	= NOTSET;
float smallestPixDim 					= 0.0;
float varCurvature   					= NOTSET;
float varStraighten   					= NOTSET;
bool priorComputationFlag 				= true;
bool straighteningComputationFlag 		= true;
float gaussianScalingFactor             = NOTSET;
float gaussianPeakLocationFactor        = NOTSET;
int posteriorMaxEstimationInterval 		= NOTSET;
int triesPerRejectionSampling      		= NOTSET;
float posteriorMaxEstimationStepSize 	= NOTSET;
AtInit atInit 							= ATINIT_NOTSET;

neighborhoodMode neighborhoodSamplingMode = NEIGHBORHOODMODE_NOTSET;

void cleanConfigTracker() {
	delete img_FOD;
	SH::clean();
	delete method;
	delete tractogram;
}

void setDefaultParametersWhenNecessary() {

	smallestPixDim = img_FOD->getSmallestPixdim();

	// Handle algorithm
	if (algorithm==ALGORITHM_NOTSET) {
		algorithm = PTT;
		if (GENERAL::verboseLevel>MINIMAL) std::cout << "Using default algorithm      : PTT " << std::endl;
	}

	switch (TRACKER::algorithm) {
	case PTT:
		tractogram 								= new Tractogram_PTT();
		method 									= new  TrackWith_PTT();
		break;
	case LOCAL_PROBABILISTIC:
		tractogram 								= new Tractogram_Local_Probabilistic();
		method 									= new  TrackWith_Local_Probabilistic();
		break;
	default :
		break;
	}

	method->setDefaultParametersWhenNecessary();

	SH::precompute(1024);
	defaultsSet = true;
}


void readFODImage() {
	if (GENERAL::verboseLevel!=QUITE) std::cout << "Reading FOD image                  : " << img_FOD->getFilePath() << std::endl;
	if(!img_FOD->readImage()) exit(EXIT_FAILURE);
}


void print() {
	std::cout << std::endl;
	std::cout << "TRACKER OPTIONS"<< std::endl;

	method->print();

	if (GENERAL::verboseLevel>ON) std::cout << std::endl << "-----------------" << std::endl;
	std::cout << "fod                  : "  << img_FOD->getFilePath()  << std::endl;
	if (GENERAL::verboseLevel>ON) img_FOD->printInfo();
	if (GENERAL::verboseLevel>ON) std::cout << "-----------------" << std::endl << std::endl;
}

}