#include <istream>
#include <boost/filesystem.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/lambda/lambda.hpp>


namespace fs = boost::filesystem;
typedef fs::path  path_t;

#include <premia/generator/utils/symbols.h>
#include <premia/generator/utils/formatter.h>
#include <premia/generator/utils/formatter_dsl.h>
#include <premia/generator/karrigell/ctx.h>

typedef boost::format fmt;
namespace lm = boost::lambda;
using namespace boost::adaptors;
#include <premia/import.h>

const std::string premia_lib_name = "premia";

typedef std::logic_error premia_exception;
 
#include <premia/generator/api/all.h>
#include <premia/generator/karrigell/var.h>

#include <premia/generator/karrigell/enum.h>
#include <premia/generator/karrigell/model.h>
#include <premia/generator/karrigell/option.h>
#include <premia/generator/karrigell/pricing.h>

#include <boost/program_options.hpp>
#include <boost/foreach.hpp>

using premia::pygen::formatter_dsl::Formatter;

namespace po = boost::program_options;

int main(int argc, char *argv[])
{
    using namespace premia::pygen;
    
    path_t current_path = fs::current_path();
    path_t root         = current_path.parent_path().parent_path();
    path_t python_dir;
    path_t data_dir;
    path_t template_dir;
    path_t output_path;
    
    int verbosity;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("output-dir", po::value(&output_path)->default_value(root / "bindings" / "karrigell"), "output directory")
        ("pypremia-dir",po::value(&python_dir)->default_value(root / "bindings" / "python"), "directory containing premia python bindings")
        ("template-dir", po::value(&template_dir)->default_value(current_path / "templates" / "karrigell"), "directory containing templates for ksgen")
        ("data-dir",   po::value(&data_dir)->default_value(root / "data"), "directory containing premia data files")
        ("verbosity,v", po::value(&verbosity)->default_value(1), "verbosity level (0 - no output, 1 - basic, 2 - detailed)")
    ;
        
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);   
    
	karrigell::Ctx ctx(output_path, verbosity);
    
    ctx.out(1)
            << "premia-root: "  << root         << std::endl
            << "output-dir: "   << output_path  << std::endl
            << "python-dir: "   << python_dir   << std::endl
            << "data-dir: "     << data_dir     << std::endl
            << "template-dir: " << template_dir << std::endl;

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }    

    fs::create_directories(output_path / "premia");

	InitVar();

	strcpy(premia_data_dir, data_dir.native().c_str());
	
    ctx.out(1) << "Initializing...";

	PremiaCtx p;

    ctx.out(1) << "ok!" << std::endl;
    
	ctx << p.models << p.families << p.pricings << p.enums << p.assets;
	
	{
	   Formatter f(output_path / "import.py");
	   f("PYTHON_DIR", python_dir.native())
	        .process_file(template_dir / "import.py.template");
	}
   
	{
	   Formatter f(output_path / "all.py");
	   f.process_file(template_dir / "all.py.template");
	}
	
	{
	   Formatter f(output_path / "validate.py");
	   f.process_file(template_dir / "validate.py.template");
	}
    return 0;
}
