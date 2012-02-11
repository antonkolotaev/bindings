#pragma once

#include <premia/generator/utils/formatter_dsl.h>
#include <premia/generator/utils/symbols.h>
#include <premia/generator/karrigell/ctx.h>
#include <premia/generator/api/var_wrapper.h>

namespace premia	{
namespace pygen		{
namespace karrigell {

    using api::EnumValue;

	namespace print 
	{
	    inline void IniEx(Formatter & out, NamedVar const & vr, std::string const &prefix);
	    
	    inline void enumParams(Formatter &out, NamedVar const &vr)
	    {
	        IniEx(out("OBJ", "pmem.get_%LABEL%()"), vr, "src + '_get_%LABEL%_'");
	    }
	    
	    inline void enumChoices(Formatter &out, api::Enum const &e)
	    {
	        int idx = 0;
	        BOOST_FOREACH(api::Enum::Members::const_reference em, e.members)
	        {
	            out("IDX",idx)("LABEL", em.second.label) 
	                << (seq, 
	                "if e == %IDX%:", +(seq, 
	                    "pmem.set_%LABEL%()",
	                    foreach_x(em.second.params, enumParams)));
	            ++idx;
	        }
	    } 
		struct param_loader : var_visitor<param_loader>
		{
			param_loader(Formatter & out, VAR const* src = 0) : out(out), src(src) {}

			void operator () (Numeric<int> const & i) 
			{
			    out << "loadScalar('%VAR_NAME%', %PREFIX%, %OBJ%, int)";
			}

			void operator () (Numeric<long> const & i) 
			{
			    out << "loadScalar('%VAR_NAME%', %PREFIX%, %OBJ%, long)";
			}

			void operator () (Numeric<double> const & i) 
			{
			    out << "loadScalar('%VAR_NAME%', %PREFIX%, %OBJ%, float)";
			}

			void operator () (std::string const & i)  
			{
			    out << "loadScalar('%VAR_NAME%', %PREFIX%, %OBJ%, str)";
			}

			void operator () (std::vector<double> const & i) 
			{
			   if (src && src->Vtype == PNLVECTCOMPACT)
			    out << "loadVectorCompact('%VAR_NAME%', %PREFIX%, %OBJ%)";
			   else
			    out << "loadVector('%VAR_NAME%', %PREFIX%, %OBJ%)";
			}
			
			void operator() (EnumValue const & e) 
			{
				out("ENUM_NAME", e.type->label) 
				   << (seq,
                  "load_%ENUM_NAME%('%VAR_NAME%', %PREFIX%, %OBJ%)");
			}
			
			
		private:
			Formatter & out;
         VAR const * src;
		};

		inline void IniEx(Formatter & out, NamedVar const & vr, std::string const &prefix)
		{
			param_loader loader(out("VAR_NAME", vr.name)("VAR_ID", symbol_utils::replaceNonAlnumCharacters(out.lookupVar("OBJ").c_str()) + "_" + vr.name)("PREFIX", prefix), vr.src); 
			
			loader.apply(vr.value);
			
			if (vr.name == "Model_Size")
			{
				out << "N_Dim = model._Model_Size";
			} 
		}

		inline void Ini(Formatter & out, NamedVar const & vr)
		{
		   IniEx(out, vr, std::string("'")+symbol_utils::replaceNonAlnumCharacters(out.lookupVar("OBJ").c_str())+"'");
		}
		struct include_enums : var_visitor<include_enums>
		{
			include_enums(Formatter & out) : out(out){}

         template <class T>
			void operator () (Numeric<T> const & i) 
			{
			}

			void operator () (std::string const & i)  
			{
			}

			void operator () (std::vector<double> const & i) 
			{
			}
			
			void operator() (EnumValue const & e) 
			{
				out("ENUM_NAME", e.type->label) 
				   << "Include(r'/premia/enum/%ENUM_NAME%.py')";
			}
			
			
		private:
			Formatter & out;
		};
		
		inline void includeEnums(Formatter &out, NamedVar const &vr)
		{
		   include_enums(out).apply(vr.value);
		}
		
