/**
 * @file   AVQuery.h
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Wed Aug 13 15:25:24 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#ifndef INCLUDE_QUERY_AVQUERY_H_
#define INCLUDE_QUERY_AVQUERY_H_

#include <string>
#include <vector>
#include <unordered_set>
#include "AVConstants.h"
#include "AVObject/AVObject.h"
#include "Geo/AVGeoPoint.h"
#include "Utils/AVPlatformMacros.h"

NS_AV_BEGIN
class AVQuery {
private:
  AVQuery();
  void addWhereItemForKey(Json::Value const & dict, std::string key);
  void assembleParameters();

private:
  Json::Value parameters;
  Json::Value where;
  std::unordered_set<std::string> selectedKeys;
  std::unordered_set<std::string> include;
  std::string order;

public:
  /**
   * The class name to query for
   *
   */
  std::string className;

  /**
   * A limit on the number of objects to return.  Note: If you are calling findObject with limit=1, you may find it easier to use getFirst instead.
   *
   */
  int limit;

  /**
   * The number of objects to skip before returning any.
   *
   */
  int skip;

public:
  /**
   * Initializes the query with a class name.
   *
   * @param newClassName The class name.
   *
   * @return
   */
  static AVQuery* queryWithClassName(std::string className);

  /**
   * Returns a AVQuery that is the or of the passed in AVQuerys.
   *
   * @param queries The list of queries to or together.
   *
   * @return a AVQuery that is the or of the passed in AVQuerys.
   */
  static AVQuery* orQueryWithSubqueries(std::vector<AVQuery*> queries);

  /**
   * Returns a AVQuery that is the AND of the passed in AVQuerys.
   *
   * @param queries The l¡ist of queries to AND together.
   *
   * @return a AVQuery that is the AND of the passed in AVQuerys.
   */
  static AVQuery* andQueryWithSubqueries(std::vector<AVQuery*> queries);

  /**
   * release memory.
   *
   */
  void release();

  /**
   * Make the query include AVObjects that have a reference stored at the provided key.
   * This has an effect similar to a join.  You can use dot notation to specify which fields in
   * the included object are also fetch.
   *
   * @param key The key to load child AVObjects for.
   */
  void includeKey(std::string key);

  /**
   * Make the query restrict the fields of the returned AVObjects to include only the provided keys.
   * If this is called multiple times, then all of the keys specified in each of the calls will be included.
   *
   * @param keys The keys to include in the result.
   */
  void selectKeys(std::vector<std::string> keys);

  /**
   * Add a constraint that requires a particular key exists.
   *
   * @param key The key that should exist.
   */
  void whereKeyExists(std::string key);

  /**
   * Add a constraint that requires a key not exist.
   *
   * @param key The key that should not exist.
   */
  void whereKeyDoesNotExist(std::string key);

  /**
   * Add a constraint to the query that requires a particular key's object to be equal to the provided object.
   *
   * @param key The key to be constrained.
   * @param object The object that must be equalled.
   */
  void whereKeyEqualTo(std::string key, Json::Value const & object);

  /**
   * Add a constraint to the query that requires a particular key's object to be equal to the provided object.
   *
   * @param key The key to be constrained.
   * @param object a pointer to AVObject instance.
   */
  void whereKeyEqualTo(std::string key, AVObject* object);

  /**
   * Add a constraint to the query that requires a particular key's object to be less than the provided object.
   *
   * @param key The key to be constrained.
   * @param object The object that provides an upper bound.
   */
  void whereKeyLessThan(std::string key, Json::Value const & object);

  /**
   * Add a constraint to the query that requires a particular key's object to be less than or equal to the provided object.
   *
   * @param key The key to be constrained.
   * @param object The object that must be equalled.
   */
  void whereKeyLessThanOrEqualTo(std::string key, Json::Value const & object);

  /**
   * Add a constraint to the query that requires a particular key's object to be greater than the provided object.
   *
   * @param key The key to be constrained.
   * @param object The object that must be equalled.
   */
  void whereKeyGreaterThan(std::string key, Json::Value const & object);

  /**
   * Add a constraint to the query that requires a particular key's object to be greater than or equal to the provided object.
   *
   * @param key The key to be constrained.
   * @param object The object that must be equalled.
   */
  void whereKeyGreaterThanOrEqualTo(std::string key,
                                    Json::Value const & object);

  /**
   * Add a constraint to the query that requires a particular key's object to be not equal to the provided object.
   *
   * @param key The key to be constrained.
   * @param object The object that must not be equalled.
   */
  void whereKeyNotEqualTo(std::string key, Json::Value const & object);

  /**
   * Add a constraint to the query that requires a particular key's object to be contained in the provided array.
   *
   * @param key The key to be constrained.
   * @param array The possible values for the key's object.
   */
  void whereKeyContainedIn(std::string key,
                           std::vector<Json::Value> const & array);

  /**
   * Add a constraint to the query that requires a particular key's object not be contained in the provided array.
   *
   * @param key The key to be constrained.
   * @param array The list of values the key's object should not be.
   */
  void whereKeyNotContainedIn(std::string key,
                              std::vector<Json::Value> const & array);

  /**
   * Add a constraint to the query that requires a particular key's array contains every element of the provided array.
   *
   * @param key The key to be constrained.
   * @param array The array of values to search for.
   */
  void whereKeyContainsAllObjectsInArray(std::string key,
                                         std::vector<Json::Value> const & array);

  /**
   * Add a constraint to the query that requires a particular key's coordinates (specified via AVGeoPoint) be near
   * a reference point.  Distance is calculated based on angular distance on a sphere.  Results will be sorted by distance
   * from reference point.
   *
   * @param key The key to be constrained.
   * @param geoPoint The reference point.  A AVGeoPoint.
   */
  void whereKeyNearGeoPoint(std::string key, AVGeoPoint* geoPoint);

  /**
   * Add a constraint to the query that requires a particular key's coordinates (specified via AVGeoPoint) be near
   * a reference point and within the maximum distance specified (in miles).  Distance is calculated based on
   * a spherical coordinate system.  Results will be sorted by distance (nearest to farthest) from the reference point.
   *
   * @param key The key to be constrained.
   * @param geoPoint The reference point.  A AVGeoPoint.
   * @param maxDistance Maximum distance in miles.
   */
  void whereKeyNearGeoPointWithinMiles(std::string key,
                                       AVGeoPoint* geoPoint,
                                       double maxDistance);

  /**
   * Add a constraint to the query that requires a particular key's coordinates (specified via AVGeoPoint) be near
   * a reference point and within the maximum distance specified (in kilometers).  Distance is calculated based on
   * a spherical coordinate system.  Results will be sorted by distance (nearest to farthest) from the reference point.
   *
   * @param key The key to be constrained.
   * @param geoPoint The reference point.  A AVGeoPoint.
   * @param maxDistance Maximum distance in kilometers.
   */
  void whereKeyNearGeoPointWithinKilometers(std::string key,
                                            AVGeoPoint* geoPoint,
                                            double maxDistance);

  /**
   * Add a constraint to the query that requires a particular key's coordinates (specified via AVGeoPoint) be near
   * a reference point and within the maximum distance specified (in radians).  Distance is calculated based on
   * angular distance on a sphere.  Results will be sorted by distance (nearest to farthest) from the reference point.
   *
   * @param key The key to be constrained.
   * @param geoPoint The reference point.  A AVGeoPoint.
   * @param maxDistance Maximum distance in radians.
   */
  void whereKeyNearGeoPointWithinRadians(std::string key,
                                         AVGeoPoint* geoPoint,
                                         double maxDistance);

  /**
   * Add a regular expression constraint for finding string values that match the provided regular expression.
   * This may be slow for large datasets.
   *
   * @param key The key that the string to match is stored in.
   * @param regex The regular expression pattern to match.
   */
  void whereKeyMatchesRegex(std::string key, std::string regex);

  /**
   * Add a constraint for finding string values that contain a provided substring.
   * This will be slow for large datasets.
   *
   * @param key The key that the string to match is stored in.
   * @param substring The substring that the value must contain.
   */
  void whereKeyContainsString(std::string key, std::string substring);

  /**
   * Add a constraint for finding string values that start with a provided prefix.
   * This will use smart indexing, so it will be fast for large datasets.
   *
   * @param key The key that the string to match is stored in.
   * @param prefix The substring that the value must start with.
   */
  void whereKeyHasPrefix(std::string key, std::string prefix);

  /**
   * Add a constraint for finding string values that end with a provided suffix.
   * This will be slow for large datasets.
   *
   * @param key The key that the string to match is stored in.
   * @param suffix The substring that the value must end with.
   */
  void whereKeyHasSuffix(std::string key, std::string suffix);

  /**
   * Matches any array with the number of elements specified by count
   *
   * @param key The key that the value is stored in, value should be kind of array
   * @param count the array size
   */
  void whereKeySizeEqualTo(std::string key, int count);

  /**
   * Sort the results in ascending order with the given key.
   *
   * @param key The key to order by.
   */
  void orderByAscending(std::string key);

  /**
   * Also sort in ascending order by the given key.  The previous keys provided will
   *
   * @param key The key to order bye
   */
  void addAscendingOrder(std::string key);

  /**
   * Sort the results in descending order with the given key.
   *
   * @param key The key to order by.
   */
  void orderByDescending(std::string key);

  /**
   * Also sort in descending order by the given key.  The previous keys provided will
   * precedence over this key.
   *
   * @param key The key to order by.
   */
  void addDescendingOrder(std::string key);

  /**
   * Returns a AVObject with the given id.
   * This mutates the AVQuery.
   *
   * @param objectId The id of the object that is being requested.
   *
   * @return The AVObject if found. Returns nil if the object isn't found, or if there was an error.
   */
  AVObject* getObjectWithId(std::string objectId);

  /**
   * Finds objects based on the constructed query.
   *
   * @return an array of AVObjects that were found.
   */
  std::vector<AVObject*> findObjects();

  /**
   * Counts objects based on the constructed query.
   *
   * @return the number of AVObjects that match the query, or -1 if there is an error.
   */
  int countObjects();

  /**
   * Counts objects asynchronously and calls the given block with the counts.
   *
   * @param callback the callbck to execute. The callback should have the following argument sianature: (int const&, AVError const&).
   */
  void countObjectsInBackgroundWithCallback(AVIntegerResultCallback callback);

  /**
   * Set where of The AVQuery
   *
   * @param where
   */
  void setWhere(Json::Value where);
};

NS_AV_END

#endif  // INCLUDE_QUERY_AVQUERY_H_
