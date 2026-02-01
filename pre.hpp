/**
 * @file pre.hpp
 * @brief Header file for the PrePy indentation preprocessor.
 */

#pragma once

#include <string>
#include <expected>

/**
 * @namespace prepy
 * @brief Contains tools for preprocessing Python-like indentation into brace-based syntax.
 */
namespace prepy {

    /**
     * @struct error
     * @brief Represents a preprocessing error.
     */
    struct error {
        std::string_view message;  /**< The error message describing what went wrong. */
        std::string_view location; /**< The location in the source string where the error occurred. */
    };

	/**
	 * @brief Preprocesses a source string by converting indentation into brace-based blocks.
	 *
	 * This function takes a string with indentation-based structure (like Python) and
	 * converts it into a string with explicit braces `{}` for blocks and semicolons `;`
	 * at the end of statements. It also detects mixed tabs and spaces and unaligned dedents.
	 *
	 * @param src The source string to preprocess.
	 * @return std::expected<std::string, error> The preprocessed string on success, or an error struct on failure.
	 */
	std::expected<std::string, error> preprocess_indentation(const std::string& src);

	/**
	 * @brief Refines the output of the indentation preprocessor for better formatting.
	 *
	 * This function cleans up the semicolons and braces to make the output more readable
	 * and compatible with standard brace-based parsers. It specifically handles the
	 * conversion of certain character sequences into newlines and braces.
	 *
	 * @param input The preprocessed string to fix up.
	 * @param fix_commas If true, it replaces ":; {" with "\n{". If false, it replaces "; {" with "\n{".
	 * @return std::string The refined string.
	 */
	std::string fixup_newlines(const std::string& input, bool fix_commas = true);

}
