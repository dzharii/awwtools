# aww-tools changelog

## 2024-01-19

Tiny update to CXX 20. No issues so far

## 2023-12-10



My most used and most simple aww tool `aww-date` got some experimental update. 

I have found that tracking the input/output dependencies calls is tricky, since, for instance, `fs_file_exists()`

can be called multiple times in the code for different cases. In the test cases, I want to somehow recreate the complex code path, but 

tracking the right calls by their call order (number of times same function was called) or by the input parameters is too brittle, how can I precisely find  the call I need? at first, for a long time, my idea was to rename `fs_file_exists()` to a case specific name, like:

- if `fs_template_folder_exists()`
  - if `fs_template_file_exists()`
    - do something
  - else if `fs_template_file_with_specification_exists()`
    - do something else

but to support this case I have to: 

- add these function to `aww_create_io_dependencies_interface`
- then implement in `aww_create_io_dependencies`
- then create a mock in `aww_create_io_dependencies_stub`

This simple does not scale, so I have decided to tag these calls at compile time and keep the generic name. 

New code added:

`include\aww-common.hpp`

```cpp
// CallTag struct definition
struct call_tag_t {
constexpr explicit call_tag_t(unsigned long long value) : value(value) {}

const unsigned long long value;
};

// Compile-time hash function
constexpr unsigned long long _compiletime_hash(const char* str, unsigned long long hash = 0, size_t index = 0) {
return str[index] ? _compiletime_hash(str, (hash * 131) + str[index], index + 1) : hash;
}

// call_tag function with compile-time length check for string literals
template <size_t N>
constexpr call_tag_t call_tag(const char (&str)[N]) {
  static_assert(N > 11, "Tag string must be at least 11 characters long."); // N includes the null terminator
  return call_tag_t(_compiletime_hash(str));
}
```



Usage at `src\internal\aww-date.cpp`

```cpp
if (deps.clipboard_set_text(result, aww::call_tag("t7svmrrhai0"))) {
    std::cout << "Copied to clipboard: " << result << "\n";
    deps.show_notification("aww date", "The date has been copied to the clipboard", aww::call_tag("tssis4p5ta2"));
} else {
    std::cout << "Failed to copy to clipboard: " << result << "\n";
    deps.show_notification("aww date", "Failed to copy the date to the clipboard", aww::call_tag("730v5jc2d3o"));
    return 1;
}
```





## 2023-12-02

Added first dev sketch: 

aww structured logging
./doc-dev-articles\2023-12-02-aww-tools-structured-logging.md

## 2023-08-20

`aww-open` refactored with io dependency and unit tests

```
aww run mdtree -FilterName ".*aww-open.*" -Output dev-experiments-unit-tests\output-aww-open.md
```





## 2023-08-12

### aww git open

My current unit-testing strategy feels overwhelming:

Dependency: 

- Interface, implementation 
- Test implementation, manual stubs

It requires some time and discipline, but it seems to be working. 

For instance, now I can write unit-tests or experiment by replacing parts of the 

tool with something new and keep the previous implementation for other tools. 

The Dependency patterns gives me this flexibility. 

Today, I have hopefully replaces all dependencies in `aww-git-open`

### aww guid

I have designed an new prompt to assist me with stub generation for unit-tests, here it is:



#### GPT Prompt

When I say a command !implement: followed by function signature, I want you to write the implementation for the C++ function.
Do not put any extra boilerplate code (like class definitions), just give me the implementation close to the example below.
Example, my request:

```
!implement:
virtual inline aww::Result launch_file_in_browser(const std::string &url) = 0;
```

Your response:
file `io_dependencies_interface`:
```cpp
        /**
         * Launch a file in the default browser.
         * @param url The url to launch.
         * @return Aww::Result indicating the success or failure of the operation.
         */
        virtual inline aww::Result launch_file_in_browser(const std::string &url) = 0;
```

file `io_dependencies`:
```cpp
        inline aww::Result launch_file_in_browser(const std::string &url) override
        {
          // TODO: implement me
        }
```

file `io_dependencies_stub`:
```cpp
        // always define a default stub function (do not put this comment in the resulting code)
        std::function<aww::Result(const std::string&)>
            launch_file_in_browser_stub = [this]([[maybe_unused]]const std::string &url) -> aww::Result
        {
            return aww::Result::ok();
        };
        // always define a counter (do not put this comment in the resulting code)
        int launch_file_in_browser_called = 0;

        // always call the stub function and increment the call counter (do not put this comment in the resulting code)
        aww::Result launch_file_in_browser(const std::string &url) override
        {
            launch_file_in_browser_called += 1;
            return launch_file_in_browser_stub(url);
        }
```



