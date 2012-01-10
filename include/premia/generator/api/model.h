#pragma once

#include <string>
#include <map>
#include <boost/noncopyable.hpp>

#include <premia/generator/api/asset.h>
#include <premia/generator/api/var_conversion.h>

namespace premia {
namespace pygen  {
namespace api	 {

	struct Pricing; 

	/// \brief A wrapper over a Premia model
	struct Model : boost::noncopyable
	{
		/// \brief constructs a model wrapper
		/// \param source pointer to native model
		/// \param asset asset which the model belongs to 
		/// \param name corrected name of the model
		template <class Ctx>
		Model(Ctx & ctx, ::Model ** source, Asset & asset, std::string const & name)
			:	name(name), asset(asset), source(source)
		{
			// initialize the native model
			(*(*source)->Init)(*source);

			// and get its variables
			VAR * vars = reinterpret_cast<VAR*>((*source)->TypeModel);

			getVars(ctx, vars, (*source)->nvar, members);

			asset.models.insert(this);
		}

		/// corrected model name (e.g. BlackScholes1dim)
		std::string const	name;
		/// model ID (e.g. BS1D)
		std::string const   ID() const { return (*source)->ID; };
		/// linearized model variables
		VarList				members;
		/// pointer to native model
		::Model       **    source;
		/// reference to the parent asset
		Asset const	   &    asset;
		/// a list of pricings for the model
		std::list<Pricing*>	pricings;
	};

	/// \brief ordinal number of the model
	inline int id(Model const & m)
	{
		return m.source - m.asset.source->models;
	}

	/// \brief all model wrappers of Premia
	struct Models : boost::noncopyable
	{
		/// \brief list of the models
		boost::ptr_list<Model>	models;

		/// \brief collects all Premia models
		template <class Ctx>
		Models(Ctx & ctx)
		{
			// used names for models
			UsedIds		used_names;
			// used to ensure model uniqueness
			std::set< ::Model*> processed_models;

			for (PremiaAsset * asset = premia_assets; asset->name; ++asset)
			{
				Asset &a = ctx.assets.lookup(asset);

				for (::Model ** m = asset->models; *m; ++m)
				{
					::Model * model = *m;

					if (processed_models.find(model) == processed_models.end())
					{
						models.push_back(ids[model->ID] = new Model(ctx, m, a, 
							correctIdentifierName(model->Name, used_names)));

						processed_models.insert(model);
					}
				}
			}
		}

		/// \brief looks for a model by its ID (e.g. BS1D)
		Model * lookup(std::string const & ID) const 
		{
			std::map<std::string, Model*>::const_iterator it = ids.find(ID);

			if (it == ids.end())
				throw std::string("cannot find a model with name ") + ID;

			return it->second;
		}

	private:
		/// \brief mapping from model IDs to models
		std::map<std::string, Model*>	ids;
	};
}

using api::Model;
using api::Models;
using api::id;

}}
