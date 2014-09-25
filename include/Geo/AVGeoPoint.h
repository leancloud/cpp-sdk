/**
 * @file   AVGeoPoint.h
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Thu Aug 14 16:30:17 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#ifndef INCLUDE_GEO_AVGEOPOINT_H_
#define INCLUDE_GEO_AVGEOPOINT_H_

#include "json/json.h"
#include "Utils/AVPlatformMacros.h"

NS_AV_BEGIN
class AVGeoPoint {
private:
  AVGeoPoint();
  double degreeToRadian(double degree);
  double radianToDegree(double radian);

public:
  /**
   * Latitude of point in degrees.  Valid range (-90.0, 90.0).
   *
   */
  double latitude;

  /**
   * Longitude of point in degrees.  Valid range (-180.0, 180.0).
   *
   */
  double longitude;

public:
  /**
   * Create a AVGeoPoint object.  Latitude and longitude are set to 0.0.
   *
   * @return a new AVGeoPoint.
   */
  static AVGeoPoint* geoPoint();

  /**
   * Creates a new AVGeoPoint object with the specified latitude and longitude.
   *
   * @param latitude Latitude of point in degrees.
   * @param longitude Longitude of point in degrees.
   *
   * @return New point object with specified latitude and longitude.
   */
  static AVGeoPoint* geoPointWithLatitudeAndLongitude(double latitude,
                                                      double longitude);

  /**
   * release memory.
   *
   */
  void release();

  /**
   * Get distance in radians from this point to specified point.
   *
   * @param point AVGeoPoint location of other point.
   *
   * @return distance in radians
   */
  double distanceInRadiansTo(AVGeoPoint* point);

  /**
   * Get distance in miles from this point to specified point.
   *
   * @param point AVGeoPoint location of other point.
   *
   * @return distance in miles
   */
  double distanceInMilesTo(AVGeoPoint* point);

  /**
   * Get distance in kilometers from this point to specified point.
   *
   * @param point AVGeoPoint location of other point.
   *
   * @return distance in kilometers.
   */
  double distanceInKillometersTo(AVGeoPoint* point);

  /**
   * get a dictionary from AVGeoPoint
   *
   * @param point
   *
   * @return
   */
  static Json::Value dictionaryFromGeoPoint(AVGeoPoint* point);

  /**
   * get an AVGeoPoint from dictionary
   *
   * @param dict
   *
   * @return
   */
  static AVGeoPoint* geoPointFromDictionary(Json::Value const & dict);
};

NS_AV_END

#endif  // INCLUDE_GEO_AVGEOPOINT_H_
