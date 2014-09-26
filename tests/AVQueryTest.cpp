#define BOOST_TEST_MODULE AVQuery 1.0 Test
#include <iostream>
#include <string>
#include <vector>
#include <boost/test/unit_test.hpp>
#include "AVOSCloud.h"
#include "AVObject/AVObject.h"
#include "Query/AVQuery.h"

using namespace std;
using namespace avoscloud;

#error replace ${appId} and ${appKey} with real appId and appKey

BOOST_AUTO_TEST_CASE(AVQuery_FindObjects_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");
  AVQuery *query = AVQuery::queryWithClassName("Student");
  query->whereKeyEqualTo("age", 27);
  query->limit = 10;
  query->skip = 1;
  std::vector<AVObject*> objects = query->findObjects();

  BOOST_CHECK(objects.size() > 0);

  query->release();
}

BOOST_AUTO_TEST_CASE(AVQuery_CountObjects_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");
  AVQuery *query = AVQuery::queryWithClassName("Student");
  query->whereKeyEqualTo("age", 27);
  query->limit = 10;
  query->skip = 1;

  int count = query->countObjects();

  BOOST_CHECK(count != -1);

  query->release();
}

BOOST_AUTO_TEST_CASE(AVQuery_OrSubQueries_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");

  AVQuery *query1 = AVQuery::queryWithClassName("Student");
  query1->whereKeyEqualTo("age", 27);
  query1->limit = 10;
  query1->skip = 1;

  AVQuery *query2 = AVQuery::queryWithClassName("Student");
  query2->whereKeyEqualTo("name", "cyang");
  query2->limit = 10;
  query2->skip = 1;

  std::vector<AVQuery*> queries{query1, query2};
  AVQuery* query3 = AVQuery::orQueryWithSubqueries(queries);

  std::vector<AVObject*> objects = query3->findObjects();
  BOOST_CHECK(objects.size() > 0);

  query1->release();
  query2->release();
  query3->release();
}

BOOST_AUTO_TEST_CASE(AVQuery_AndSubQueries_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");

  AVQuery *query1 = AVQuery::queryWithClassName("Student");
  query1->whereKeyEqualTo("age", 27);
  query1->limit = 10;
  query1->skip = 1;

  AVQuery *query2 = AVQuery::queryWithClassName("Student");
  query2->whereKeyEqualTo("name", "cyang");
  query2->limit = 10;
  query2->skip = 1;

  std::vector<AVQuery*> queries{query1, query2};
  AVQuery* query3 = AVQuery::andQueryWithSubqueries(queries);

  std::vector<AVObject*> objects = query3->findObjects();
  BOOST_CHECK(objects.size() > 0);

  query1->release();
  query2->release();
  query3->release();
}
