#define BOOST_TEST_MODULE AVObject 1.0 Test
#include <string>
#include <vector>
#include <boost/test/unit_test.hpp>
#include "AVOSCloud.h"
#include "AVObject/AVObject.h"
#include "AVObject/AVRelation.h"

using namespace std;
using namespace avoscloud;

#error replace ${appId} and ${appKey} with real appId and appKey

BOOST_AUTO_TEST_CASE(AVObject_SetObject_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");
  AVObject* object = AVObject::objectWithClassName("Post");
  object->setObjectForKey("cyang", "name");
  object->setObjectForKey(27, "age");
  vector<string> keys = object->allKeys();
  BOOST_CHECK_EQUAL(keys.size(), 2);

  object->release();
}

BOOST_AUTO_TEST_CASE(AVObject_RemoveObject_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");
  AVObject* object = AVObject::objectWithClassName("Post");
  object->setObjectForKey("cyang", "name");
  object->setObjectForKey(27, "age");
  object->removeObjectForKey("age");
  vector<string> keys = object->allKeys();
  BOOST_CHECK_EQUAL(keys.size(), 1);

  object->release();
}

BOOST_AUTO_TEST_CASE(AVObject_SaveInBackground_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");
  AVObject* object = AVObject::objectWithClassName("Post");
  object->setObjectForKey("cyang", "name");
  object->setObjectForKey(27, "age");

  object->saveInBackground();
  BOOST_CHECK(object->objectId.length() > 0);
  object->deleteInBackground();

  object->release();
}

BOOST_AUTO_TEST_CASE(AVObject_SaveObjectInBackgroundWithCallback_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");
  AVObject* object = AVObject::objectWithClassName("Post");
  object->setObjectForKey("cyang", "name");
  object->setObjectForKey(27, "age");

  object->saveInBackgroundWithCallback([&](bool const& succeeded, AVError const& error){
      BOOST_CHECK(succeeded);
    });
  object->deleteInBackground();

  object->release();
}

BOOST_AUTO_TEST_CASE(AVObject_UpdateObject_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");
  AVObject* object = AVObject::objectWithClassName("post");
  object->objectId = "53e9dc19e4b08d999ea69659";
  object->fetch();
  object->setObjectForKey("male", "gender");
  object->setObjectForKey("yahoo", "name");
  object->saveInBackgroundWithCallback([&](bool const& succeeded, AVError const& error){
      BOOST_CHECK(succeeded);
    });
  object->release();
}

BOOST_AUTO_TEST_CASE(AVObject_SaveAllInBackground_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");
  std::vector<AVObject*> objects;

  for (int i = 0; i < 2; ++i)
  {
      AVObject* object = AVObject::objectWithClassName("Post");
      object->setObjectForKey("cyang", "name");
      object->setObjectForKey(i, "age");
      objects.push_back(object);
  }

  AVObject::saveAllInBackground(objects);

  for (auto& object:objects)
  {
    BOOST_CHECK(object->objectId.length() > 0);
    object->deleteInBackground();
    object->release();
  }
}

BOOST_AUTO_TEST_CASE(AVObject_SaveAllInBackgroundWithCallback_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");
  std::vector<AVObject*> objects;

  for (int i = 10; i < 12; ++i)
  {
      AVObject* object = AVObject::objectWithClassName("Post");
      object->setObjectForKey("debugger", "name");
      object->setObjectForKey(i, "age");
      objects.push_back(object);
  }

  AVObject::saveAllInBackgroundWithCallback(objects, [&](bool const& succeeded, AVError const& error){
      BOOST_CHECK(succeeded);
    });

  for (auto& object:objects)
  {
    object->deleteInBackground();
    object->release();
  }
}

BOOST_AUTO_TEST_CASE(AVObject_fetch_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");
  AVObject* object = AVObject::objectWithClassName("Post");
  object->objectId = "53e9cff1e4b0648828d795cb";
  object->fetch();
  BOOST_CHECK_EQUAL(object->objectForKey("age").asInt(), 19);

  object->release();
}

BOOST_AUTO_TEST_CASE(AVObject_PointerType_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");
  AVObject* object = AVObject::objectWithClassName("Post");
  object->objectId = "53e9cff1e4b0648828d795cb";
  object->fetch();

  AVObject* student = AVObject::objectWithClassName("Student");
  student->setObjectForKey(object, "children");

  student->saveInBackgroundWithCallback([&](bool const& succeeded, AVError const& error){
      BOOST_CHECK(succeeded);
    });

  student->release();
}

BOOST_AUTO_TEST_CASE(AVObject_DeleteObject_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");
  AVObject* object = AVObject::objectWithClassName("Armor");
  object->saveInBackground();

  object->deleteInBackgroundWithCallback([&](bool const& succeeded, AVError const& error){
      BOOST_CHECK(succeeded);
    });

  object->release();
}

BOOST_AUTO_TEST_CASE(AVObject_GeoType_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");

  AVObject* object = AVObject::objectWithClassName("Armor");
  AVGeoPoint* point = AVGeoPoint::geoPointWithLatitudeAndLongitude(39.9139, 116.3917);

  object->setObjectForKey(point, "location");
  object->saveInBackgroundWithCallback([&](bool const& succeeded, AVError const& error){
      BOOST_CHECK(succeeded);
    });

  object->deleteInBackground();
  object->release();
}

BOOST_AUTO_TEST_CASE(AVObject_AddRelationForKey_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");

  AVObject* student = AVObject::objectWithClassName("Student");
  student->setObjectForKey("cyang", "name");
  student->saveInBackground();

  AVObject* post = AVObject::objectWithClassName("Post");
  post->objectId = "53e9cff1e4b0648828d795cb";

  student->addRelationForKey(post, "like");

  student->saveInBackgroundWithCallback([&](bool const& succeeded, AVError const& error){
      BOOST_CHECK(succeeded);
    });

  student->deleteInBackground();

  student->release();
}

BOOST_AUTO_TEST_CASE(AVObject_RemoveRelationForKey_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");

  AVObject* student = AVObject::objectWithClassName("Student");
  student->setObjectForKey("cyang", "name");
  student->saveInBackground();

  AVObject* post = AVObject::objectWithClassName("Post");
  post->objectId = "53e9cff1e4b0648828d795cb";

  student->addRelationForKey(post, "like");

  student->saveInBackgroundWithCallback([&](bool const& succeeded, AVError const& error){
      BOOST_CHECK(succeeded);
    });

  student->removeRelationForKey(post, "like");

  student->saveInBackgroundWithCallback([&](bool const& succeeded, AVError const& error){
      BOOST_CHECK(succeeded);
    });

  student->deleteInBackground();

  post->release();
  student->release();
}
