#pragma once

#include <premia/generator/api/pricing.h>
#include <premia/generator/karrigell/var.h>

namespace premia {
namespace pygen {
namespace karrigell {


	inline void generatePricingMethod(Ctx & ctx, PricingMethod const & met)
	{
		Formatter f(ctx.filename(met));
		f	("METHOD_NAME", met.name)
			("MODEL_NAME", met.pricing.model->ID())
			("FAMILY_NAME", met.pricing.family->name)
			("BGCOLOR_BASE","met_colors")
			("OBJ", "method")
			<< (seq, 
				"%OBJ% = pricings.%MODEL_NAME%.%MODEL_NAME%_%FAMILY_NAME%.%METHOD_NAME%()", "",
				"if run_computation:", +(seq,
				    "pass",
				    foreach_x(met.members, print::Ini)),
				"table <= (TR(TD((B('Pricing method:')),align='right') + TD(enum_submit('pricing_method', [x for x in pricings.%MODEL_NAME%.%MODEL_NAME%_%FAMILY_NAME%.all() if type(opt) in x.options()],'%METHOD_NAME%'))+TD(),bgcolor=clr(%BGCOLOR_BASE%,clridx)))",
				foreach_x(met.members, print::Table)
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

		for_each(p.methods, boost::bind(generatePricingMethod, boost::ref(ctx), _1));

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
		for_each(p.pricings, boost::bind(generatePricing, boost::ref(ctx), _1));

		return ctx;
	}

}}}
