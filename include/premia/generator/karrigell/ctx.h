#pragma once

#include <boost/filesystem.hpp>
#include <premia/generator/api/all.h>
#include <premia/generator/utils/null_stream.h>

namespace premia {
namespace pygen {
namespace karrigell {

	struct Ctx
	{
		Ctx(path_t const &output_dir, int verbosity) 
		    :   output_dir_(output_dir) 
		    ,   base_path_(output_dir / "premia") 
		    ,   verbosity_(verbosity)
		{}
		
		std::ostream& out(int v) const 
		{
	   	   static null_stream nullstr;

		   if (verbosity_< v) return nullstr;
		   else               return std::cout;
		}

		/// returns path to the directory where enumerations will be generated
		fs::path enumDir() const { return base_path_ / "enum"; } 

		/// \param e a reference to a enum type
		/// returns path to a file where the enumeration should be generated
		fs::path filename(Enum const & e) const { 	return enumDir() / py(e.label);	}

		/// creates a directory
		/// \param p path to directory
		void create(fs::path const & p) const 	{	fs::create_directory(p);	}

		/// adds .py extension to a string
		static std::string py(std::string const & p) { return p + ".py"; }

		/// returns a path to the directory where models and pricings will be generated
		fs::path mod() const {	return base_path_ / "mod"; 	}

		/// returns a path to the directory where the model and its pricing methods will be generated
		fs::path dir(Model const & m) const {	return mod() / m.ID();		}

		/// returns path to a file where the model should be generated
		fs::path filename(Model const & m) const { 	return dir(m) / "model.py"; }

		/// returns path to directory holding all options
		fs::path opt() const { return base_path_ / "opt"; }

		/// returns path to a directory holding options of the given option family 
		fs::path dir(Family const & f) const { return opt() / f.name; }

		fs::path optionsFile(Family const & f) const { return dir(f) / py("options"); }

		/// returns path to a file holding code for the given option
		fs::path filename(Option const & opt) const { return dir(opt.family) / py(opt.name); }

		/// returns a label for the pricing
		static std::string name(Pricing const & p) { return p.model->ID() + "_" + p.family->name; }

		/// returns path to a directory where methods of the pricing will be generated
		fs::path dir(Pricing const & p) const {	return dir(*p.model) / name(p);	}

		/// returns path to a file where a description of the family will be generated
		fs::path filename(Pricing const & p) const	{ return dir(p) / py(name(p)); }

		/// returns path to a file where the pricing method will be generated
		fs::path filename(PricingMethod const & m) const { return dir(m.pricing) / py(m.name); }

		fs::path methodsFile(Option const & o) const { return dir(o.family) / py(o.name + "_methods"); }

		fs::path methodsFile(Pricing const & p) const { return dir(p) / py("methods"); }

	private:   
	    fs::path    output_dir_;
		fs::path	base_path_;
		int         verbosity_;
	};

}}}
