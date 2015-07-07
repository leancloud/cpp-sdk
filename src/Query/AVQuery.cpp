/**
 * @file   AVQuery.cpp
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Wed Aug 13 15:25:33 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */
#include "Query/AVQuery.h"

#include <string>
#include <vector>

#include "Request/AVPaasClient.h"

NS_AV_BEGIN

AVQuery::AVQuery():className(),
                   parameters(Json::Value(Json::objectValue)),
                   where(Json::Value(Json::objectValue)),
                   selectedKeys(),
                   include(),
                   order(),
                   limit(0),
                   skip(0) {
}

AVQuery* AVQuery::orQueryWithSubqueries(std::vector<AVQuery*> queries) {
  if (queries.size() <= 0) {
    return nullptr;
  }

  std::string className;

  Json::Value input(Json::arrayValue);

  for (auto& query : queries) {
    input.append(query->where);
    if (className.length() != 0 && query->className != className) {
      continue;
    }

    className = query->className;
  }

  AVQuery *result = AVQuery::queryWithClassName(className);
  result->where["$or"] = input;
  return result;
}

AVQuery* AVQuery::andQueryWithSubqueries(std::vector<AVQuery*> queries) {
  if (queries.size() <= 0) {
    return nullptr;
  }

  std::string className;

  Json::Value input(Json::arrayValue);

  for (auto& query : queries) {
    input.append(query->where);
    if (className.length() != 0 && query->className != className) {
      continue;
    }

    className = query->className;
  }

  AVQuery *result = AVQuery::queryWithClassName(className);
  if (input.size() > 1) {
    result->where["$and"] = input;
  } else {
    result->where = input[0];
  }

  return result;
}

AVQuery* AVQuery::queryWithClassName(std::string className) {
  AVQuery* query = new AVQuery();
  query->className = className;

  return query;
}

void AVQuery::release() {
  AV_SAFE_DELETE(this);
}

void AVQuery::includeKey(std::string key) {
  this->include.insert(key);
}

void AVQuery::selectKeys(std::vector<std::string> keys) {
  this->selectedKeys.insert(keys.begin(), keys.end());
}

void AVQuery::whereKeyExists(std::string key) {
  Json::Value dict(Json::objectValue);
  dict["$exists"] = 1;
  this->addWhereItemForKey(dict, key);
}

void AVQuery::whereKeyDoesNotExist(std::string key) {
  Json::Value dict(Json::objectValue);
  dict["$exists"] = 0;
  this->addWhereItemForKey(dict, key);
}

void AVQuery::whereKeyEqualTo(std::string key, Json::Value const & object) {
  Json::Value dict(Json::objectValue);
  dict["$eq"] = object;
  this->addWhereItemForKey(dict, key);
}

void AVQuery::whereKeyEqualTo(std::string key, AVObject* object) {
  Json::Value dict(Json::objectValue);
  Json::Value objectDict(Json::objectValue);
  objectDict["__type"] = "Pointer";
  objectDict["className"] = object->className;
  if (object->hasValidObjectId()) {
    objectDict["objectId"] = object->objectId;
  }

  dict["$eq"] = objectDict;
}

void AVQuery::whereKeyLessThan(std::string key, Json::Value const & object) {
  Json::Value dict(Json::objectValue);
  dict["$lt"] = object;
  this->addWhereItemForKey(dict, key);
}

void AVQuery::whereKeyLessThanOrEqualTo(std::string key,
                                        Json::Value const & object) {
  Json::Value dict(Json::objectValue);
  dict["$lte"] = object;
  this->addWhereItemForKey(dict, key);
}

void AVQuery::whereKeyGreaterThan(std::string key, Json::Value const & object) {
  Json::Value dict(Json::objectValue);
  dict["$gt"] = object;
  this->addWhereItemForKey(dict, key);
}

void AVQuery::whereKeyGreaterThanOrEqualTo(std::string key,
                                           Json::Value const & object) {
  Json::Value dict(Json::objectValue);
  dict["$gte"] = object;
  this->addWhereItemForKey(dict, key);
}

void AVQuery::whereKeyNotEqualTo(std::string key, Json::Value const & object) {
  Json::Value dict(Json::objectValue);
  dict["$ne"] = object;
  this->addWhereItemForKey(dict, key);
}

