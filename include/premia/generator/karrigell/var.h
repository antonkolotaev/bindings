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
	    inline void TableEx(Formatter & out, NamedVar const & vr);
	    
	    inline void enumParams(Formatter &out, NamedVar const &vr)
	    {
	        TableEx(out("OBJ", "pmem.get_%LABEL%()")("PREFIX", "vlabel+'_get_%LABEL%_'"), vr);
	    }
	    
	    inline void enumChoices(Formatter &out, api::Enum const &e)
	    {
	        int idx = 0;
	        BOOST_FOREACH(api::Enum::Members::const_reference em, e.members)
	        {
	            out("IDX",idx)("LABEL", em.second.label) 
	                << (seq, 
	                "if e == %IDX%:", +(seq, 
	                    "v.setChoice(lambda x: x.set_%LABEL%())", 
	                    //"v.member.set_%LABEL%()",
	                    foreach_x(em.second.params, enumParams)));
	            ++idx;
	        }
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
				   << "from kspremia.enum import %ENUM_NAME%";
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

		template <class T> const char * converter() { return "int"; }
		template <> const char * converter<double>(){ return "float"; }
		template <> const char * converter<long>  (){ return "long"; }

		inline void TableEx(Formatter & out, NamedVar const & vr);

		struct tbl_val_printer : var_visitor<tbl_val_printer>
		{
			tbl_val_printer(Formatter & out, VAR const *src, bool iterable) : out(out), src(src), iterable(iterable) {}
			
			template <class Scalar>
				void operator () (Numeric<Scalar> const & i) 
			{
				out("CONSTR",tostr(i.constraint))
				   ("SYMB", symbol<Scalar>())
				   ("CONV", converter<Scalar>())
				   ("ITERABLE", iterable ? "True" : "False")	
				   << "v.processScalar('_%VAR_NAME%', '%FRIENDLY_NAME%', %PREFIX%+'_%VAR_NAME%', '%SYMB% %CONSTR%', %ONCHANGE%, %ITERABLE%, %CONV%)"
;
			}

			void operator () (std::string const & i)  
			{
			   out << "v.processScalar('_%VAR_NAME%', '%FRIENDLY_NAME%', %PREFIX%+'_%VAR_NAME%', '', '', False, str)";
			}

			void operator () (std::vector<double> const & i) 
			{
			   if (src && src->Vtype==PNLVECTCOMPACT)
			      out 
			      << "v.processVectorCompact('_%VAR_NAME%', '%FRIENDLY_NAME%', %PREFIX%+'_%VAR_NAME%')";
			   else
			      out 
			      << "v.processVector('_%VAR_NAME%', '%FRIENDLY_NAME%', %PREFIX%+'_%VAR_NAME%')";
			}
			
			void operator() (EnumValue const & e) 
			{
			    out("ENUM_TYPE", e.type->label) 
				   << "v.processEnum(%ENUM_TYPE%, '%FRIENDLY_NAME%', %OBJ%._%VAR_NAME%, %PREFIX%+'_%VAR_NAME%')";
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
			out("VAR_NAME", vr.name)
			   ("VAR_ID", symbol_utils::replaceNonAlnumCharacters(out.lookupVar("OBJ").c_str()) + "_" + vr.name)
               ("FRIENDLY_NAME", vr.src->Vname)
			   << (seq, call(boost::bind(TableVal, _1, vr)));
		}

		inline void Table(Formatter & out, NamedVar const & vr)
		{
		   TableEx(out("PREFIX", std::string("'")+symbol_utils::replaceNonAlnumCharacters(out.lookupVar("OBJ").c_str())+"'"), vr);
		}

	}


}}}
