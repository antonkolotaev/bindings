#include <istream>
#include <boost/filesystem.hpp>
#include <boost/lambda/lambda.hpp>


namespace fs = boost::filesystem;
typedef fs::path  path_t;

#include <premia/generator/utils/symbols.h>
#include <premia/generator/utils/formatter.h>
#include <premia/generator/utils/formatter_dsl.h>
#include <premia/generator/karrigell/ctx.h>

typedef boost::format fmt;
namespace lm = boost::lambda;
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

template <class Filter>
void copyDir(const fs::path& source, const fs::path& dest, Filter filter) {

    try {
        // Check whether the function call is valid
        if(!fs::exists(source) || !fs::is_directory(source)) {
            std::cerr << "Source directory " << source.string().c_str()
                << " does not exist or is not a directory." << std::endl;
            return;
        }
    }
    catch(fs::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
    // Iterate through the source directory
    for(fs::directory_iterator it(source); it != fs::directory_iterator(); it++) {
        try {
            fs::path current(it->path());
            if(fs::is_directory(current)) {
                // Found directory: Recursion
                copyDir(current, 
                    dest / current.filename(), filter);
            }
            else {
                if (filter(current)) 
                {
                   fs::create_directories(dest);
                   
                   // Found file: Copy
                   fs::copy_file(current,
                       fs::path(dest / current.filename()), fs::copy_option::overwrite_if_exists);
                }
            }
        }
        catch(fs::filesystem_error& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}


using premia::pygen::formatter_dsl::Formatter;

namespace po = boost::program_options;

bool tocopy(fs::path const &p)
{
   std::string e = p.extension().string();
   return
      e == ".js" ||
      e == ".pih"||
      e == ".py"
      ; 
}

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
    
    copyDir(template_dir, output_path / "premia",tocopy);

    InitVar();

    strcpy(premia_data_dir, data_dir.string().c_str());

    ctx.out(1) << "Initializing...";

    PremiaCtx p;

    ctx.out(1) << "ok!" << std::endl;

    ctx << p.models << p.families << p.pricings << p.enums << p.assets;

        {
            Formatter f(output_path / "import.py");
            f("PYTHON_DIR", python_dir.string())
                .process_file(template_dir / "import.py.template");
        }

        {
            Formatter f(output_path / "all.py");
            f.process_file(template_dir / "all.py.template");
        }

        {
            Formatter f(output_path / "index.py");
            f.process_file(template_dir / "validate.py.template");
        }

    fs::copy_file(template_dir / "graph.pih", output_path / "graph.pih", fs::copy_option::overwrite_if_exists);
    fs::copy_file(template_dir / "flotr2.min.js", output_path / "flotr2.min.js", fs::copy_option::overwrite_if_exists);

    return 0;
}
