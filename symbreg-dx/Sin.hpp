
#ifndef EL_SIN_hpp
#define EL_SIN_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
//#include "beagle/GP/Primitive.hpp"
#include "beagle/IndividualBag.hpp"

#include "beagle/WrapperT.hpp"
#include "beagle/Double.hpp"
#include "beagle/GP/Primitive.hpp"
#include "beagle/GP/PrimitiveInline.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Datum.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

//
namespace el
{

class Sin : public Beagle::GP::Primitive
{

public:

	//! GP::Cos allocator type.
	typedef Beagle::AllocatorT<Sin,Primitive::Alloc>
	Alloc;
	//! GP::Cos handle type.
	typedef Beagle::PointerT<Sin,Primitive::Handle>
	Handle;
	//! GP::Cos bag type.
	typedef Beagle::ContainerT<Sin,Primitive::Bag>
	Bag;

	explicit Sin(std::string inName="SIN");
	virtual ~Sin()
	{ }

#ifdef BEAGLE_HAVE_RTTI
    virtual const std::type_info* getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(Beagle::GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual void execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext);

};


}


#endif //