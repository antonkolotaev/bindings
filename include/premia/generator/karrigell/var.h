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
	    inline void Ini(Formatter & out, NamedVar const & vr);
	    
	    inline void enumParams(Formatter &out, NamedVar const &vr)
	    {
	        Ini(out("OBJ", "%OBJ%._%VAR_NAME%.get_%LABEL%()"), vr);
	    }
	    
	    inline void enumChoices(Formatter &out, api::Enum const &e)
	    {
	        int idx = 0;
	        BOOST_FOREACH(api::Enum::Members::const_reference em, e.members)
	        {
	            out("IDX",idx)("LABEL", em.second.label) 
	                << (seq, 
	                "if e == %IDX%:", +(seq, 
	                    "%OBJ%._%VAR_NAME%.set_%LABEL%()",
	                    foreach_x(em.second.params, enumParams)));
	            ++idx;
	        }
	    } 
		struct param_loader : var_visitor<param_loader>
		{
			param_loader(Formatter & out) : out(out) {}

			/// assert for INT based types
			void operator () (Numeric<int> const & i) 
			{
				out << "if '_%VAR_ID%' in dir(): %OBJ%.%VAR_NAME% = int(_%VAR_ID%)";
			}

			/// assert for LONG based types
			void operator () (Numeric<long> const & i) 
			{
				out << "if '_%VAR_ID%' in dir(): %OBJ%.%VAR_NAME% = long(_%VAR_ID%)";
			}

			/// assert for DOUBLE based types
			void operator () (Numeric<double> const & i) 
			{
				out << "if '_%VAR_ID%' in dir(): %OBJ%.%VAR_NAME% = float(_%VAR_ID%)";
			}

			/// assert for FILENAME
			void operator () (std::string const & i)  
			{
				out << "if '_%VAR_ID%' in dir(): %OBJ%.%VAR_NAME% = _%VAR_ID%";
			}

			/// assert for PNLVECT and PNLVECTCOMPACT
			void operator () (std::vector<double> const & i) 
			{
				out << (seq, 
					//"if 'N_Dim' not in dir(): N_Dim = len(%OBJ%._%VAR_NAME%)",
					"for i in range(len(%OBJ%._%VAR_NAME%)):", 
					//"   #extlist(%OBJ%._%VAR_NAME%, N_Dim)",
					"   if '_%VAR_ID%[' + str(i) + ']' in dir(): ",
					"      %OBJ%._%VAR_NAME%[i] = float(REQUEST['%VAR_ID%[' + str(i) + ']'])"); 
			}
			
			/// assert for ENUM
			void operator() (EnumValue const & e) 
			{
				out << (seq, 
				       "if '_%VAR_ID%' in dir():", +(seq, 
				            "e = int(_%VAR_ID%)",
				            call(boost::bind(print::enumChoices, _1, boost::cref(*e.type)))
				            ));
			}
		private:
			Formatter & out;

		};

		inline void Ini(Formatter & out, NamedVar const & vr)
		{
			param_loader loader(out("VAR_NAME", vr.name)("VAR_ID", symbol_utils::replaceNonAlnumCharacters(out.lookupVar("OBJ").c_str()) + "_" + vr.name)); 
			
			out << "";
			out << "try:";
			out.incindent();
			
			loader.apply(vr.value);
			
			if (vr.name == "Model_Size")
			{
				out << "N_Dim = model._Model_Size";
			} 

            out.decindent();
            
            out << "except Exception, ex:";

			out.incindent();
			
			out << "add_error('Error in %VAR_NAME%:' + str(ex))";
			
            out.decindent();
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

		inline void Table(Formatter & out, NamedVar const & vr);

		struct tbl_val_printer : var_visitor<tbl_val_printer>
		{
			tbl_val_printer(Formatter & out) : out(out) {}
			
			static std::string row(std::string const &s)
			{
			    return "table <= TR(TD('%FRIENDLY_NAME%',align='right') + " + s + ",bgcolor=clr(%BGCOLOR_BASE%,clridx))";
			}


			template <class Scalar>
				void operator () (Numeric<Scalar> const & i) 
			{
				out("CONSTR",tostr(i.constraint))
				   ("SYMB", symbol<Scalar>())	
					<< row("TD(INPUT(name='%VAR_ID%'%SUBMIT%,value=%OBJ%._%VAR_NAME%)) + TD('%SYMB% %CONSTR%')"); 
			}

			/// assert for FILENAME
			void operator () (std::string const & i)  
			{
				out << row("TD(INPUT(name='%VAR_ID%'%SUBMIT%,value=%OBJ%._%VAR_NAME%))+TD()");
			}

			/// assert for PNLVECT and PNLVECTCOMPACT
			void operator () (std::vector<double> const & i) 
			{
			    out << "table <= TR(TD('%FRIENDLY_NAME%', align='right',rowspan=len(%OBJ%._%VAR_NAME%)) + TD(INPUT(name='%VAR_ID%[0]',value=%OBJ%._%VAR_NAME%[0])) + TD('R',rowspan=len(%OBJ%._%VAR_NAME%)),bgcolor=clr(%BGCOLOR_BASE%,clridx))";
			    
				out << "table <= Sum([TR(TD(INPUT(name='%VAR_ID%[' + str(i) + ']',value=%OBJ%._%VAR_NAME%[i])),bgcolor=clr(%BGCOLOR_BASE%,clridx)) for i in range(1,len(%OBJ%._%VAR_NAME%))])";
			}
			
			static void printEnumChoices(Formatter &out, api::Enum::Members::const_reference p)
			{
			    out("LABEL", p.second.quoted_original_label) <<  "'%LABEL%',";    
			}

			/// assert for ENUM
			void operator() (EnumValue const & e) 
			{
			    out << (seq, "labels = [", +foreach_x(e.type->members, printEnumChoices), "]");  
			    
			    bool has_params = false;
			    
			    int idx = 0;
			    BOOST_FOREACH(api::Enum::Members::const_reference p, e.type->members)
			    {
			        out("IDX", idx++)("KEY",p.first) << "if %OBJ%._%VAR_NAME%._value.key() == %KEY%: list_idx = %IDX%";
			        if (!p.second.params.empty())
			            has_params = true;
			    }
			    
			    out("CHANGE", has_params ? ", onchange='submit();'": "") 
			       << "L = SELECT(name = '%VAR_ID%'%CHANGE%).from_list(labels)";
                out << "L.select(value=list_idx)";
				out << row("TD(L)+TD()");
				
				BOOST_FOREACH(api::Enum::Members::const_reference p, e.type->members)
				    BOOST_FOREACH(NamedVar const &v, p.second.params)
				    {
			            out("KEY",p.first) << "if %OBJ%._%VAR_NAME%._value.key() == %KEY%:";
			            
			            out.push_scope();				  
			              
				        out("CHOICE",p.second.label)
				           ("OBJ", "%OBJ%._%VAR_NAME%.get_%CHOICE%()") 
				              << +call(boost::bind(Table,_1, v));
				        out << "";
				        out.pop_scope();
				    }
			}
		private:
			Formatter & out;

		};
		
		inline void TableVal(Formatter & out, NamedVar const & vr)
		{
		    if (vr.has_setter) std::cout << vr.name << std::endl;
		    std::string submit = vr.has_setter ? ", onchange='submit();'": "";
		    out << "clridx = clridx + 1";
			tbl_val_printer(out("SUBMIT", submit)).apply(vr.value);
		}

		inline void Table(Formatter & out, NamedVar const & vr)
		{
		    std::string star;
		    if (vr.iterable) star = "*";
			out("VAR_NAME", vr.name)
			   ("VAR_ID", symbol_utils::replaceNonAlnumCharacters(out.lookupVar("OBJ").c_str()) + "_" + vr.name)
               ("FRIENDLY_NAME", star + vr.src->Vname)
			   << (seq, call(boost::bind(TableVal, _1, vr)));
		}
        inline void IterableName(Formatter &out, NamedVar const &vr)
        {
            if (vr.iterable)
                out("VNAME",vr.src->Vname) << "'%VNAME%',";
        }
        inline void IterableNameCorr(Formatter &out, NamedVar const &vr)
        {
            if (vr.iterable)
                out("VNAME",vr.name) << "'%VNAME%',";
        }

        inline void Iterables(Formatter& out, VarList const &vars)
        {
            out << (seq, "iterables = [",
                    "   'No iteration',",
                    +foreach_x(vars,IterableName),
                    "]",
                    "v_iterables = [",
                    "   '',",
                    +foreach_x(vars,IterableNameCorr),
                    "]",
                    "clridx = clridx + 1",
				    "table <= (TR(TD(('Iterate'),align='right') + TD(enum_submit_mod('iterate_%ENTITY_NAME%', iterables, iterables[int(_iterate_%ENTITY_NAME%)]))+TD(),bgcolor=clr(%BGCOLOR_BASE%,clridx)))",
				    "if _iterate_%ENTITY_NAME% <> '0':",
                    "   clridx = clridx + 1",
				    "   table <= (TR(TD(('Iterate To'),align='right') + TD(INPUT(name='iterate_to',value=getattr(%OBJ%, v_iterables[int(_iterate_%ENTITY_NAME%)])))+TD(),bgcolor=clr(%BGCOLOR_BASE%,clridx)))",
                    "   clridx = clridx + 1",
				    "   table <= (TR(TD(('#Iterations'),align='right') + TD(INPUT(name='iteration_steps',value=10))+TD(),bgcolor=clr(%BGCOLOR_BASE%,clridx)))",
				    "   iterate_object = %OBJ%",
				    "   iterate_name = v_iterables[int(_iterate_%ENTITY_NAME%)]"
                    );
        }
	}


}}}
