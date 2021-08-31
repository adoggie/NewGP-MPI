#include "Table.h"
#include "beagle/GP.hpp"
#include "SymbRegEvalOp.hpp"
#include "NumCpp.hpp"
#include <cmath>
#include "Config.h"

using namespace Beagle;

SymbRegEvalOp::SymbRegEvalOp() :
		GP::EvaluationOp("SymbRegEvalOp")
{ }

Fitness::Handle SymbRegEvalOp::evaluate(GP::Individual& inIndividual, GP::Context& ioContext){

    auto &data = mDataSet->dataset();
    nc::NdArray<double> lSquareError(TEST_SAMPLE_ROWS,TEST_SAMPLE_COLS);

    for(auto & xy : data){
        auto &x = std::get<0>(xy);
        auto &y = std::get<1>(xy);
        setValue("X", el::Table(x), ioContext);
        el::Table  lResult = el::RawTable(nc::Shape(TEST_SAMPLE_ROWS,TEST_SAMPLE_COLS) );

        inIndividual.run(lResult, ioContext);
        auto lError = y.data() - lResult.getWrappedValue().data();
        auto t = nc::power(lError,2);
        lSquareError += t;
    }

    nc::NdArray<double> z(lSquareError.shape());

    auto lMSE  = lSquareError / (double)data.size();
    auto lRMSE = nc::sqrt(lMSE);

    double lFitness = 0;
    for(auto& e : lRMSE){
        lFitness += 1. / (1. + e);
    }
    lFitness =  lFitness / lSquareError.size();
    return new FitnessSimple(lFitness);

/*
	double lSquareError = 0.;
	for(unsigned int i=0; i<mDataSet->size(); i++) {
		Beagle_AssertM((*mDataSet)[i].second.size() == 1);
		const Double lX((*mDataSet)[i].second[0]);
		setValue("X", lX, ioContext);
		const Double lY((*mDataSet)[i].first);
		Double lResult;
		inIndividual.run(lResult, ioContext);
		const double lError = lY-lResult;
		lSquareError += (lError*lError);
	}
	const double lMSE  = lSquareError / mDataSet->size();
	const double lRMSE = sqrt(lMSE);
	const double lFitness = 1. / (1. + lRMSE);
	return new FitnessSimple(lFitness);
 */
}

void SymbRegEvalOp::init(System& ioSystem)
{
	GP::EvaluationOp::init(ioSystem);

	// Get reference to data set component
	mDataSet = castHandleT<el::DataSetRegression>(ioSystem.getComponent("DataSetRegression"));
	if(mDataSet == NULL) {
		throw Beagle_RunTimeExceptionM("'DataSetRegression' component not found in System, impossible to proceed further!");
	}
    auto & dataset = mDataSet->dataset();
	if( dataset.size()){
	    return ;
	}
	constexpr int N = 10;
	for(int n = 0;n<N;n++){
	    nc::NdArray<double> x;
	    x = nc::random::randFloat<double>(nc::Shape(TEST_SAMPLE_ROWS,TEST_SAMPLE_COLS),-1,1);

	    nc::NdArray<double> y(x.shape());
	    std::transform(x.begin(),x.end(),y.begin(),[](auto & lX){
	        double lY = lX*(lX*(lX*(lX+1.)+1.)+1.);
	        return lY;
	    });
	    dataset.push_back(std::make_tuple(x,y));
	}


	// If data set is empty, initialize it with random samples of quartic polynomial
//	if(mDataSet->size() == 0) {
//		mDataSet->resize(20);
//		for(unsigned int i=0; i<mDataSet->size(); i++) {
//			const double lX = ioSystem.getRandomizer().rollUniform(-1., 1.);
//			const double lY = lX*(lX*(lX*(lX+1.)+1.)+1.);
//			(*mDataSet)[i].first = lY;
//			(*mDataSet)[i].second.resize(1);
//			(*mDataSet)[i].second[0] = lX;
//		}
//	}
}
