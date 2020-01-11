#ifndef SRC_TRACKER_ALGORITHMS_PTT_H_
#define SRC_TRACKER_ALGORITHMS_PTT_H_

#include "../../../base/algorithm.h"
#include "../../../math/sphericalHarmonics.h"
#include "fast_PTF_discretizer.h"
#include "fast_PTF.h"
#include "streamline_ptt.h"
#include "tractogram_ptt.h"
#include "algorithm_ptt_defaults.h"

class TrackWith_PTT : public TractographyAlgorithm {

public:

	TrackWith_PTT();
	~TrackWith_PTT();

	virtual Initialization_Decision initialize();
	virtual Propagation_Decision 	propagate(int stepCounter);
	virtual void setSeed();
	virtual void flip();
	virtual void append();
	virtual void removeLast();
	virtual void setStreamline(Streamline *_streamline) { streamline = (Streamline_PTT*)_streamline;};
	virtual void setDefaultParametersWhenNecessary();
	virtual void print();

	FPTF 			*curve;
	void 			 calcLikelihoodAndPosterior();



private:

	void 		 	 get_initial_curve();
	void 		 	 get_a_candidate_curve();
	void 			 estimatePosteriorMax();
	void 		 	 rejectionSample();

	FPTF 			*initial_curve;
    float 		 	 initialPosteriorMax;
    
	float 		 	 posteriorMax;
    
	size_t 		 	 current_init_postEstItCount;
	size_t 		 	 current_prop_postEstItCount;

	float 			*FOD;

	RandomDoer  	*doRandomThings;

	Streamline_PTT 	*streamline;
    
};


#endif
