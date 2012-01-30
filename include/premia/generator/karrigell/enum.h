#pragma once

#include <premia/generator/utils/formatter_dsl.h>
#include <premia/generator/api/enum.h>
#include <premia/generator/karrigell/ctx.h>

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

	/// generates a wrapper for a model and a file with enumeration of all pricings for the model
	inline void generateEnum (Ctx const & ctx, Enum const & e)
	{
		Formatter f(ctx.filename(e));
		f << foreach_x(e.members, print::enumOption);
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
