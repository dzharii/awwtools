#include "internal/aww-api.hpp"
#include <iostream>
#include <string>
#include <vector>

/*
I'm working on a new tool for the aww command suite, introducing aww api.
This tool makes interacting with APIs easier using a command like
aww api <profile> <endpoint> --<parameter> "<value>".
For example, to query GPT-4: aww api chatgpt.gpt4 --prompt "How old is the universe?".

Profile Configuration: The tool uses JSON or YAML profiles, such as chatgpt.json or chatgpt.yaml,
to store API configurations. These profiles detail the URL template for the endpoint,
REST configurations for the call, and methods for retrieving the API key.

Local Storage and Management: Profiles are stored in an aww-apis directory.
This helps organize the profiles and allows the tool to find and use them easily.

Usage Example:

    To fetch data or interact with an API, navigate to the folder containing your
    aww-apis directory and run:

    aww api service.endpoint --parameter "value"

    For GPT-4, to ask a question:

    aww api chatgpt.gpt4 --prompt "What's the weather like on Mars?"

The chatgpt.gpt4 indicates the profile with all necessary configurations,
and --prompt shows how to pass parameters.
*/

int main(int argc, char** argv) {
  try {
    std::vector<std::string> cmdArgs(argv, argv + argc);
    cmdArgs.erase(cmdArgs.begin()); // remove first element

    aww::internal::aww_api::aww_api_io_dependencies deps;
    return aww::internal::aww_api::aww_api_main(cmdArgs, deps);
  } catch (std::exception& ex) {
    std::cerr << ex.what() << "\n";
    return 1;
  } catch (...) {
    std::cerr << "Caught unknown exception.\n";
    return 1;
  }
}
