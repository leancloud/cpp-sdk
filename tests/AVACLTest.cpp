#define BOOST_TEST_MODULE AVObject 1.0 Test
#include <string>
#include <vector>
#include <boost/test/unit_test.hpp>
#include "AVOSCloud.h"
#include "ACL/AVACL.h"
#include "AVObject/AVObject.h"

using namespace std;
using namespace avoscloud;

BOOST_AUTO_TEST_CASE(AVACL_SetReadAndWriteAccess_Test) {
  AVOSCloud::setApplicationId("rucl8605ver7n0o9ns5bg2yj6q9fwaw5v2gi7jtu8pjqiabm", "6455krb6zjawnn8mpruapvdltshmilny012u4yeys3tgusfd");
  AVObject* object = AVObject::objectWithClassName("Post");
  object->setObjectForKey("cyang", "name");
  object->setObjectForKey(27, "age");

  AVACL* acl = AVACL::ACL();
  acl->setReadAccessForUserId(true, "1234567890fedfe234");
  acl->setWriteAccessForUserId(true, "34567fedfedfab345");

  object->setACL(acl);
  object->saveInBackground();

  BOOST_CHECK(object->objectId.length() > 0);

  object->release();
  acl->release();
}
