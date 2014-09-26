#define BOOST_TEST_MODULE AVPaasClient 1.0 Get Put Post Delete Test
#include <string>
#include <unordered_map>
#include <boost/test/unit_test.hpp>
#include "Request/AVPaasClient.h"

using namespace std;
using namespace avoscloud;

#error replace ${appId} and ${appKey} with real appId and appKey

BOOST_AUTO_TEST_CASE(AVPaasClientGetTest) {
  AVPaasClient *client = AVPaasClient::sharedInstance();

  client->applicationId = "${appId}";
  client->clientKey = "${appKey}";

  Json::Value parameters;
  client->getObject("classes/Post/536b5fbee4b098d841dc9854", parameters, [&](Json::Value const & root, AVError const & error){
      BOOST_CHECK(error.domain.length() == 0);

      for (auto it = root.begin(); it != root.end(); ++it)
      {
        BOOST_CHECK_EQUAL(root["objectId"].asString(), "536b5fbee4b098d841dc9854");
      }
    });
}

BOOST_AUTO_TEST_CASE(AVPaasClientPostTest) {
  AVPaasClient *client = AVPaasClient::sharedInstance();

  client->applicationId = "${appId}";
  client->clientKey = "${appKey}";

  Json::Value parameters;
  parameters["name"] = "debugger87";

  client->postObject("classes/Post", parameters, [&](Json::Value const & root, AVError const & error){
      BOOST_CHECK(error.domain.length() == 0);
      BOOST_CHECK(root["objectId"].asString().length() > 0);
    });
}
