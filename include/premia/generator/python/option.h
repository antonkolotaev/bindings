#pragma once

namespace premia {
namespace pygen {
namespace python {

	/// generates wrapper for an option
	PyCtx const & operator << (PyCtx const & ctx, Option const & opt)
	{
		Formatter f(ctx.filename(opt));
		f	("NAME",	  opt.name)
			("OPTION_ID", id(opt))
			("ASSET_ID",  id(opt.family.asset))
			("FAMILY_ID", id(opt.family))
			<< (seq, 
				"from ...common import *", "",
				"class %NAME%(object):", +(seq, "",
					"def __init__(self):", 
						+call(boost::bind(print::Initializers, _1, boost::ref(opt.vars))), "", 
					foreach_x(opt.vars, boost::bind(print::PropertyEx, _1, _2, boost::cref(opt.vars))), "", 
					"def __repr__(self): return getRepr(self, 'Option')", 
 					"def makeCurrent(self):", +(seq,
						"from premia import interop",
 						"interop.setCurrentAsset(%ASSET_ID%)",
 						"interop.setCurrentOption(%FAMILY_ID%, %OPTION_ID%)",
						foreach_x(opt.vars, print::copy_param),
						"interop.stopWriteParameters()"
 					), "",
					"@staticmethod",
					"def parameters(): ", +(seq, 
						"return [", +foreach_x(opt.vars, print::member), "]")
					))
			;

		return ctx;
	}

	namespace print {

		/// prints import option clause in a family file
		void OptionUsing(Formatter & out, Option const & opt)
		{
			out("NAME", opt.name) << "from .%NAME% import %NAME%";
		}

		/// mentions an option in a family file
		void OptType(Formatter & out, Option const & opt)
		{
			out("NAME", opt.name) << "%NAME%,";
		}
	}

	/// generates wrappers for all options in the family and a file listing options in the family
	PyCtx const & operator << (PyCtx const & ctx, Family const & f)
	{
		ctx.create(ctx.dir(f));
		// generating option wrappers
		for_each(f.options, boost::ref(ctx) << lm::_1);

		// creating family's option listing
		Formatter ff(ctx.filename(f));
		ff << (seq, 
			foreach_x(f.options, print::OptionUsing), "", 
			"def all(): return [", +foreach_x(f.options, print::OptType), "]");

		/* ctx.out(2) << "   " << f.name << std::endl; */

		return ctx;
	}

	namespace print 
	{
		/// prints import family clause for all families file
		void importFamily(Formatter & out, std::string const & family_name)
		{
			out("NAME", family_name) << "from .opt.%NAME% import %NAME%";
		}

		/// mentions the family in all families file
		void family(Formatter & out, std::string const & family_name)
		{
			out("NAME", family_name) << "%NAME%,";
		}
	}

	/// generates all options of all families and creates all families file
	PyCtx const& operator << (PyCtx const& ctx, Families const & fs)
	{
		ctx.out(1) << "Generating option families:...";

		ctx.create(ctx.opt());

		for_each(fs.families, ctx << lm::_1);

		Formatter f(ctx.optionsPy());
		f << (seq, 
			foreach_x(fs.names | map_keys, print::importFamily), "", 
			"def all(): return [", +foreach_x(fs.names | map_keys, print::family), "]");

		/* ctx.out(1) << "ok!" << std::endl; */

		return ctx;
	}
}}}
