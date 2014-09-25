/**
 * @file   AVRelation.h
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Thu Aug 28 16:20:01 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#ifndef INCLUDE_AVOBJECT_AVRELATION_H_
#define INCLUDE_AVOBJECT_AVRELATION_H_

#include <string>

#include "AVObject/AVObject.h"
#include "Query/AVQuery.h"
#include "Utils/AVPlatformMacros.h"

NS_AV_BEGIN

/*!
 A class that is used to access all of the children of a many-to-many relationship.  Each instance
 of AVRelation is associated with a particular parent object and key.
 */
class AVRelation {
public:
  /**
   * parent object of this AVRelation.
   *
   */
  AVObject* parent;

  /**
   * key of this AVRelation.
   *
   */
  std::string key;

  /**
   * className of target class. if a person `like` a book, the relation key is `like`,
   * while the targetClass is the className of book.
   *
   */
  std::string targetClass;

public:
  /**
   * Query for an AVRelation.
   *
   * @return A AVQuery that can be used to get objects in this relation.
   */
  AVQuery* query();

  /**
   * release memory.
   *
   */
  void release();

  /**
   * Adds a relation to the passed in object.
   *
   * @param object AVObject to add relation to.
   */
  void addObject(AVObject* object);

  /**
   * Removes a relation to the passed in object.
   *
   * @param object AVObject to add relation to.
   */
  void removeObject(AVObject* object);

  /**
   * A AVQuery that can be used to get parent objects in this relation.
   *
   * @param parentClassName parent Class Name
   * @param relationKey relation Key
   * @param child child object
   *
   * @return the Query
   */
  static AVQuery* reverseQueryWithRelationKeyAndChildObject(std::string parentClassName,
                                                            std::string relationKey,
                                                            AVObject* child);
};

NS_AV_END

#endif  // INCLUDE_AVOBJECT_AVRELATION_H_
