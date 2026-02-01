#pragma once

#include <string>
#include <expected>

namespace prepy {

    struct error {
        std::string_view message;
        std::string_view location;
    };

	std::expected<std::string, error> preprocess_indentation(const std::string& src);

	std::string fixup_newlines(const std::string& input, bool fix_commas = true);

}
