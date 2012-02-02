#pragma once

#include <premia/generator/api/option.h>
#include <premia/generator/karrigell/var.h>

namespace premia {
namespace pygen {
namespace karrigell {

	inline void printMethodForOption(Formatter & out, PricingMethod const * method)
	{
		out("METHOD_NAME", method->name) << "print A('%METHOD_NAME%',href='/validate?m=%MODEL_ID%&f=%FAMILY_NAME%&o=%OPT_NAME%&meth=%METHOD_NAME%') + BR()";
	}

	inline void generateAllMethodsForOption(Formatter & out, Option::AvailableMethods::const_reference r)
	{
		out("MODEL_NAME", r.first->name)
		   ("MODEL_ID", r.first->ID())
			<< (seq, 
				"from HTMLTags import *",
				"print H4('%MODEL_NAME%') + BR()", 
				foreach_x(r.second, printMethodForOption));
	}


	inline void generateOption(Ctx & ctx, Option const & opt)
	{
		Formatter f(ctx.filename(opt));
		f	("OPT_NAME", opt.name)
			("FAMILY_NAME", opt.family.name)
			("OBJ", "opt")
			("BGCOLOR_BASE","opt_colors")
			("ENTITY_NAME", "option")
			<< (seq, 
				"%OBJ% = options.%FAMILY_NAME%.%OPT_NAME%()", "",
				foreach_x(opt.vars, print::Ini),
				"table <= (TR(TD((B('Family:')),align='right') + TD(enum_submit_mod('family', model.families(), '%FAMILY_NAME%'))+TD(),bgcolor=clr(%BGCOLOR_BASE%,clridx)))",
				"clridx = clridx + 1",
				"table <= (TR(TD((B('Option:')),align='right') + TD(enum_submit('option', pricing_options(model.ID(), '%FAMILY_NAME%'),'%OPT_NAME%'))+TD(),bgcolor=clr(%BGCOLOR_BASE%,clridx)))",
				foreach_x(opt.vars, print::Table),
				call(boost::bind(print::Iterables, _1, boost::cref(opt.vars)))
			);

		Formatter ff(ctx.methodsFile(opt));
		ff	("OPT_NAME", opt.name)
			("FAMILY_NAME", opt.family.name)
			<< (seq, 
				"from HTMLTags import *",
				"print H3('Pricings methods available for %OPT_NAME%')",
				foreach_x(opt.methods_for_models, generateAllMethodsForOption));

	}

	inline void printRefToOption(Formatter & out, Option const & opt)
	{
		out("OPT_NAME", opt.name)("FAMILY_NAME",opt.family.name) 
			<< "print A('%OPT_NAME%',href='/premia/opt/%FAMILY_NAME%/%OPT_NAME%_methods') + BR()";
	}

	inline void generateFamily (Ctx & ctx, Family const & f)
	{
		ctx.create(ctx.dir(f));

		std::for_each(f.options.begin(), f.options.end(), boost::bind(generateOption, boost::ref(ctx), _1));

		Formatter ff(ctx.optionsFile(f));
		ff	("FAMILY_NAME", f.name)
			<< (seq, 
				"from HTMLTags import *", 
				foreach_x(f.options, printRefToOption)
			);
	}

	inline void printRefToFamily(Formatter & out, Family const & f)
	{
		out("FAMILY_NAME", f.name) << (seq, 
			"print H4(\"%FAMILY_NAME%\") + BR()",
			foreach_x(f.options, printRefToOption)
			);
	}

	inline Ctx& operator << (Ctx & ctx, Families const & f)
	{
		ctx.create(ctx.opt());

		std::for_each(f.families.begin(), f.families.end(), boost::bind(generateFamily, boost::ref(ctx), _1));

		Formatter ff(ctx.opt() / "all.py");
		ff	<< (seq, "from HTMLTags import *", "print H3('Available instruments')", foreach_x(f.families, printRefToFamily));

		return ctx;
	}

}}}
