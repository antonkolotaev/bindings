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

      out << "from HTMLTags import *";
      
      out("ENUM_NAME", e.label)
	    << "def process(label, vlabel, v):";
/*
      out << (seq,
            "   if vlabel not in v.ctx.REQUEST:",
            "      v.ctx.REQUEST[vlabel] = '0'");

	    
	    
	    out << "v.clrinc()";
*/	    
	    out.incindent();
	    out << (seq, "labels = [", +foreach_x(e.members, printEnumChoices), "]");  
/*	    
	    out << (seq,
            "if v.ctx.history_mode:",
            "   e = labels.index(v.member._labels[v.member._value.key()])",
            "else:",
            "   e = int(v.ctx.REQUEST[vlabel])");
*/	    
	    bool has_params = false;
	    
	    BOOST_FOREACH(api::Enum::Members::const_reference p, e.members)
	    {
	        if (!p.second.params.empty())
	            has_params = true;
	    }
	    
	    out("CHANGE", has_params ? "True" : "False") 
	         << "e = v.setLabels(label, vlabel, labels,%CHANGE%)";
/*	    
	    out("CHANGE", has_params ? ", onchange='submit();'": "") 
	       << "L = SELECT(name = vlabel%CHANGE%).from_list(labels)";
      
       out << "L.select(value=e)";
       out << "if v.ctx.history_mode:";
       out << "   v.table <= TR(TD(label,align='right') + TD(labels[e])+TD(),bgcolor=v.currentColor)";
       out << "else:";
       out << "   v.table <= TR(TD(label,align='right') + TD(L)+TD(),bgcolor=v.currentColor)";
*/			
		 out.decindent();


       out 
            << (seq,
               "   try:",
               + +call(boost::bind(print::enumChoices, _1, boost::cref(e))),
               "   except Exception, ex:",
               "      v.addError('Error in '+label+':' + str(ex))"
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
