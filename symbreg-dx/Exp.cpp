
#include "Exp.hpp"
#include "Table.h"
#include <cmath>

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI


using namespace el ;

Exp::Exp(std::string inName) :
		Beagle::GP::Primitive(1, inName)
{ }


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief  Return the tag of the type of data needed as input for the primitive Exp.
 *  \param  inN Index of the argument to get the type tag.
 *  \return Type_info (RTTI) tagging the data type needed.
 *  \throw  AssertException If the index inN given is greater than 0.
 */
const std::type_info* Exp::getArgType(unsigned int inN, Beagle::GP::Context&) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inN<1);
	return &typeid( Table );
	Beagle_StackTraceEndM("const std::type_info* GP::Exp::getArgType(unsigned int inN, GP::Context&) const");
}


/*!
 *  \brief  Return the tag of the type of data return by primitive Exp.
 *  \return Type_info (RTTI) tagging the data type returned.
 */
const std::type_info* Exp::getReturnType(Beagle::GP::Context&) const
{
	Beagle_StackTraceBeginM();
	return &typeid(Table);
	Beagle_StackTraceEndM("const std::type_info* GP::Exp::getReturnType(GP::Context&) const");
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Execute the characteristic operation of a exponential of number.
 *  \param outResult Result of the exponential.
 *  \param ioContext Evolutionary context.
 */
void Exp::execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Table& lResult = Beagle::castObjectT<Table&>(outResult);
	get1stArgument(lResult, ioContext);
	lResult.getWrappedValue().exp();
//	if(lResult < 80.) lResult = std::exp(lResult);
//	else lResult = 1.;
	Beagle_StackTraceEndM("void GP::Exp::execute(GP::Datum& outResult, GP::Context& ioContext)");
}

