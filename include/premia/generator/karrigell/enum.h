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
	    bool has_params = false;
	    
	    BOOST_FOREACH(api::Enum::Members::const_reference p, e.members)
	    {
	        if (!p.second.params.empty())
	            has_params = true;
	    }

		Formatter(ctx.filename(e))
		   ("CHANGE", has_params ? "True" : "False")
		   << (seq, 
		      "def process(label, vlabel, v):", +(seq,
		         "labels = [", 
		            +foreach_x(e.members, printEnumChoices), 
		         "]",
		         "e = v.setLabels(label, vlabel, labels,%CHANGE%)",
               "try:",
                  +call(boost::bind(print::enumChoices, _1, boost::cref(e))),
               "except Exception, ex:",
               "   v.addError('Error in '+label+':' + str(ex))"
		   ));
/*
      out("ENUM_NAME", e.label)
	    << "def process(label, vlabel, v):";
	    out.incindent();
	    out << (seq, "labels = [", +foreach_x(e.members, printEnumChoices), "]");  
	    
	    out("CHANGE", has_params ? "True" : "False") 
	         << "e = v.setLabels(label, vlabel, labels,%CHANGE%)";
		 out.decindent();


       out 
            << (seq,
               "   try:",
               + +call(boost::bind(print::enumChoices, _1, boost::cref(e))),
               "   except Exception, ex:",
               "      v.addError('Error in '+label+':' + str(ex))"
      );
*/
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
