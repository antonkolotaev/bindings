#pragma once 

namespace premia {
namespace pygen  {
namespace python {

	namespace print {

		/// binds with the formatter label for the enumeration member and its ordinal number
		template <class Pair>
		inline Formatter & operator && (Formatter & out, Pair const & p)
		{
			return out("LABEL", p.second.label)("ID", p.first);
		}

		/// prints initializer part for python map that implements the enum type
		void Ini(Formatter & out, Enum::Members::const_reference p)
		{
			out("LABEL", p.second.quoted_original_label)
			   ("ID", p.first) 
			   << "%ID% : '%LABEL%',";
		}

		/// prints accessors to enum members
		void Prop(Formatter & out, Enum::Members::const_reference p)
		{
			(out && p) << (seq,
					//"@staticmethod", 
					//"def %LABEL%(): return %CLASS%(%ID%)", 
					"def set_%LABEL%(self): self._value = %LABEL%()", 
					"def get_%LABEL%(self):",
					"   assert type(self._value) == %LABEL%",
					"   return self._value", 
					"");
		}
		
		void Label(Formatter & out, Enum::Members::const_reference p)
		{
			(out && p) << "%LABEL%,";
		};
		
		void Choice(Formatter & out, Enum::Members::const_reference p)
		{
			(out && p) << (seq,
			    "", 
			    "class %LABEL%(object):", +(seq,
			        "def __init__(self):", 
			        +call(boost::bind(print::Initializers, _1, p.second.params)), "",
			        "def key(self): return %ID%", "",
			        foreach(p.second.params, boost::bind(print::PropertyEx, _1, _2, boost::cref(p.second.params))),
			        "def export(self):", +(seq,
			            "from premia import interop",
			            "interop.write_enum(%ID%)",
			            foreach(p.second.params, print::copy_param)), "",
			        "def export_ignoring(self):", +(seq,
			            "from premia import interop",
			            "interop.ignore_enum(%ID%)",
			            foreach(p.second.params, print::ignore_param)), "",
			        "def __repr__(self): return '%LABEL%'"));
        }		
	}

	/// generates an enumeration wrapper 
	PyCtx const & operator << (PyCtx const & ctx, Enum const & e)  
	{
		Formatter f(ctx.filename(e));
		f("CLASS", e.label) << (seq,
		         "from premia.common import *", "",
				 "class %CLASS%:", +(seq,
					"_labels = {", +(
						foreach(e.members, print::Ini)),
					"}",
					"def __init__(self, newval): self._value = newval", "",
					"",
					"def __repr__(self): return self._value.__repr__()",
					"",
					"def export(self): self._value.export()",
					"",
					foreach(e.members, print::Prop)), "",
				//"class Base(object):", +(seq,
				//    "@staticmethod",
    			//	"def choices():", +(seq, 
    			//	    "return [", +foreach(e.members, print::Label), "]")),
				foreach(e.members, print::Choice)
			);
			
			
		ctx.out(2) << "   " << e.label << std::endl;

		return ctx;
	}

	/// generates wrappers for all Premia enumerations
	PyCtx const & operator << (PyCtx const & ctx, Enums const & e) 
	{
		ctx.out(1) << "Generating enums:...";

		ctx.create(ctx.enumDir());
		boost::for_each(e.enums() | map_values, ctx << lm::_1);

		ctx.out(1) << "ok!" << std::endl;
		return ctx;
	}

}}}
