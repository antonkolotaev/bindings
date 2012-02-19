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
	    << "def process_%ENUM_NAME%(ctx, table, colors, label, pmem, vlabel):";

      out << (seq,
            "   if vlabel not in REQUEST:",
            "      REQUEST[vlabel] = '0'",
            "   e = int(REQUEST[vlabel])");

	    
	    out.incindent();
	    
	    out << "clrinc()";
	    
	    out << (seq, "labels = [", +foreach_x(e.members, printEnumChoices), "]");  
	    
	    bool has_params = false;
	    
	    BOOST_FOREACH(api::Enum::Members::const_reference p, e.members)
	    {
	        if (!p.second.params.empty())
	            has_params = true;
	    }
	    
	    out("CHANGE", has_params ? ", onchange='submit();'": "") 
	       << "L = SELECT(name = vlabel%CHANGE%).from_list(labels)";
	    out("BGCOLOR_BASE", "colors");
      
       out << "L.select(value=e)";
       out << "if history_mode:";
       out << "   table <= TR(TD(label,align='right') + TD(labels[e])+TD(),bgcolor=clr(%BGCOLOR_BASE%,clridx))";
       out << "else:";
       out << "   table <= TR(TD(label,align='right') + TD(L)+TD(),bgcolor=clr(%BGCOLOR_BASE%,clridx))";
				
		 out.decindent();


       out 
            << (seq,
               "   try:",
               + +call(boost::bind(print::enumChoices, _1, boost::cref(e))),
               "   except Exception, ex:",
               "      add_error('Error in '+property_name+':' + str(ex))"
      );

	}

	inline Ctx& operator << (Ctx & ctx, Enums const & enums)
	{
		ctx.create(ctx.enumDir());
		
		BOOST_FOREACH(Enums::EnumsToLabels::const_reference r, enums.enums())
		{
		    generateEnum(ctx, r.second);
		}

		return ctx;
	}

}}}
