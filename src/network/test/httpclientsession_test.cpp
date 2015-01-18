#include <gtest/gtest.h>
#include "lyxHTTPClientSession.h"
#include "lyxHTTPRequest.h"
#include "lyxHTTPResponse.h"
#include <iostream>

using namespace lyx;

TEST(HTTPSessionTest, Get) {
    HTTPClientSession s("localhost", 80);
    HTTPRequest request(HTTPRequest::HTTP_GET, "/");
    request.setContentLength(0);
    s.sendRequest(request);

    HTTPResponse response;
    s.receiveResponse(response);
    EXPECT_EQ (51, response.getContentLength());
    EXPECT_EQ ("text/html", response.getContentType());

}
