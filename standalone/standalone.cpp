#include "argparse/argparse.hpp"
#include "pre.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

struct args : public argparse::Args {
	std::string& input = arg("input", "Path to the input file");
	std::optional<std::string>& output = arg("output", "Path to the output file");
	bool& fix_commas = flag("c,fix-commas", "Should we fix commas?");
};

std::string strip_extension(const std::string& path) {
    std::filesystem::path p(path);
    if (p.extension() == ".py")
        p.replace_extension("");
    return p.string();
}

int main(int argc, char** argv) {
	auto args = argparse::parse<struct args>(argc, argv);
	if(!args.output)
		args.output = strip_extension(args.input);

	std::istream* in;
	if(args.input == "cin")
		in = &std::cin;
	else in = new std::ifstream(args.input);

	if(!*in) {
		std::cerr << "Input file not found" << std::endl;
		return -1;
	}

	std::string input;
	for(char c; in->get(c); )
		input += c;

	auto output_ = prepy::preprocess_indentation(input);
	if(!output_) {
		std::cerr <<  output_.error().message << std::endl;
		return -2;
	}
	auto output = prepy::fixup_newlines(*output_, args.fix_commas);

	std::ostream* out;
	if(*args.output == "cout")
		out = &std::cout;
	else out = new std::ofstream(*args.output);

	out->write(output.c_str(), output.size());
	out->flush();
}
