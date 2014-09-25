#define BOOST_TEST_MODULE AVPaasClient 1.0 Get Put Post Delete Test
#include <string>
#include <unordered_map>
#include <boost/test/unit_test.hpp>
#include "Request/AVPaasClient.h"

using namespace std;
using namespace avoscloud;

BOOST_AUTO_TEST_CASE(AVPaasClientGetTest) {
  AVPaasClient *client = AVPaasClient::sharedInstance();

  client->applicationId = "rucl8605ver7n0o9ns5bg2yj6q9fwaw5v2gi7jtu8pjqiabm";
  client->clientKey = "6455krb6zjawnn8mpruapvdltshmilny012u4yeys3tgusfd";

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

  client->applicationId = "rucl8605ver7n0o9ns5bg2yj6q9fwaw5v2gi7jtu8pjqiabm";
  client->clientKey = "6455krb6zjawnn8mpruapvdltshmilny012u4yeys3tgusfd";

  Json::Value parameters;
  parameters["name"] = "debugger87";

  client->postObject("classes/Post", parameters, [&](Json::Value const & root, AVError const & error){
      BOOST_CHECK(error.domain.length() == 0);
      BOOST_CHECK(root["objectId"].asString().length() > 0);
    });
}
