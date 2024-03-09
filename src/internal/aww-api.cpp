#include <iostream>
#include <string>
#include <regex>

#include "spdlog/spdlog.h"
#include "fmt/core.h"
#include "internal/aww-api.hpp"

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/URI.h>
#include <iostream>
#include <sstream>

namespace aww::internal::aww_api
{
    using namespace Poco::Net;
    using namespace Poco;
    using namespace std;

    int aww_api_main([[maybe_unused]] const std::vector<std::string> &cmdArgs, aww_api_io_dependencies_interface &deps)
    {
        spdlog::info("Hello spdlog");
        fmt::print("Hello, world from fmt PLEASE REMOVE THIS \b!\n");

        // The URI we are requesting data from
        URI uri("http://example.com");
        std::string path(uri.getPathAndQuery());
        if (path.empty())
            path = "/";

        // Create a session using the URI's host and port
        HTTPClientSession session(uri.getHost(), uri.getPort());

        // Create and send the request
        HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
        session.sendRequest(request);

        // Receive the response
        HTTPResponse response;

        // Print the status and reason for the response
        cout << response.getStatus() << " " << response.getReason() << endl;

        // Extract the response body
        istream &rs = session.receiveResponse(response);
        stringstream ss;
        StreamCopier::copyStream(rs, ss);

        // Print the response body
        cout << ss.str();

        deps.show_notification("aww api", "It works", aww::call_tag("9mnoizxrhdw"));
        return 0;
    }

}
