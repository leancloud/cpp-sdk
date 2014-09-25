/**
 * @file   AVObject.h
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Tue Sep  2 14:49:54 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#ifndef INCLUDE_AVOBJECT_AVOBJECT_H_
#define INCLUDE_AVOBJECT_AVOBJECT_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <boost/any.hpp>

#include "AVConstants.h"
#include "Utils/AVPlatformMacros.h"
#include "Geo/AVGeoPoint.h"

NS_AV_BEGIN

class AVRelation;
class AVACL;

class AVObject {
private:
  AVACL* acl;

private:
  void addObjectIntoRelationDataForKey(std::unordered_map<std::string, std::vector<AVObject*>> *relationData,
                                       AVObject* object,
                                       std::string key);
  Json::Value generateRelationObjectsByArray(std::vector<AVObject*> objects,
                                             bool isAdded);

protected:
  /**
   * constructor of AVObject.
   *
   * @return
   */
  AVObject();

  /**
   * get object path of the object.
   *
   * @return
   */
  std::string myObjectPath();


public:
  /**
   *  The id of the object.
   *
   */
  std::string objectId;

  /**
   * When the object was last updated.
   *
   */
  std::string updatedAt;

  /**
   * When the object was created.
   *
   */
  std::string createdAt;

  /**
   * The class name of the object.
   *
   */
  std::string className;

  /**
   * store data of object, which is key-value compliant.
   *
   */
  Json::Value localData;

  /**
   * objects will be added into AVRelation.
   *
   */
  std::unordered_map<std::string, std::vector<AVObject*>> addedRelationData;

  /**
   * objects will be removed from AVRelation.
   *
   */
  std::unordered_map<std::string, std::vector<AVObject*>> removedRelationData;

public:
  /**
   * Creates a new AVObject with a class name.
   *
   * @param className A class name can be any alphanumeric string that begins with a letter. It represents an object in your app, like a User of a Document.
   *
   * @return the object that is instantiated with the given class name.
   */
  static AVObject* objectWithClassName(std::string const & className);

  /**
   * Creates a reference to an existing AVObject for use in creating associations between AVObjects.  Calling isDataAvailable on this
 object will return NO until fetchIfNeeded or refresh has been called.  No network request will be made.
   *
   * @param className The object's class.
   * @param objectId The object id for the referenced object.
   *
   * @return A AVObject without data.
   */
  static AVObject* objectWithoutDataWithClassName(std::string const & className,
                                                  std::string const & objectId);

  /**
   * Release memory.
   *
   */
  void release();

  /**
   * Set the ACL for this object.
   *
   */
  void setACL(AVACL* acl);

  /**
   * Get the ACL for this object.
   *
   * @return ACL of this object
   */
  AVACL* getACL();

  /**
   *  Returns an array of the keys contained in this object. This does not include
 createdAt, updatedAt, authData, or objectId. It does include things like username
 and ACL.
   *
   * @return
   */
  std::vector<std::string> allKeys();

  /**
   * Returns the object associated with a given key.
   *
   * @param key The key that the object is associated with.
   *
   * @return The value associated with the given key, or nil if no value is associated with key.
   */
  Json::Value objectForKey(std::string const & key);

  /**
   * Sets the object associated with a given key.
   *
   * @param object The object.
   * @param key The key.
   */
  void setObjectForKey(Json::Value const & object, std::string const & key);

  /**
   * Sets the object associated with a given key.
   *
   * @param object an AVObject instance with objectId.
   * @param key The key.
   */
  void setObjectForKey(AVObject* const &  object, std::string const & key);

  /**
   * Sets the object associated with a given key.
   *
   * @param object an AVGeoPoint instance.
   * @param key The key.
   */
  void setObjectForKey(AVGeoPoint* const & geoPoint, std::string const & key);

  /**
   * Sets the object associated with a given key.
   *
   * @param key The key.
   */
  void removeObjectForKey(std::string const & key);

  /**
   * Returns the relation object associated with the given key
   *
   * @param key The key that the relation is associated with.
   *
   * @return an AVRelation.
   */
  AVRelation* relationForKey(std::string key);

  /**
   * Add object into relation data; The object must have valid objectId.
   *
   * @param object The object to be added into relation data;
   * @param key relation key.
   */
  void addRelationForKey(AVObject* object, std::string key);

  /**
   * Remove object from relation data; The object must have valid objectId.
   *
   * @param object The object to be removed from relation data;
   * @param key relation key.
   */
  void removeRelationForKey(AVObject* object, std::string key);

  /**
   * Saves the AVObject asynchronously.
   *
   */
  void saveInBackground();

  /**
   * Saves the AVObject asynchronously and executes the given callback block.
   *
   * @param callback The callback to execute. The callback should have the following argument sianature: (bool const&, AVError const&)
   *
   */
  void saveInBackgroundWithCallback(AVBooleanResultCallback callback);

  /**
   * Saves a collection of objects all at once.
   *
   * @param objects
   */
  static void saveAllInBackground(std::vector<AVObject*> objects);

  /**
   *  Saves a collection of objects all at once asynchronously and calls a callback when done.
   *
   * @param objects
   * @param callback
   */
  static void saveAllInBackgroundWithCallback(std::vector<AVObject*> objects,
                                              AVBooleanResultCallback callback);

  /**
   *  Fetches the AVObject with the current data from the server.
   *
   */
  void fetch();

  /**
   *  Fetches the AVObject with the current data and specified keys from the server and sets an error if it occurs.
   *
   * @param keys
   */
  void fetchWithKeys(std::vector<std::string> keys);

  /**
   * Deletes the AVObject.
   *
   */
  void deleteInBackground();

  /**
   * delet the AVObject and execute the given callback.
   *
   * @param callback
   */
  void deleteInBackgroundWithCallback(AVBooleanResultCallback callback);

  /**
   * check if the object has valid objectId
   *
   * @return
   */
  bool hasValidObjectId();
};

NS_AV_END

#endif  // INCLUDE_AVOBJECT_AVOBJECT_H_
