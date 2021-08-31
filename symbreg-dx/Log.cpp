
#include "Log.hpp"
#include "Table.h"

#include <cmath>

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

using namespace el;


/*!
 *  \brief Construct a new neperian logarithm primitive.
 *  \param inName Name of the neperian logarithm primitive.
 */
Log::Log(std::string inName) :
		Beagle::GP::Primitive(1, inName)
{ }


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief  Return the tag of the type of data needed as input for the primitive Log.
 *  \param  inN Index of the argument to get the type tag.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type needed.
 *  \throw  AssertException If the index inN given is greater than 0.
 */
const std::type_info* Log::getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inN<1);
	return &typeid(Table);
	Beagle_StackTraceEndM("const std::type_info* Log::getArgType(unsigned int inN, Context& ioContext) const");
}


/*!
 *  \brief  Return the tag of the type of data return by primitive Log.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type returned.
 */
const std::type_info* Log::getReturnType(Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	return &typeid(Table);
	Beagle_StackTraceEndM("const std::type_info* Log::getReturnType(Context& ioContext) const");
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Execute the characteristic operation of a protected neperian logarithm of Double.
 *  \param outResult Result of the neperian logarithm.
 *  \param ioContext Evolutionary context.
 *  \par Note:
 *    The Log primitive give as result 1. when the input is 0.0.
 *    The logarithm is calculated on the absolute value of the input.
 */
void Log::execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Table& lResult = Beagle::castObjectT<Table&>(outResult);
	get1stArgument(lResult, ioContext);
	lResult.getWrappedValue().log();
//	if(fabs(lResult) > 0.000001) lResult = log(fabs(lResult));
//	else lResult = 1.;
	Beagle_StackTraceEndM("void Log::execute(Datum& outResult, Context& ioContext)");
}

