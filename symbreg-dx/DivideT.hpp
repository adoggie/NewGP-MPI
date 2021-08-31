
#ifndef EL_GP_DivideT_hpp
#define EL_GP_DivideT_hpp

#include <cmath>
#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/GP/Primitive.hpp"
#include "beagle/GP/PrimitiveInline.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Datum.hpp"
#include "Table.h"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI


namespace el
{
template <class T>
class DivideT : public Beagle::GP::Primitive
{

public:

	//! GP::DivideT allocator type.
	typedef Beagle::AllocatorT<DivideT<T>,Beagle::GP::Primitive::Alloc>
	Alloc;
	//! GP::DivideT handle type.
	typedef Beagle::PointerT<DivideT<T>,Beagle::GP::Primitive::Handle>
	Handle;
	//! GP::DivideT bag type.
	typedef Beagle::ContainerT<DivideT<T>,Beagle::GP::Primitive::Bag>
	Bag;

	explicit DivideT(std::string inName="DIV");
	virtual ~DivideT()
	{ }

#ifdef BEAGLE_HAVE_RTTI
    virtual const std::type_info* getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(Beagle::GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual std::string deparse(std::vector<std::string>& lSubTrees) const;
	virtual void        execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext);

};

typedef DivideT<el::Table> Divide;

}



/*!
 *  \brief Construct a new division primitive.
 *  \param inName Name of the division primitive.
 */
template <class T>
el::DivideT<T>::DivideT(std::string inName) :
		Beagle::GP::Primitive(2, inName)
{ }


/*!
 *  \brief Write this primitive as a C function
 */
template <class T>
std::string el::DivideT<T>::deparse(std::vector<std::string>& lSubTrees) const
{
	Beagle_AssertM(getNumberArguments()==2);
	Beagle_AssertM(lSubTrees.size() == 2);
	std::ostringstream lOSS;
	lOSS << "(" << lSubTrees[0] << "/" << lSubTrees[1] << ")";
	return lOSS.str();
}


#ifdef BEAGLE_HAVE_RTTI


template <class T>
const std::type_info* el::DivideT<T>::getArgType(unsigned int inN,
        Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inN<2);
	return &typeid(T);
	Beagle_StackTraceEndM("const std::type_info* GP::DivideT<T>::getArgType(unsigned int inN, GP::Context& ioContext) const");
}


template <class T>
const std::type_info* el::DivideT<T>::getReturnType(Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	return &typeid(T);
	Beagle_StackTraceEndM("const std::type_info* GP::DivideT<T>::getReturnType(GP::Context& ioContext) const");
}

#endif // BEAGLE_HAVE_RTTI


template <class T>
void el::DivideT<T>::execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	T& lResult = Beagle::castObjectT<T&>(outResult);
	T lArg2;
	get1stArgument(lResult, ioContext);
	get2ndArgument(lArg2, ioContext);
//	if((lArg2 < T(0.001)) && (T(-0.001) < lArg2)) lResult = T(1.0);
//	else lResult /= lArg2;
    lResult.getWrappedValue() /= lArg2.getWrappedValue();
	Beagle_StackTraceEndM("void GP::DivideT<T>::execute(GP::Datum& outResult, GP::Context& ioContext)");
}


#endif //
