#pragma once

#include <premia/generator/utils/formatter_dsl.h>
#include <premia/generator/api/enum.h>
#include <premia/generator/karrigell/ctx.h>
#include <premia/generator/karrigell/var.h>

namespace premia {
namespace pygen {
namespace karrigell {
	
	namespace print 
	{
		/// prints import clause in the pricings file for a model
		void enumOption(Formatter & out, Enum::Members::const_reference p)
		{
			out("LABEL", p.second.label) << "print '<option value=\\\"%LABEL%\\\"' + sel(\"%LABEL%\") + '>%LABEL%'";
		}
	}
	
		inline void printEnumChoices(Formatter &out, api::Enum::Members::const_reference p)
		{
		    out("LABEL", p.second.quoted_original_label) <<  "'%LABEL%',";    
		}
	
	/// generates a wrapper for a model and a file with enumeration of all pricings for the model
	inline void generateEnum (Ctx const & ctx, Enum const & e)
	{
		Formatter out(ctx.filename(e));
		out("ENUM_NAME", e.label) 
		   << (seq,
            "def load_%ENUM_NAME%(property_name, src_prefix, dst):",
            "   try:",
            "      pname = '_' + property_name",
            "      src = src_prefix + pname",
            "      pmem = getattr(dst, pname)",
            "",
            "      if src in REQUEST:",
            "         e = int(REQUEST[src])",
		      + + +call(boost::bind(print::enumChoices, _1, boost::cref(e))),
		      "   except Exception, ex:",
            "      add_error('Error in '+property_name+':' + str(ex))"
);
            out << "";

            out
			    << "def process_%ENUM_NAME%(ctx, table, colors, label, pmem, src_prefix):";
			    
			    out.incindent();
			    
			    out << "clrinc()";
			    
			    out << (seq, "labels = [", +foreach_x(e.members, printEnumChoices), "]");  
			    
			    bool has_params = false;
			    
			    int idx = 0;
			    BOOST_FOREACH(api::Enum::Members::const_reference p, e.members)
			    {
			        out("IDX", idx++)("KEY",p.first) << "if pmem._value.key() == %KEY%: list_idx = %IDX%";
			        if (!p.second.params.empty())
			            has_params = true;
			    }
			    
			    out("CHANGE", has_params ? ", onchange='submit();'": "") 
			       << "L = SELECT(name = src_prefix%CHANGE%).from_list(labels)";
			    out("BGCOLOR_BASE", "colors");
			    
             out << "L.select(value=list_idx)";
             out << "table <= TR(TD(label,align='right') + TD(L)+TD(),bgcolor=clr(%BGCOLOR_BASE%,clridx))";
				
				
				BOOST_FOREACH(api::Enum::Members::const_reference p, e.members)
            {
               out("KEY",p.first) << "if pmem._value.key() == %KEY%: #%ENUM_NAME%";
               out << "   pass";

               out.push_scope();				  
                 
               out("CHOICE",p.second.label)
                  ("OBJ", "pmem.get_%CHOICE%()") 
                  ("PREFIX", "src_prefix+'_get_%CHOICE%_'")
                    << +foreach_x(p.second.params, print::TableEx);
                    
               out << "";
               out.pop_scope();
				 }
				 out.decindent();

	}

	inline Ctx& operator << (Ctx & ctx, Enums const & enums)
	{
		ctx.create(ctx.enumDir());
		
		BOOST_FOREACH(Enums::EnumsToLabels::const_reference r, enums.enums())
		{
		    generateEnum(ctx, r.second);
		}

//		boost::for_each(enums.enums() | map_values, boost::bind(generateEnum, boost::ref(ctx), _1));

		return ctx;
	}

}}}