void AVQuery::whereKeyContainedIn(std::string key,
                                  std::vector<Json::Value> const & array) {
  Json::Value jsonArray(Json::arrayValue);
  for (auto& object : array) {
    jsonArray.append(object);
  }

  Json::Value dict(Json::objectValue);
  dict["$in"] = jsonArray;

  this->addWhereItemForKey(dict, key);
}

void AVQuery::whereKeyNotContainedIn(std::string key,
                                     std::vector<Json::Value> const & array) {
  Json::Value jsonArray(Json::arrayValue);
  for (auto& object : array) {
    jsonArray.append(object);
  }

  Json::Value dict(Json::objectValue);
  dict["$nin"] = jsonArray;

  this->addWhereItemForKey(dict, key);
}

void AVQuery::whereKeyContainsAllObjectsInArray(std::string key,
                                                std::vector<Json::Value> const & array) {
  Json::Value jsonArray(Json::arrayValue);
  for (auto& object : array) {
    jsonArray.append(object);
  }

  Json::Value dict(Json::objectValue);
  dict["$all"] = jsonArray;

  this->addWhereItemForKey(dict, key);
}

void AVQuery::whereKeyNearGeoPoint(std::string key, AVGeoPoint* geoPoint) {
  Json::Value dict(Json::objectValue);
  dict["$nearSphere"] = AVGeoPoint::dictionaryFromGeoPoint(geoPoint);
  this->addWhereItemForKey(dict, key);
}

void AVQuery::whereKeyNearGeoPointWithinMiles(std::string key,
                                              AVGeoPoint* geoPoint,
                                              double maxDistance) {
  Json::Value dict(Json::objectValue);
  dict["$nearSphere"] = AVGeoPoint::dictionaryFromGeoPoint(geoPoint);
  dict["$maxDistanceInMiles"] = maxDistance;
  this->addWhereItemForKey(dict, key);
}

void AVQuery::whereKeyNearGeoPointWithinKilometers(std::string key,
                                                   AVGeoPoint* geoPoint,
                                                   double maxDistance) {
  Json::Value dict(Json::objectValue);
  dict["$nearSphere"] = AVGeoPoint::dictionaryFromGeoPoint(geoPoint);
  dict["$maxDistanceInKilometers"] = maxDistance;
  this->addWhereItemForKey(dict, key);
}

void AVQuery::whereKeyNearGeoPointWithinRadians(std::string key,
                                                AVGeoPoint* geoPoint,
                                                double maxDistance) {
  Json::Value dict(Json::objectValue);
  dict["$nearSphere"] = AVGeoPoint::dictionaryFromGeoPoint(geoPoint);
  dict["$maxDistanceInRadians"] = maxDistance;
  this->addWhereItemForKey(dict, key);
}

void AVQuery::whereKeyMatchesRegex(std::string key, std::string regex) {
  Json::Value dict(Json::objectValue);
  dict["$regex"] = regex;
  this->addWhereItemForKey(dict, key);
}

void AVQuery::whereKeyContainsString(std::string key, std::string substring) {
  this->whereKeyMatchesRegex(key,
                             StringUtils::string_format(".*%s.*", substring));
}

void AVQuery::whereKeyHasPrefix(std::string key, std::string prefix) {
  this->whereKeyMatchesRegex(key, StringUtils::string_format("^%s.*", prefix));
}

void AVQuery::whereKeyHasSuffix(std::string key, std::string suffix) {
  this->whereKeyMatchesRegex(key, StringUtils::string_format(".*%s$", suffix));
}

void AVQuery::whereKeySizeEqualTo(std::string key, int count) {
  Json::Value dict(Json::objectValue);
  dict["$size"] = count;
  this->addWhereItemForKey(dict, key);
}

void AVQuery::orderByAscending(std::string key) {
  this->order = key;
}

void AVQuery::addAscendingOrder(std::string key) {
  if (this->order.length() <= 0) {
    this->orderByAscending(key);
    return;
  }

  this->order = StringUtils::string_format("%s,%s", this->order, key);
}

void AVQuery::orderByDescending(std::string key) {
  this->order = StringUtils::string_format("-%s", key);
}

