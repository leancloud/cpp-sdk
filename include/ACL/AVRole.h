/**
 * @file   AVRole.h
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Thu Sep 18 17:06:50 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#ifndef INCLUDE_ACL_AVROLE_H_
#define INCLUDE_ACL_AVROLE_H_

#include <string>
#include "AVObject/AVObject.h"
#include "Utils/AVPlatformMacros.h"

NS_AV_BEGIN

class AVQuery;

class AVRole : public AVObject {
private:
  AVRole();

public:
  /**
   * Gets or sets the name for a role. This value must be set before the role
   * has been saved to the server, and cannot be set once the role has been
   * saved.
   * A role's name can only contain alphanumeric characters, _, -, and spaces.
   *
   */
  std::string name;

public:
  /**
   * Constructs a new AVRole with the given name. If no default ACL has been
   * specified, you must set an ACL for the role.
   *
   * @param name The name of the Role to create.
   *
   * @return pointer to AVRole instance
   */
  static AVRole* roleWithName(std::string name);

  /**
   * Constructs a new AVRole with the given name.
   *
   * @param name The name of the Role to create.
   * @param acl The ACL for this role. Roles must have an ACL.
   *
   * @return pointer to AVRole instance
   */
  static AVRole* roleWithNameAndACL(std::string name, AVACL* acl);

  /**
   * Release memory.
   *
   */
  void release();

  /**
   * Gets the AVRelation for the AVUsers that are direct children of this role.
   * These users are granted any privileges that this role has been granted
   * (e.g. read or write access through ACLs). You can add or remove users from
   * the role through this relation.
   *
   * @return the relation for the users belonging to this role.
   */
  AVRelation* users();

  /**
   * Gets the AVRelation for the AVRoles that are direct children of this role.
   * These roles' users are granted any privileges that this role has been granted
   * (e.g. read or write access through ACLs). You can add or remove child roles
   * from this role through this relation.
   *
   * @return the relation for the roles belonging to this role.
   */
  AVRelation* roles();

  /**
   * Querying for Roles
   *
   * @return pointer to AVQuery
   */
  static AVQuery* query();
};

NS_AV_END

#endif  // INCLUDE_ACL_AVROLE_H_
