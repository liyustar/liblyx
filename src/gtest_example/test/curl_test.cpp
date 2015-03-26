#include "gtest/gtest.h"

#include <curl/curl.h>
#include <string>
#include <iostream>

int a = 123;

size_t wcb(char *ptr, size_t size, size_t nmumb, void *userp)
{
    int n = *(int*)userp;
    EXPECT_EQ(123, n);
    std::cout << "write_callback: (" << size * nmumb << ")  " << std::string(ptr, size * nmumb);
    return size * nmumb;
}

TEST(CurlTest, Post) {
    CURL *curl;
    // std::string url  = "http://192.168.1.135:8080";
    std::string url  = "http://192.168.1.135:8080/quantexpt-ims-web/interfaces/test";
    std::string data = "method=hello&params={test:1,price:6.8,qty:100}";

    curl = curl_easy_init();
    EXPECT_TRUE(curl != NULL);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wcb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&a);

    CURLcode result = curl_easy_perform(curl);
    EXPECT_EQ(CURLE_OK, result) << curl_easy_strerror(result);

    curl_easy_cleanup(curl);
}
