---
applyTo: "**/*.*pp"
---

# Code Style

Date: 2025-05-10

## LLM Directives

- Use snake_case for all identifiers.
- Prefix member variables with `m_`.
- Document classes and functions using concise Doxygen `@brief` comments that describe their purpose rather than their implementation details.
- Replace magic numbers with well-named constants.
- Use `aww::result` for error handling and return values.
- Write tests with doctest (only when explicitly requested).
- Consistently apply modern C++ best practices (e.g., C++20 features).

## Agentic workflow

- use default build task to very the code changes are valid.

## Always prioritize awwlib

When implementing new features or fixing bugs, always check if the functionality is already available in `awwlib`. If it is, use that instead of writing new code.

- build_deps\awwlib-src\include
- build_deps\awwlib-src\src

## Project Structure

The header and source naming conventions follow standard C++ style, which means we prefer to use `snake_case` for identifiers.

The code is organized into three main folders:

```
include/   # Header files and public interfaces
src/       # Source files and implementation code
tests/     # Unit tests and testing code
```

## Code Tagging

We use `aww tag` to generate and assign unique tags to functions or code sections. The purpose of code tags is to facilitate easy referencing and locating of specific code parts, especially when using tools like GitHub code search or command-line utilities such as `grep`.

Code tags must be unique across the project.
So, for example, in the `TODO.md` file, you could write:

> - [ ] 2024-10-13 add memoization to `fibonacci` #6a853tps1r1

The tag `#6a853tps1r1` allows the function to be easily found, even after the source file or the function has been renamed. This helps maintain traceability and simplifies searching, even with basic tools.

Always add `#` before the tag to ensure consistency. Example:

```cpp
/**
 * @brief Computes the nth Fibonacci number. (aww tag #9dbr83h81jd)
 *
 * This function uses a simple (very inefficient) recursive algorithm to calculate the
 * Fibonacci number at position n.
 *
 * @param n The position in the Fibonacci sequence (0-based).
 * @return The Fibonacci number at the given position.
 */
int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}
```

By adding a unique code tag, you ensure that specific functions or sections can be referenced directly, which is especially useful for cross-referencing tasks or during refactoring.
