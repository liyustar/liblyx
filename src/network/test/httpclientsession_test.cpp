#include <gtest/gtest.h>
#include "lyxHTTPClientSession.h"
#include "lyxHTTPRequest.h"
#include "lyxHTTPResponse.h"
#include "lyxStreamCopier.h"
#include <iostream>

using namespace lyx;

TEST(HTTPSessionTest, Get) {
    HTTPClientSession s("localhost", 80);
    HTTPRequest request(HTTPRequest::HTTP_GET, "/");
    request.setContentLength(0);
    s.sendRequest(request);

    HTTPResponse response;
    std::istream& rs = s.receiveResponse(response);
    EXPECT_EQ (51, response.getContentLength());
    EXPECT_EQ ("text/html", response.getContentType());
    /*
    std::string str;
    StreamCopier::copyToString(rs, str, 800);
    std::cout << "Response Body:" << std::endl << str << std::endl;
    */
    std::ostringstream ostr;
    StreamCopier::copyStream(rs, ostr);
    std::cout << "Response Body:" << std::endl << ostr.str() << std::endl;
}
