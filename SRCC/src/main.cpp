#include <iostream>
#include <string>

#include "main.hpp"
#include "cxxopts.hpp"
#include "parameters.hpp"
#include "RSVSclass.hpp"
#include "RSVSintegration.hpp"

#include "makeontargetchange.h"

#ifdef RSVSTEST
#include "test.hpp"
#endif // RSVSTEST

using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#ifndef LIB_RSVS
int main(int argc, char* argv[])
#else
int main_rsvs3d(int argc, char* argv[])
#endif
{
	#ifndef RSVSTEST
	return RSVSExecution(argc, argv);
	#elif defined(TEST_ALL)
	rsvstest::maintest();
	#else
	rsvstest::newtest();
	#endif //RSVSTEST
}
#pragma GCC diagnostic pop

int RSVSExecution(int argc, char* argv[]){

	param::parameters paramconf;

	auto parseOut = parse::CommandLineParser(argc, argv, paramconf);
	if (parseOut.execFlow>0){
		integrate::RSVSclass RSVSobj;
		RSVSobj.paramconf = paramconf;

		integrate::execute::All(RSVSobj);

	} else {
		// Output parameter file to the directory (NO OVERWRITE?)
		NoExecution(parseOut, paramconf);
	}
	return (0);
}

void NoExecution(parse::ParserOutput parseOut, param::parameters &paramconf){


	if (parseOut.execFlow==-2){
		parseOut.paramFileOut="failexec_rsvsconfig.json";
	}

	param::io::writeflat(parseOut.paramFileOut+"flat", paramconf);
	param::io::write(parseOut.paramFileOut, paramconf);

	if (parseOut.execFlow==-2){
		cerr << "Error while parsing the arguments. Check generated '"
			<< parseOut.paramFileOut << "' file";
			exit(-2);
	}
}

/**
Takes in the command line arguments and returns an integer specifying the
execution flow of the rest of the program.

@param[in]  argc       The number of arguments
@param      argv       the arguments
@param      paramconf  The parameter struncture

@return     Number indicating the state of the program after parsing: 0   does
            nothing; 1   Run RSVS; -1  No exec stops it; -2  Error parsing
            inputs.
*/
parse::ParserOutput parse::CommandLineParser(int argc, char* argv[], 
	param::parameters &paramconf){

	parse::ParserOutput parseOut;
	parseOut.execFlow=0;
	std::vector<std::string> triggerExec,configParam, configFiles, configPredef,
		noexecStr;
	// options that will trigger execution of RSVS
	triggerExec = {"use-config","load-config","param"};

	std::string strDescription;
	strDescription = "\nProgram for the execution of the Restricted-Surface";
	strDescription += " Volume of Solid in 3D";
	cxxopts::Options options(argv[0], strDescription);
	options
	.positional_help("[optional args]")
	.show_positional_help();
	
	options.add_options("")
	("h,help", "Print help")
	;

	options.add_options("Parameter configuration")
	("u,use-config","Use one of the predefined configurations stored in the code.",
		cxxopts::value(configPredef), "STRING")
	("l,load-config", 
		std::string("Load configuration file in JSON format to set parameter "
		"structure. Multiple files can be specified and will be processed"
		" in order of appearance."), cxxopts::value(configFiles), "FILES")
	("p,param", "Define a parameter manually on the command line."
		" The format must be a flat key into the JSON configuration:"
		" (e.g.: '/snak/maxsteps:50' will set 'param.snak.maxsteps=50')",
		 cxxopts::value(configParam),
		"KEY:VAL")
	("default-config","Output the default configuration to a file.",
		cxxopts::value<std::string>()->implicit_value("default_config"),
		"FILE");

	options.add_options("Execution control")
	("n,noexec",std::string("Do not execute RSVS process, "
		"will only parse the inputs and output "
		"the resulting configuration file to 'arg'"),
		cxxopts::value(noexecStr)->implicit_value("./noexec_config.json"))
	("e,exec", "Execute RSVS. With no command line argument the "
		"program does nothing.")
	;

	auto result = options.parse(argc, argv);
	// ""
	if (result.count("help")){
		std::cout << options.help({"", "Execution control",
			"Parameter configuration"}) << std::endl;
		exit(0);
		parseOut.execFlow = -1;
	}
	// "Execution control"
	if(result.count("noexec")>0 && result.count("exec")>0){
		std::cerr << std::endl << " Invalid specification of -e (exec) and -n (noexec)";
		std::cerr << " on the command line." << std::endl;
		std::cerr << " see --help for more info" << std::endl;
		exit(-1);
	} else if (result.count("exec")){
		parseOut.execFlow = 1;
	} else if (result.count("noexec")){
		parseOut.execFlow = -1;
		for (auto confCase : noexecStr){
			parseOut.paramFileOut = confCase;
			break;
		}
		std::cout << parseOut.paramFileOut << std::endl;
	}

	// Parameter configuration
	// if one of the triggers is found specify that execution
	// should take place
	if (parseOut.execFlow==0){
		for (auto i : triggerExec){
			if(result.count(i)){
				parseOut.execFlow = 1;
				break;
			}
		}
	}
	// Parse a predefined config:
	if (result.count("use-config")){
		for (auto confCase : configPredef) {
			parse::config::useconfig(confCase, paramconf);
		}
	}
	if (result.count("load-config")){
		for (auto confCase : configFiles) {
			parse::config::loadconfig(confCase, paramconf);
		}
	}
	if (result.count("param")){
		if(param::io::updatefromstring(configParam, paramconf)>0){
			parseOut.execFlow = -2;
		}
	}

	return(parseOut);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void parse::config::useconfig(const std::string &confCase,
	param::parameters &paramconf){

	std::cerr << "No predefined cases yet." << std::endl;

}
#pragma GCC diagnostic pop
void parse::config::loadconfig(const std::string &confCase,
	param::parameters &paramconf){

	param::io::read(confCase, paramconf);

}

parse::ParserOutput parse::StringParser(std::vector<std::string> &commands,
	param::parameters &paramconf){

	int argc = commands.size();
	char **argv;
	argv = new char*[argc];
	for (int i = 0; i < argc; ++i)
	{
		argv[i] = new char[commands[i].length()+1];
		strcpy(argv[i], commands[i].c_str());
	}

	auto parseOut = parse::CommandLineParser(argc, argv, paramconf);

	for (int i = 0; i < argc; ++i)
	{
		delete [] argv[i];
	}
	return(parseOut);
}