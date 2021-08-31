

#include "Cos.hpp"
#include <cmath>

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

#include "Table.h"
using namespace el;


/*!
 *  \brief Construct a new cosinus primitive.
 *  \param inName Name of the cosinus primitive.
 */
Cos::Cos(std::string inName) :
		Beagle::GP::Primitive(1, inName)
{ }


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief  Return the tag of the type of data needed as input for the primitive Cos.
 *  \param  inN Index of the argument to get the type tag.
 *  \return Type_info (RTTI) tagging the data type needed.
 *  \throw  AssertException If the index inN given is greater than 0.
 */
const std::type_info* Cos::getArgType(unsigned int inN, Beagle::GP::Context&) const
{
//	Beagle_StackTraceBeginM();
//	Beagle_AssertM(inN<1);
//	return &typeid(Double);
//	Beagle_StackTraceEndM("const std::type_info* GP::Cos::getArgType(unsigned int inN, GP::Context&) const");
    return nullptr;
}


/*!
 *  \brief  Return the tag of the type of data return by primitive Cos.
 *  \return Type_info (RTTI) tagging the data type returned.
 */
const std::type_info* Cos::getReturnType(Beagle::GP::Context&) const
{
//	Beagle_StackTraceBeginM();
//	return &typeid(Double);
//	Beagle_StackTraceEndM("const std::type_info* GP::Cos::getReturnType(GP::Context&) const");
    return nullptr;
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Execute the characteristic operation of a cosinus of number.
 *  \param outResult Result of the cosinus.
 *  \param ioContext Evolutionary context.
 */
void Cos::execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Table & lResult = Beagle::castObjectT<Table&>(outResult);
	get1stArgument(lResult, ioContext);
//	lResult = cos(lResult);
    lResult.getWrappedValue().cos();

	Beagle_StackTraceEndM("void GP::Cos::execute(GP::Datum& outResult, GP::Context& ioContext)");
}

