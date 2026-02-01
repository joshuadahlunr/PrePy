#include "pre.hpp"

#include <sstream>
#include <string>
#include <vector>
#include <regex>

namespace prepy {
	static int next_tab_stop(int col) { return col + (8 - (col % 8)); }

	static std::string mark(std::string_view line, char bracket) {
		if (line.size() < 1)
			return std::string(1, bracket);
		auto unsafe = const_cast<char *>(line.data());
		unsafe[0] = bracket;
		return "";
	}

	std::expected<std::string, error> preprocess_indentation(const std::string &src) {
		std::istringstream in(src);
		std::ostringstream out;

		std::vector<int> indent_stack;
		indent_stack.push_back(0);

		std::string line;
		bool first = true;
		size_t total_bytes = 0;

		while (std::getline(in, line)) {
			int col = 0;
			bool saw_space = false;
			bool saw_tab = false;
			size_t i;
			struct deferred_update {
				size_t &total_bytes;
				size_t &i;

				~deferred_update() { total_bytes += i; }
			} deferred = {total_bytes, i};

			// Measure indentation
			for (i = 0; i < line.size(); ++i)
				if (line[i] == ' ') {
					col++;
					saw_space = true;
				} else if (line[i] == '\t') {
					col = next_tab_stop(col);
					saw_tab = true;
				} else break;

			if (saw_space && saw_tab)
				return std::unexpected(error{"Mixed tabs and spaces", std::string_view{src}.substr(total_bytes + i, 1)});

			std::string trimmed = line.substr(i);

			// Strip comments from the trimmed portion
			size_t comment_pos = trimmed.find('#');
			if(comment_pos == std::string::npos)
				comment_pos = trimmed.find("//");
			if (comment_pos != std::string::npos)
				trimmed = trimmed.substr(0, comment_pos);

			// Remove trailing whitespace after comment removal
			auto last_non_ws = trimmed.find_last_not_of(" \t\r\n\v\f\x85\xA0");
			if (last_non_ws != std::string::npos)
				trimmed = trimmed.substr(0, last_non_ws + 1);
			else trimmed.clear();

			if (!first)
				out << '\n';
			first = false;

			// Blank line (whitespace/comments only) → pass through unchanged
			if (trimmed.empty()) {
				out << line;
				continue;
			}

			// Apply trimming to end of line
			size_t end = line.size() - 1;
			if (last_non_ws != std::string::npos)
				end = i + last_non_ws + 1;
			else if (comment_pos != std::string::npos)
				end = i + comment_pos;
			line = line.substr(0, end);

			int prev = indent_stack.back();

			if (col > prev) {
				indent_stack.push_back(col);
				out << mark(line, '{');
				out << line;
			} else if (col < prev) {
				std::string dedent_line = line;
				std::string_view view = dedent_line;
				size_t first_non_whitespace = std::distance(dedent_line.begin(), std::find_if(dedent_line.begin(), dedent_line.end(), [](char c) { return !(c == ' ' || c == '\t'); }));
				size_t replacements = 0;

				while (indent_stack.back() > col) {
					indent_stack.pop_back();
					if (replacements < first_non_whitespace)
						out << mark(view.substr(replacements, first_non_whitespace - replacements), '}');
					else out << '}';
					++replacements;
				}
				if (indent_stack.back() != col)
					return std::unexpected(error{"Unaligned dedent level", std::string_view{src}.substr(total_bytes + i, 1)});

				out << dedent_line;
			} else	out << line;

			// Emit newlines
			auto last = line.find_last_not_of(" \t\r\n\v\f\x85\xA0");
			if (last != std::string::npos && line[last] != '\\')
				out << ';';
		}

		// Emit final DEDENTs
		while (indent_stack.size() > 1) {
			out << '}';
			indent_stack.pop_back();
		}

		return out.str();
	}

	std::string fixup_newlines(const std::string& input, bool fix_commas /*= true*/) {
		static std::regex pattern(R"(;\s*\{)"), comma_pattern(R"(:;\s*\{)");
		return std::regex_replace(input, fix_commas ? comma_pattern : pattern, "\n{");
	}
} // namespace prepy
