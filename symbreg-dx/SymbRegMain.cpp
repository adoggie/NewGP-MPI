
#include "beagle/GP.hpp"
#include "SymbRegEvalOp.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>

using namespace std;
using namespace Beagle;

#include "NumCpp.hpp"
#include "Table.h"
#include "AddT.hpp"
#include "SubtractT.hpp"
#include "MultiplyT.hpp"
#include "DivideT.hpp"
#include "Sin.hpp"
#include "Cos.hpp"
#include "Exp.hpp"
#include "Log.hpp"

#include "DataSetRegression.hpp"

#include "Config.h"

void test_table(){
    nc::NdArray<double> x;
    x = nc::random::randFloat<double>({5,5},-1,1);

    nc::NdArray<double> y(x.shape());
    std::transform(x.begin(),x.end(),y.begin(),[](auto & lX){
        double lY = lX*(lX*(lX*(lX+1.)+1.)+1.);
        return lY;
    });
    x.print();
    std::cout<<"-----"<< std::endl;

    y.print();
}

int _main(int argc, char *argv[]){
//    auto a = nc::random::randInt<>({5, 5}, 0, 100);
//    std::cout << a;
//    test_table();
    test_numcpp();
    return 0;
	try {
		// Build primitives
		GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet;
		lSet->insert(new el::Add );
		lSet->insert(new el::Subtract);
		lSet->insert(new el::Multiply);
		lSet->insert(new el::Divide);
		lSet->insert(new el::Sin);
		lSet->insert(new el::Cos);
		lSet->insert(new el::Exp);
		lSet->insert(new el::Log);
		lSet->insert(new GP::TokenT<el::Table>("X"));
//		lSet->insert(new GP::TokenT<Double>("X"));
//		lSet->insert(new GP::EphemeralDouble);
		// lSet->insert(new GP::TokenT<Double>("Pi", Double(M_PI)));

		// Build a system with GP package
		System::Handle lSystem = new System;
		lSystem->addPackage(new GP::PackageBase(lSet));
		lSystem->addPackage(new GP::PackageConstrained);

		// Add data set for regression component
		lSystem->addComponent(new el::DataSetRegression);

		// Add evaluation operator allocator
		lSystem->setEvaluationOp("SymbRegEvalOp", new SymbRegEvalOp::Alloc);

		// Initialize the evolver
		Evolver::Handle lEvolver = new Evolver;
		lEvolver->initialize(lSystem, argc, argv);

		// Create population
		Vivarium::Handle lVivarium = new Vivarium;

		// Launch evolution
		lEvolver->evolve(lVivarium, lSystem);

	} catch(Exception& inException) {
		inException.terminate();
	} catch(exception& inException) {
		cerr << "Standard exception caught:" << endl;
		cerr << inException.what() << endl << flush;
		return 1;
	} catch(...) {
		cerr << "Unknown exception caught!" << endl << flush;
		return 1;
	}
	return 0;
}

