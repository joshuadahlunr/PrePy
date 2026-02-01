# PrePy

PrePy is a C++ library and standalone tool designed to preprocess indentation-based syntax (similar to Python) into a brace-based syntax. This is particularly useful as a first stage for parsers that prefer explicit block delimiters.

## Features

- **Indentation to Braces**: Automatically converts Python-style indentation into explicit `{}` blocks.
- **Statement Termination**: Appends semicolons `;` to the end of statements.
- **Mixed Indentation Detection**: Detects and reports errors when tabs and spaces are mixed for indentation.
- **Comment Handling**: Supports both `#` and `//` style comments.
- **Newline Refinement**: Includes a fixup utility to clean up the resulting brace placement for better readability.

## Building

PrePy uses CMake as its build system and requires a C++23 compatible compiler.

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

### Library

You can include `pre.hpp` in your project and link against the `prepy` library.

```cpp
#include "pre.hpp"
#include <iostream>

int main() {
    std::string code = "def hello():\n    print('world')";
    auto preprocessed = prepy::preprocess_indentation(code);

    if (preprocessed) {
        // By default, fixup_newlines preserves colons but cleans up semicolons before braces
        std::cout << prepy::fixup_newlines(*preprocessed) << std::endl;
    } else {
        std::cerr << "Error: " << preprocessed.error().message << std::endl;
    }
    return 0;
}
```

### Standalone Tool

The project also builds a standalone executable named `prepy`.

```bash
./prepy <input_file> [output_file]
```

#### Options:
- `-c, --fix-commas`: Refine colon-semicolon-brace sequences (removes colons before braces).
- Use `cin` as input or `cout` as output for standard I/O.

## Example

**Input:**
```python
def greet(name):
    if name:
        print("Hello, " + name)
    else:
        print("Hello, guest")
```

**Output (after fixup, default):**
```python
def greet(name):
{   if name:
{       print("Hello, " + name);
}   else:
{       print("Hello, guest");}}
```

## License

This project is open-source. See the individual files for details.
