#pragma once 

namespace premia {
namespace pygen  {
namespace python {

	inline void printAssetModel(Formatter & out, Model const * m)
	{
		out("MODEL_NAME", m->name) << "%MODEL_NAME%,";
	}

	inline void printAsset (Formatter & out, Asset const & a)
	{
		out("ASSET_NAME", a.source->name) << (seq, 
			"class %ASSET_NAME%(object):", +(seq, 
				"@staticmethod",
				"def models(): ", +(seq, 
					"return [", +foreach_x(a.models, printAssetModel), "]"), ""
			));
	}

	inline void printAssetName(Formatter & out, Asset const & a)
	{
		out("ASSET_NAME", a.source->name) << "%ASSET_NAME%, ";
	}

	/// generates correspondence between Premia assets and models
	PyCtx const & operator << (PyCtx const & ctx, Assets const & a) 
	{
		Formatter f(ctx.assetsPy());
		f << (seq, 
			"from models import *", "",
			foreach_x(a.assets, printAsset),
			"def all(): return [", +foreach_x(a.assets, printAssetName), "]" 
		);

		return ctx;
	}

}}}
