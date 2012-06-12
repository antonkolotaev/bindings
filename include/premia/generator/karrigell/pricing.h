#pragma once

#include <premia/generator/api/pricing.h>
#include <premia/generator/karrigell/var.h>
#include <premia/generator/karrigell/result_kinds.h>

namespace premia {
namespace pygen {
namespace karrigell {

   inline void printResultElement(Formatter &out, NamedVar const &vr)
   {
      out ("VLABEL", vr.name)
          ("FRIENDLY", vr.src->Vname)
      << "('%VLABEL%', '%FRIENDLY%'),";
   }
   
   inline void printIterateCheckBoxes(Formatter &out, VarList const &results)
   {
      out << "result_members = [";
      out << +foreach_x(results, printResultElement);
      out << "]";
      out << "v.printResultSeries(result_members)";
   }
    
	inline void generatePricingMethod(Ctx & ctx, PricingMethod const & met)
	{
		Formatter f(ctx.filename(met));
		f	("METHOD_NAME", met.name)
			("MODEL_NAME", met.pricing.model->ID())
			("FAMILY_NAME", met.pricing.family->name)
			("BGCOLOR_BASE","met_colors")
			("OBJ", "method")
			("ENTITY_NAME", "method")
			<< (seq, 
			   foreach_x(met.members, print::includeEnums),
			   "from premia.mod.%MODEL_NAME%.%MODEL_NAME%_%FAMILY_NAME%.%METHOD_NAME% import %METHOD_NAME%",
			   "def %OBJ%_%MODEL_NAME%_%FAMILY_NAME%_%METHOD_NAME%(v):",
				"   v.setMethod(%METHOD_NAME%, '%METHOD_NAME%')", "",
 				"   method = v.method",
				+foreach_x(met.members, print::Table),
				+call(boost::bind(print::Iterables, _1, boost::cref(met.members))),
				+call(boost::bind(printIterateCheckBoxes, _1, boost::cref(met.results))),
				"   return v"
			);
	}

	inline void printCompatibleOption(Formatter & out, Option const * opt)
	{
		out("OPT_NAME", opt->name)
			<< " + TD(A('%OPT_NAME%', href='/validate?m=%MODEL_ID%&f=%FAMILY_NAME%&o=%OPT_NAME%&meth=%METHOD_NAME%')) \\";
	}

	inline void printRefsToMethod(Formatter & out, PricingMethod const & method)
	{
		out("METHOD_NAME", method.name) 
			<< (seq, 
				"table <= TR(TD('%METHOD_NAME%:',align='right') \\", 
					+foreach_x(method.compatible_options, printCompatibleOption),
				")",
				"");
	}

	inline void generatePricing (Ctx & ctx, Pricing const & p)
	{
		ctx.create(ctx.dir(p));

		std::for_each(p.methods.begin(), p.methods.end(), boost::bind(generatePricingMethod, boost::ref(ctx), _1));

		Formatter f(ctx.methodsFile(p));
		f	("MODEL_ID", p.model->ID())
			("FAMILY_NAME", p.family->name)
			<< (seq, 
				"from HTMLTags import *",
				"print H3('Methods of the pricing %MODEL_ID%_%FAMILY_NAME%')",
				"table = TABLE(Class=\"content\")",
				foreach_x(p.methods, printRefsToMethod),
				"print table"
			);
	}

	inline Ctx& operator << (Ctx & ctx, Pricings const & p)
	{
	   ctx << ResultKindsInitialized();
	   
		std::for_each(p.pricings.begin(), p.pricings.end(), boost::bind(generatePricing, boost::ref(ctx), _1));

		return ctx;
	}

}}}
