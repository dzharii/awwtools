#include <iostream>
#include <vector>
#include <string>
#include "internal/aww-web.hpp"

/**
 * Project Name: aww-web
 * Description:
 * I'm developing aww-web, a tool designed to extend the functionality of aww tools 
 * into a web interface. 
 * By leveraging webview, it will present a cross-platform browser window, 
 * enabling users to interact 
 * with aww tools via a web page. This approach will facilitate a user-friendly
 * environment to execute tools 
 * such as aww guid, aww open, and aww create directly through the web,
 * ensuring a seamless integration between 
 * the web interface and the underlying C++ code.
 *
 * The essence of aww-web lies in its capability to execute C++ code from the web page,
 * maintaining output consistency 
 * with the command line versions of aww tools. For instance, generating a new GUID
 * or opening files with the system's 
 * default application can be done through the web interface with the same effectiveness
 * as the command line tools.
 *
 * Objective:
 * The primary aim is to enhance accessibility and ease of use for aww tools by
 * bridging the command line utility 
 * with web accessibility. This new tool will not only preserve the performance
 * and simplicity of the existing tools 
 * but also introduce a more versatile and convenient way for users to leverage
 * the aww tools suite.
 */


int main(int argc, char **argv)
{
  try
  {
    std::vector<std::string> cmdArgs(argv, argv + argc);
    cmdArgs.erase(cmdArgs.begin()); // remove first element

    aww::internal::aww_web::aww_web_io_dependencies deps;
    return aww::internal::aww_web::aww_web_main(cmdArgs, deps);
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
}
