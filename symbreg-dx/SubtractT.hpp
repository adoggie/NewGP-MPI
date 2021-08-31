

#ifndef EL_GP_SubtractT_hpp
#define EL_GP_SubtractT_hpp

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
class SubtractT : public Beagle::GP::Primitive
{

public:

	//! GP::SubtractT allocator type.
	typedef Beagle::AllocatorT<SubtractT<T>,Beagle::GP::Primitive::Alloc>
	Alloc;
	//! GP::SubtractT handle type.
	typedef Beagle::PointerT<SubtractT<T>,Beagle::GP::Primitive::Handle>
	Handle;
	//! GP::SubtractT bag type.
	typedef Beagle::ContainerT<SubtractT<T>,Beagle::GP::Primitive::Bag>
	Bag;

	explicit SubtractT(std::string inName="SUB");
	virtual ~SubtractT()
	{ }

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(Beagle::GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual std::string deparse(std::vector<std::string>& lSubTrees) const;
	virtual void        execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext);

};


typedef SubtractT< el::Table > Subtract;

}


template <class T>
el::SubtractT<T>::SubtractT(std::string inName) :
		Beagle::GP::Primitive(2, inName)
{ }


template <class T>
std::string el::SubtractT<T>::deparse(std::vector<std::string>& lSubTrees) const
{
	Beagle_AssertM(getNumberArguments()==2);
	Beagle_AssertM(lSubTrees.size() == 2);
	std::ostringstream lOSS;
	lOSS << "(" << lSubTrees[0] << "-" << lSubTrees[1] << ")";
	return lOSS.str();
}


#ifdef BEAGLE_HAVE_RTTI


template <class T>
const std::type_info* el::SubtractT<T>::getArgType(unsigned int inN,
        Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inN<2);
	return &typeid(T);
	Beagle_StackTraceEndM("const std::type_info* GP::SubtractT<T>::getArgType(unsigned int inN, GP::Context& ioContext) const");
}


template <class T>
const std::type_info* el::SubtractT<T>::getReturnType(Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	return &typeid(T);
	Beagle_StackTraceEndM("const std::type_info* GP::SubtractT<T>::getReturnType(GP::Context& ioContext) const");
}

#endif // BEAGLE_HAVE_RTTI


template <class T>
void el::SubtractT<T>::execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	T& lResult = Beagle::castObjectT<T&>(outResult);
	T lArg2;
	get1stArgument(lResult, ioContext);
	get2ndArgument(lArg2, ioContext);
	lResult.getWrappedValue() -= lArg2.getWrappedValue();
	Beagle_StackTraceEndM("void GP::SubtractT<T>::execute(GP::Datum& outResult, GP::Context& ioContext)");
}


#endif //
