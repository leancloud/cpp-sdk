#define BOOST_TEST_MODULE AVObject 1.0 Test
#include <string>
#include <vector>
#include <boost/test/unit_test.hpp>
#include "Geo/AVGeoPoint.h"

using namespace std;
using namespace avoscloud;

BOOST_AUTO_TEST_CASE(AVGeoPoint_Distance_Test) {
  AVGeoPoint* point1 = AVGeoPoint::geoPointWithLatitudeAndLongitude(39.9139, 116.3917);
  AVGeoPoint* point2 = AVGeoPoint::geoPointWithLatitudeAndLongitude(-39.9139, 116.3917);

  BOOST_CHECK(point1->distanceInKillometersTo(point2) >= 8000);

  point1->release();
  point2->release();
}