		template <class Scalar>
			std::string const & tostr(api::Range<Scalar> const * c)
		{
			// since number of different ranges is small, let's memoize their representations
			typedef std::map<Range<Scalar> const *, std::string> Cache;

			static Cache	cache;

			// looking in the cache
			typename Cache::const_iterator it = cache.find(c);

			if (it != cache.end())
				return it->second;
			else	// if not in the cache
			{	
				std::string res;

				if (c != 0) 
				{
					res = "in ";

					res += c->low_inclusive ? "[" : "(";

					res += c->has_low() ? str(c->low) : "-inf";

					res += ";";

					res += c->has_hi() ? str(c->hi) : "+inf";

					res += c->hi_inclusive ? "]" : ")";
				}

				cache[c] = res;

				return cache[c];
			}
		}

		template <class T> const char * symbol() { return "Z"; }
		template <> const char * symbol<double>(){ return "R"; }

		inline void TableEx(Formatter & out, NamedVar const & vr);

		struct tbl_val_printer : var_visitor<tbl_val_printer>
		{
			tbl_val_printer(Formatter & out, VAR const *src, bool iterable) : out(out), src(src), iterable(iterable) {}
			
			static std::string row(std::string const &s)
			{
			    return "table <= TR(TD('%FRIENDLY_NAME%',align='right') + " + s + ",bgcolor=clr(%BGCOLOR_BASE%,clridx))";
			}

			template <class Scalar>
				void operator () (Numeric<Scalar> const & i) 
			{
				out("CONSTR",tostr(i.constraint))
				   ("SYMB", symbol<Scalar>())
				   ("ITERABLE", iterable ? "True" : "False")	
				   << "processScalar(ctx, table, %BGCOLOR_BASE%, %OBJ%,'_%VAR_NAME%', '%FRIENDLY_NAME%', %PREFIX%+'_%VAR_NAME%', '%SYMB% %CONSTR%', %ONCHANGE%, %ITERABLE%)"
;
			}

			/// assert for FILENAME
			void operator () (std::string const & i)  
			{
			   out << "processScalar(ctx, table, %BGCOLOR_BASE%, %OBJ%,'_%VAR_NAME%', '%FRIENDLY_NAME%', %PREFIX%+'_%VAR_NAME%', '', '', False)";
			}

			/// assert for PNLVECT and PNLVECTCOMPACT
			void operator () (std::vector<double> const & i) 
			{
			   if (src && src->Vtype==PNLVECTCOMPACT)
			      out <<
			      "processVectorCompact(ctx, table, %BGCOLOR_BASE%, %OBJ%,'_%VAR_NAME%', '%FRIENDLY_NAME%', %PREFIX%+'_%VAR_NAME%')";
			   else
			      out <<
			      "processVector(ctx, table, %BGCOLOR_BASE%, %OBJ%,'_%VAR_NAME%', '%FRIENDLY_NAME%', %PREFIX%+'_%VAR_NAME%')";
			}
			
			/// assert for ENUM
			void operator() (EnumValue const & e) 
			{
			    out("ENUM_TYPE", e.type->label) 
				   << "process_%ENUM_TYPE%(ctx, table, %BGCOLOR_BASE%, '%FRIENDLY_NAME%', %OBJ%._%VAR_NAME%, %PREFIX%+'_%VAR_NAME%')";
			}
		private:
			Formatter & out;
         VAR const * src;
         bool iterable;
		};
		
		inline void TableVal(Formatter & out, NamedVar const & vr)
		{
		    if (vr.has_setter) std::cout << vr.name << std::endl;
		    std::string submit = vr.has_setter ? ", onchange='submit();'": "";
		    std::string onchange = vr.has_setter ? "'submit();'": "''";
			tbl_val_printer(out("SUBMIT", submit)("ONCHANGE",onchange), vr.src, vr.iterable).apply(vr.value);
		}

		inline void TableEx(Formatter & out, NamedVar const & vr)
		{
		    std::string star;
		    if (vr.iterable) star = "*";
			out("VAR_NAME", vr.name)
			   ("VAR_ID", symbol_utils::replaceNonAlnumCharacters(out.lookupVar("OBJ").c_str()) + "_" + vr.name)
               ("FRIENDLY_NAME", star + vr.src->Vname)
			   << (seq, call(boost::bind(TableVal, _1, vr)));
		}