void AVQuery::addDescendingOrder(std::string key) {
  if (this->order.length() <= 0) {
    this->orderByDescending(key);
    return;
  }

  this->order = StringUtils::string_format("%s,-%s", this->order, key);
}

AVObject* AVQuery::getObjectWithId(std::string objectId) {
  AVObject* object = AVObject::objectWithClassName(this->className);
  object->objectId = objectId;
  object->fetch();

  return object;
}

std::vector<AVObject*> AVQuery::findObjects() {
  std::vector<AVObject*> objects;

  std::string path = StringUtils::string_format("classes/%s", this->className);
  this->assembleParameters();

  AVPaasClient::sharedInstance()->
      getObject(path, this->parameters, [&](Json::Value const & root, AVError const & error){
          if (error.domain.length() == 0) {
            Json::Value results = root["results"];
            for (auto it = results.begin(); it != results.end(); ++it) {
              Json::Value jsonValue = *it;
              AVObject* object = AVObject::objectWithClassName(this->className);

              for (auto objIt = jsonValue.begin(); objIt != jsonValue.end(); ++objIt) {
                std::string key = objIt.key().asString();

                if (key == "objectId") {
                  std::string value = (*objIt).asString();
                  object->objectId = value;
                } else if (key == "createdAt") {
                  std::string value = (*objIt).asString();
                  object->createdAt = value;
                } else if (key == "updatedAt") {
                  std::string value = (*objIt).asString();
                  object->updatedAt = value;
                }
              }

              object->localData = jsonValue;
              object->localData.removeMember("objectId");
              object->localData.removeMember("createdAt");
              object->localData.removeMember("updatedAt");

              objects.push_back(object);
            }
          }
        });

  return objects;
}

int AVQuery::countObjects() {
  int count = 0;
  this->countObjectsInBackgroundWithCallback([&](int const & result, AVError const & error){
      count = result;
    });

  return count;
}

void AVQuery::countObjectsInBackgroundWithCallback(AVIntegerResultCallback callback) {
  std::string path = StringUtils::string_format("classes/%s", this->className);
  this->assembleParameters();

  this->parameters["limit"] = 0;
  this->parameters["count"] = 1;

  AVPaasClient::sharedInstance()->
      getObject(path, this->parameters, [&](Json::Value const & root, AVError const & error){
          if (error.domain.length() == 0) {
            int count = root["count"].asInt();
            callback(count, error);
          } else {
            callback(-1, error);
          }
        });
}

///////////////////////////////private methods/////////////////////////////
void AVQuery::addWhereItemForKey(Json::Value const & dict, std::string key) {
  if (dict.isMember("$eq")) {
    if (this->where.isMember("$and")) {
      Json::Value eqDict = Json::Value(Json::objectValue);
      eqDict[key] = dict["$eq"];

      this->where["$and"].append(eqDict);
    } else {
      Json::Value eqArray(Json::arrayValue);

      Json::Value eqDict(Json::objectValue);
      eqDict[key] = dict["$eq"];

      eqArray.append(eqDict);
      this->where["$and"] = eqArray;
    }
  } else {
    if (this->where.isMember(key)) {
      this->where[key].append(dict);
    } else {
      this->where[key] = dict;
    }
  }
}

void AVQuery::assembleParameters() {
  this->parameters = Json::nullValue;
  if (!this->where.empty()) {
    this->parameters["where"] = this->where.toStyledString();
  }

  if (this->limit > 0) {
    this->parameters["limit"] = this->limit;
  }

  if (this->skip > 0) {
    this->parameters["skip"] = this->skip;
  }

  if (this->order.length() > 0) {
    this->parameters["order"] = this->order;
  }

  if (this->include.size() > 0) {
    std::string includeStr;

    for (auto& item : this->include) {
      includeStr.append(item);
      includeStr.append(",");
    }

    this->parameters["include"] = includeStr;
  }

  if (this->selectedKeys.size() > 0) {
    Json::Value selectedKeysArray(Json::arrayValue);
    for (auto& item : this->include) {
      selectedKeysArray.append(item);
    }

    this->parameters["keys"] = selectedKeysArray;
  }
}

void AVQuery::setWhere(Json::Value where) {
  this->where = where;
}

NS_AV_END
