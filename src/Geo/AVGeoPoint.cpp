/**
 * @file   AVGeoPoint.cpp
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Thu Aug 14 16:43:23 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#include "Geo/AVGeoPoint.h"

#include <cmath>

NS_AV_BEGIN

const double pi = 3.14159265358;

AVGeoPoint::AVGeoPoint():latitude(0),
                         longitude(0) {
}

AVGeoPoint* AVGeoPoint::geoPoint() {
  AVGeoPoint* point = new AVGeoPoint();
  return point;
}

AVGeoPoint* AVGeoPoint::geoPointWithLatitudeAndLongitude(double latitude, double longitude) {
  AVGeoPoint* point = AVGeoPoint::geoPoint();
  point->latitude = latitude;
  point->longitude = longitude;

  return point;
}

void AVGeoPoint::release() {
  AV_SAFE_DELETE(this);
}

double AVGeoPoint::distanceInRadiansTo(AVGeoPoint* point) {
  return this->distanceInKillometersTo(point) / 6378.140;
}

double AVGeoPoint::distanceInMilesTo(AVGeoPoint* point) {
  return this->distanceInKillometersTo(point) / 1.609344;
}

double AVGeoPoint::distanceInKillometersTo(AVGeoPoint* point) {
  double theta = this->longitude - point->longitude;
  double dist = sin(degreeToRadian(this->latitude)) * sin(degreeToRadian(point->latitude))
                + cos(degreeToRadian(this->latitude)) * cos(degreeToRadian(point->latitude))
                * cos(degreeToRadian(theta));

  dist = acos(dist);
  dist = radianToDegree(dist);
  dist = dist * 60 * 1.1515 * 1.609344;

  return dist;
}

Json::Value AVGeoPoint::dictionaryFromGeoPoint(AVGeoPoint* point) {
  Json::Value dict(Json::objectValue);
  dict["__type"] = "GeoPoint";
  dict["latitude"] = point->latitude;
  dict["longitude"] = point->longitude;

  return dict;
}

AVGeoPoint* AVGeoPoint::geoPointFromDictionary(Json::Value const & dict) {
  return AVGeoPoint::geoPointWithLatitudeAndLongitude(dict["latitude"].asDouble(),
                                                      dict["longitude"].asDouble());
}

///////////////////////////////private methods/////////////////////////////
double AVGeoPoint::degreeToRadian(double degree) {
  return degree * pi / 180.0;
}

double AVGeoPoint::radianToDegree(double radian) {
  return radian * 180.0 / pi;
}

NS_AV_END