		inline void Table(Formatter & out, NamedVar const & vr)
		{
		   TableEx(out("PREFIX", std::string("'")+symbol_utils::replaceNonAlnumCharacters(out.lookupVar("OBJ").c_str())+"'"), vr);
		}

		struct iterable_name_printer : var_visitor<iterable_name_printer>
		{
			iterable_name_printer(Formatter & out, bool iterable) : out(out), iterable(iterable) {}
			
			template <class Scalar>
				void operator () (Numeric<Scalar> const & i) 
			{
                if (iterable)
                {
                  out << "iterScalar(ctx, %OBJ%, '%VNAME%', '%VARNAME%', %PREFIX% + '_%VNAME%')";

                }
			}

			void operator () (std::string const & i)  
			{
			}

			void operator () (std::vector<double> const & i) 
			{
                  out << "iterVector(ctx, %OBJ%, '%VNAME%', '%VARNAME%', %PREFIX% + '_%VNAME%')";
			}
			
			void operator() (EnumValue const & e) 
			{
			   out("ENUM_TYPE",e.type->label) 
			   << "iterables_%ENUM_TYPE%(ctx, %OBJ%.%VNAME%, %PREFIX% + '_%VNAME%')";
			}
		private:
			Formatter & out;
			bool iterable;

		};
		
		  inline std::string quote(std::string const &s)
		  {
		      return "'" + s + "'";
		  }
		
        inline void IterableEx(Formatter &out, NamedVar const &vr)
        {
            iterable_name_printer(
               out("VARNAME",vr.src->Vname)
                  ("VNAME",vr.name)
                  ,
               vr.iterable
            ).apply(vr.value);
        }
        
        inline void Iterable(Formatter &out, NamedVar const &vr)
        {
            IterableEx(out("PREFIX",/*out.lookupVar("ENTITY_NAME")*/"''"), vr);
        }

        inline void Iterables(Formatter& out, VarList const &vars)
        {
            out << (seq, 
                    //"ctx = Ctx()",
                    //foreach_x(vars,Iterable),
                    "clridx = clridx + 1",
				    "table <= (TR(TD(('Iterate'),align='right') + TD(enum_submit_mod('iterate_%ENTITY_NAME%', ctx.iterables, ctx.iterables[int(_iterate_%ENTITY_NAME%)]))+TD(),bgcolor=clr(%BGCOLOR_BASE%,clridx)))",
				    "if _iterate_%ENTITY_NAME% <> '0':",
                    "   clridx = clridx + 1",
				    "   if 'iterate_%ENTITY_NAME%_' + ctx.iterables_corr[int(_iterate_%ENTITY_NAME%)] in REQUEST:",
                    "      old_value = REQUEST['iterate_%ENTITY_NAME%_' + ctx.iterables_corr[int(_iterate_%ENTITY_NAME%)]]",
                    "   else:",
                    "      old_value = ctx.iterables_getter[int(_iterate_%ENTITY_NAME%)]()",
				    "   table <= (TR(TD(('Iterate To'),align='right') + TD(INPUT(name='iterate_%ENTITY_NAME%_' + ctx.iterables_corr[int(_iterate_%ENTITY_NAME%)],value=old_value))+TD(),bgcolor=clr(%BGCOLOR_BASE%,clridx)))",
                    "   clridx = clridx + 1",
				    "   table <= (TR(TD(('#Iterations'),align='right') + TD(INPUT(name='iteration_steps',value=10))+TD(),bgcolor=clr(%BGCOLOR_BASE%,clridx)))",
				    "   iterate_object = %OBJ%",
				    "   iterate_name = ctx.iterables[int(_iterate_%ENTITY_NAME%)]",
				    "   iteration_getter = ctx.iterables_getter[int(_iterate_%ENTITY_NAME%)]",
				    "   iteration_setter = ctx.iterables_setter[int(_iterate_%ENTITY_NAME%)]",
				    "   if 'iterate_%ENTITY_NAME%_' + ctx.iterables_corr[int(_iterate_%ENTITY_NAME%)] in REQUEST:",
				    "      iterate_to = float(REQUEST['iterate_%ENTITY_NAME%_' + ctx.iterables_corr[int(_iterate_%ENTITY_NAME%)]])"
                    );
        }
	}


}}}
