#pragma once

#include <premia/generator/api/model.h>
#include <premia/generator/karrigell/var.h>

namespace premia {
namespace pygen {
namespace karrigell {


	inline void generateModel(Ctx & ctx, Model const & m)
	{
		ctx.create(ctx.dir(m));

		Formatter f(ctx.filename(m));
		f	("MODEL_NAME", m.name)
			("MODEL_LABEL", (*m.source)->Name)
			("OBJ", "model")
			("BGCOLOR_BASE","mod_colors")
			<< (seq, 
				"%OBJ% = models.%MODEL_NAME%()", "",
				foreach(m.members, print::Ini),
				"table <= (TR(TD((B('Model:')),align='right') + TD(enum_submit('model', model.assetType().models(), '%MODEL_LABEL%'))+TD(),bgcolor=clr(%BGCOLOR_BASE%,clridx)))",
				foreach(m.members, print::Table)
			);
	}

	inline void printPricingForModel(Formatter & out, Pricing const * p)
	{
		out("FAMILY_NAME",p->family->name) << "+ TD(A('%FAMILY_NAME%', href='/premia/mod/%MODEL_ID%/%MODEL_ID%_%FAMILY_NAME%/methods')) \\";
	}

	inline void printPricingsForModel(Formatter & out, Model const * m)
	{
		out("MODEL_NAME", m->name)("MODEL_ID", m->ID()) 
			<< (seq, "table <= TR(TD('%MODEL_NAME%:') \\", +foreach(m->pricings, printPricingForModel), ")");
	}


	inline Ctx& operator << (Ctx & ctx, Models const & m)
	{
		ctx.create(ctx.mod());

		for_each(m.models, boost::bind(generateModel, boost::ref(ctx), _1));

		return ctx;
	}

	inline void printModelsForAsset(Formatter & out, Asset const & a)
	{
		out("ASSET", a.source->name) << (seq, 
			"table <= TR(TD(H4('%ASSET%')))", 
			foreach(a.models, printPricingsForModel)
			);
	}

	inline Ctx& operator << (Ctx & ctx, Assets const & assets)
	{
		Formatter f(ctx.mod() / "all.py");
		f	<< (seq, 
				"from HTMLTags import *", 
				"print H3('Available models')",
				"table = TABLE()", 
				foreach(assets.assets, printModelsForAsset), 
				"print table");

		return ctx;
	}

}}}
