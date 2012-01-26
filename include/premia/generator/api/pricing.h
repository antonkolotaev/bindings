#pragma once

#include <premia/generator/api/model.h>
#include <premia/generator/api/option.h>

/// \brief extract from a pricing name a model and a family names the pricing corresponds to
/// \param pricing_name a pricing name to parse
/// \param model_name [out] extracted model name
/// \param family_name [out] extracted family name
inline void parsePricingName(std::string const & pricing_name, std::string & model_name, std::string & family_name)
{
	// since some model IDs contain '_', we should look for the last one
	std::string::size_type pos = pricing_name.find_last_of('_');

	if (pos == std::string::npos)
		throw std::string("there is no '_' in pricing name: ") + pricing_name;

	model_name = pricing_name.substr(0, pos);
	family_name = pricing_name.substr(pos + 1);
}

namespace premia {
namespace pygen  {
namespace api    {

	struct PricingMethod;

	/// \brief A wrapper over a Premia pricing
	struct Pricing : boost::noncopyable
	{
		/// a pointer to a corresponding model wrapper
		Model const  * model;
		/// a pointer to a corresponding family wrapper
		Family		 * family;
		/// a pointer to the native pricing
		::Pricing **   source;
		/// a reference to the parent asset
		Asset const&   asset;

		/// a list of all methods
		boost::ptr_list<PricingMethod>	methods;

		std::map<Option const*, std::list<PricingMethod*> >	methods_for_options;

		/// collects all methods in the pricing
		/// \brief pricing a pointer to the native pricing
		/// \param asset a reference to the parent asset
		template <class Ctx>
   		     Pricing(Ctx & ctx, ::Pricing** pricing, Asset const & asset);
	};

	/// \brief gets the ordinal number of the pricing
	inline int id(Pricing const & pricing)
	{
		return pricing.source - pricing.asset.source->pricings;
	}

	/// \brief A wrapper over a Premia pricing method
	struct PricingMethod : boost::noncopyable
	{
		/// \brief initializes the method
		/// \param source a pointer to the native pricing method
		/// \pricing a reference to the parent pricing
		template <class Ctx>
		PricingMethod(Ctx & ctx, ::PricingMethod **source, Pricing & pricing)
			:	source(source), pricing(pricing), name((*source)->Name)
		{
			// initializing the native method
			(*(*source)->Init)(*source, (**pricing.family->source)[0]);		

			// getting variables
			getVars(ctx, reinterpret_cast<VAR*>((*source)->Par), MAX_PAR, members);

			// obtaining a list of options compatible with the method
			BOOST_FOREACH(Option & opt, pricing.family->options)
			{
				if ((*(*source)->CheckOpt)(*opt.source, *pricing.model->source) == OK)
				{
					compatible_options.push_back(&opt);

					opt.methods_for_models[pricing.model].insert(this);
				}
			}

			// getting method results
			getVars(ctx, reinterpret_cast<VAR*>((*source)->Res), MAX_PAR, results);
		}

		/// a pointer to the native pricing method
		::PricingMethod **  source;
		/// a reference to the parent pricing
		Pricing const &		pricing;
		/// corrected name of the method
		std::string const   name;
		/// linearized list of the method members
		VarList				members;
		/// linearized list of the method result parameters
		VarList				results;
		/// list of the options compatible with the method
		std::list<Option const*>	compatible_options;
	};

	/// \brief gets the ordinal number of the method in the pricing
	inline int id(PricingMethod const & m)
	{
		return m.source - (*m.pricing.source)->Methods;
	}

	/// \brief holds all the Premia pricings 
	struct Pricings
	{
		/// \brief collects the pricings
		template <class Ctx>
		Pricings(Ctx & ctx) 
		{
			std::set< ::Pricing*> pricings_processed;

			for (PremiaAsset * asset = premia_assets; asset->name; ++asset)
			{
				for (::Pricing ** pricing = asset->pricings; *pricing; ++pricing)
				{
					if (pricings_processed.find(*pricing) == pricings_processed.end())
					{
						Pricing * p = new Pricing(ctx, pricing, ctx.assets.lookup(asset));
						pricings.push_back(p);
						pricings_processed.insert(*pricing);
					}
				}
			}
		}

		/// list of the pricings
		boost::ptr_list<Pricing>	pricings;
	};

		template <class Ctx>
		Pricing::Pricing(Ctx & ctx, ::Pricing** pricing, Asset const & asset)
			:	source(pricing), asset(asset)
		{
			std::string model_name, family_name;

			parsePricingName((*pricing)->ID, model_name, family_name);

			// corresponding model
			Family * family = ctx.families.lookup(family_name);
			// corresponding family
			Model * model = ctx.models.lookup(model_name);

			// register the pricing in its model
			model->pricings.push_back(this);

			this->family = family;
			this->model = model;

			for (::PricingMethod **method = (*source)->Methods; *method; ++method)
			{
				PricingMethod* pm = new PricingMethod(ctx, method, *this);
				methods.push_back(pm);
				BOOST_FOREACH(Option const * opt, pm->compatible_options)
				{
					methods_for_options[opt].push_back(pm);
				}
			}
		}

}

using api::Pricing;
using api::PricingMethod;
using api::Pricings;
using api::id;

}}