

#ifndef EL_EXP_hpp
#define EL_EXP_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"

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


//namespace Beagle
//{
//namespace GP
//{

namespace el
{

/*!
 *  \class Exp beagle/GP/Exp.hpp "beagle/GP/Exp.hpp"
 *  \brief GP exponential primitive class.
 *  \ingroup GPF
 *  \ingroup Primit
 *  \ingroup MathPrimit
 */
class Exp : public Beagle::GP::Primitive
{

public:

	//! GP::Exp allocator type.
	typedef Beagle::AllocatorT<Exp,Beagle::GP::Primitive::Alloc>
	Alloc;
	//! GP::Exp handle type.
	typedef Beagle::PointerT<Exp,Beagle::GP::Primitive::Handle>
	Handle;
	//! GP::Exp bag type.
	typedef Beagle::ContainerT<Exp,Beagle::GP::Primitive::Bag>
	Bag;

	explicit Exp(std::string inName="EXP");
	virtual ~Exp()
	{ }

#ifdef BEAGLE_HAVE_RTTI
virtual const std::type_info* getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(Beagle::GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual void execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext);

};


// end Exp Class
} // end namespace el




#endif // Beagle_GP_Exp_hpp