Also  [dot-awwtools/aww-scripts/mdtree.ps1 at main · dzharii/dot-awwtools · GitHub](https://github.com/dzharii/dot-awwtools/blob/main/aww-scripts/mdtree.ps1) helps a lot for setting GPT context and asking it to generate tests. 

```
aww run mdtree -FilterName ".*aww-guid.*"
```

### And the unit-testing prompt worked well


I want you to be an expert developer in Test in modern C++ language who:

- writes solid and simple-to-understand unit-test for C++ projects
- uses doctest/doctest.h library for unit-testing
- prioritizes the most important tests to cover the most important code first
- writes bug-free and elegant code.

In my next message, I will give you a task; before providing your answer with the code,
brainstorm it internally:

- find and fix possible bugs
- make code organization more elegant
- ensure the code is testable and advice how to improve testability
- ensure there are no security vulnerabilities.

Codding style:
- for unused parameters use `[[maybe_unused]]` attribute

Attempt to fix the issues from discovered in the brainstorm and only then provide the answer to my request.
Say "Ack" to acknowledge or ask me any questions that may improve your response.
And then wait for my next request with the task for you.



## 2023-06-30

- Tiny refactoring for aww-git-open. Ready for making unit-tests.

## 2023-06-22

The error handling aww::Result code for I/O operations is too verbose and complex.
I've decided to revert some code to use std::filesystem functions instead and added
global error handler.

```cpp
int main(int argc, char **argv)
{
  try
  {
    std::vector<std::string> cmdArgs(argv, argv + argc);
    cmdArgs.erase(cmdArgs.begin()); // remove first element

    aww::internal::aww_create::aww_create_io_dependencies deps;
    return aww::internal::aww_create::aww_create_main(cmdArgs, deps);
  }
  catch (std::exception &ex)
  {
    std::cerr << ex.what() << "\n";
    return 1;
  }
  catch (...)
  {
    std::cerr << "Caught unknown exception.\n";
    return 1;
  }
```

The exception situation is truly should be !! exceptional !!, so I'd rather handle I/O error in the
global handler.

## 2023-06-19
The idea about returning aw::Result from each I/O operation adds lots of ugly error handling code.

nope, maybe some global try/cath will be better.
Maybe I should whitch back to std::* io opearions.



```
    fs::path currentDir;
    if (aww::Result res = deps.fs_get_current_directory_absolute_path(currentDir); res.is_failed())
    {
      std::cout << "Failed to get current directory"
                << res.error()
                << "\n";
      return 1;
    }

```


## 2023-06-19

aww::Result improving experiment

2023-06-20 [if statement - cppreference.com](https://en.cppreference.com/w/cpp/language/if)
```cpp
    if (aww::Result res = deps.fs_get_current_directory_absolute_path(currentDir); res.is_failed())
    {
      std::cout << "Failed to get current directory"
                << res.error()
                << "\n";
      return 1;
    }
```



## 2023-06-17

- `aww_git_open` started refactoring to extract I/O dependencies


## 2023-06-08

- Refactor / move from `aww create` to `awwlib`
  - `aww::fs::` file_or_dir_exists, create_directories,
  - Rename camel case to snake case


## 2023-06-07

- Use `aww-run-aww` for custom cmd, sh and ps1 scripts


## 2023-06-05

- The Great Unit testing refactoring
- Introduced dependency pattern for input/output dependencies.
- refactored `aww date` and `aww create` to use dependency injection and covered
the utils with unit-tests.
- `CPP_HEADER_FILE_NAME` renamed to `CAPITALIZED_FILE_NAME`
This is important milestone to make code more stable and perform less manual adhoc tests.

## 2023-05-01
- fixed bug in aww run when failed command was reported as failed and then successful. O_O
- wintoastlib -- removed debug output

## 2023-04-16

`aww.cpp` now provides a static definition of the tool list and passes arguments directly to the tool's `main()` function using  `const std::vector<std::string> &cmdArgs`. This change eliminates the need to check for the existence of `aww-tool.exe` using the filesystem and replaces the previous flow of reading arguments, serializing them as a string, and passing them to the `system()` call. Now, arguments are passed directly to the tool's main function as shown in the example below:
Before:
```
aww tool-name => read args => serialize args back as string => find-if aww-tool exists => pass args as a string via system(...) call.
```
After:
```
aww date one two three => aww::internal::aww_date::aww_date_main(awwExecutableArgs)
````
These changes result in a more efficient and streamlined workflow for using the aww tools.

## 2023-04-10

- add more `aww create` templates for powershell cmd and sh companions

## 2023-03-27
- fixed bug in `aww create` : `CPP_HEADER_FILE_NAME` to generate valid identifiers.


the commands:
```sh
aww create .\include\internal\aww-xyz.hpp
aww create .\src\internal\aww-xyz.cpp
```
were very helpful during migration to lib.


## 2021-03-26

Migrated to library:
- `aww-create.cpp`
- `aww-git-open.cpp`
- `aww-guid.cpp`
- ... and all others

## 2023-03-24

Changelog created!
`aww create` now creates folders when the file name does not have an extension. This makes it impossible to create extension-less files, but for the sake of simplicity -- it is what it is.

`aww date` -- converted to use `main` implementation from lib.







