/**
 * @file   AVACL.h
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Fri Jul 18 17:50:49 2014
 *
 * @brief  AVACL
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#ifndef INCLUDE_ACL_AVACL_H_
#define INCLUDE_ACL_AVACL_H_

#include <string>
#include "json/json.h"
#include "Utils/AVPlatformMacros.h"

NS_AV_BEGIN

class AVUser;
class AVRole;

class AVACL {
private:
  AVACL();
  Json::Value dictionaryToReadAndWrite(bool read, bool write);
  Json::Value dictionaryForKeyIfCreate(std::string key, bool create);
  void allowReadForKey(bool allowed, std::string key);
  bool isReadAllowedForKey(std::string key);
  void allowWriteForyKey(bool allowed, std::string key);
  bool isWriteAllowedForyKey(std::string key);
  std::string roleName(std::string name);

public:
  Json::Value permissionById;

public:
  /**
   * Creates an ACL with no permissions granted.
   *
   * @return pointer to AVACL instance
   */
  static AVACL* ACL();

  /**
   * Creates an ACL where only the provided user has access.
   *
   * @param user the AVUser
   *
   * @return pointer to AVACL instance
   */
  static AVACL* ACLWithUser(AVUser* user);

  /**
   * release memory.
   *
   */
  void release();

  /**
   * Set whether the public is allowed to read this object.
   *
   * @param allowed allowed or not
   */
  void setPublicReadAccess(bool allowed);

  /**
   * Gets whether the public is allowed to read this object.
   *
   * @return true if allowed else false
   */
  bool getPublicReadAccess();

  /**
   * Set whether the public is allowed to write this object.
   *
   * @param allowed allowed or not
   */
  void setPublicWriteAccess(bool allowed);

  /**
   * Gets whether the public is allowed to write this object.
   *
   * @return true if allowed else false
   */
  bool getPublicWriteAccess();

  /**
   * Set whether the given user id is allowed to read this object.
   *
   * @param allowed allowed or not
   * @param userId the AVUser's objectId
   */
  void setReadAccessForUserId(bool allowed, std::string userId);

  /**
   * Gets whether the given user id is *explicitly* allowed to read this object.
   * Even if this returns false, the user may still be able to access it if getPublicReadAccess returns true
   * or if the user belongs to a role that has access.
   *
   * @param userId the AVUser's objectId
   *
   * @return true if allowed else false
   */
  bool getReadAccessForUserId(std::string userId);

  /**
   * Set whether the given user id is allowed to write this object.
   *
   * @param allowed allowed or not
   * @param userId the AVUser's objectId
   */
  void setWriteAccessForUserId(bool allowed, std::string userId);

  /**
   * Gets whether the given user id is *explicitly* allowed to write this object.
   * Even if this returns false, the user may still be able to write it if getPublicWriteAccess returns true
   * or if the user belongs to a role that has access.
   *
   * @param userId the AVUser's objectId
   *
   * @return true if allowed else false
   */
  bool getWriteAccessForUserId(std::string userId);

  /**
   * Set whether the given user is allowed to read this object.
   *
   * @warning if the user doesn't have valid objectId, no operation is performed.
   * @param allowed allowed or not
   * @param user the AVUser
   */
  void setReadAccessForUser(bool allowed, AVUser* user);

  /**
   * Gets whether the given user is *explicitly* allowed to read this object.
   * Even if this returns false, the user may still be able to access it if getPublicReadAccess returns true
   * or if the user belongs to a role that has access.
   *
   * @param user the AVUser
   *
   * @return true if allowed else false. if the user doesn't have valid objectId, return true.
   */
  bool getReadAccessForUser(AVUser* user);

  /**
   * Set whether the given user is allowed to write this object.
   *
   * @warning if the user doesn't have valid objectId, no operation is performed.
   * @param allowed allowed or not
   * @param user the AVUser
   */
  void setWriteAccessForUser(bool allowed, AVUser* user);

  /**
   * Gets whether the given user is *explicitly* allowed to write this object.
   * Even if this returns false, the user may still be able to write it if getPublicWriteAccess returns true
   * or if the user belongs to a role that has access.
   *
   * @param user the AVUser
   *
   * @return true if allowed else false. if the user doesn't have valid objectId, return true.
   */
  bool getWriteAccessForUser(AVUser* user);

  /**
   * Set whether users belonging to the role with the given name are allowed
   * to read this object.
   *
   * @param allowed The name of the role.
   * @param name Whether the given role can read this object.
   */
  void setReadAccessForRoleWithName(bool allowed, std::string name);

  /**
   * Get whether users belonging to the role with the given name are allowed
   * to read this object. Even if this returns false, the role may still
   * be able to read it if a parent role has read access.
   *
   * @param name The name of the role.
   *
   * @return if allowed else false
   */
  bool getReadAccessForRoleWithName(std::string name);

  /**
   * Set whether users belonging to the role with the given name are allowed
   * to write this object.
   *
   * @param allowed The name of the role.
   * @param name Whether the given role can write this object.
   */
  void setWriteAccessForRoleWithName(bool allowed, std::string name);

  /**
   * Get whether users belonging to the role with the given name are allowed
   * to write this object. Even if this returns false, the role may still
   * be able to write it if a parent role has write access.
   *
   * @param name The name of the role.
   *
   * @return if allowed else false
   */
  bool getWriteAccessForRoleWithName(std::string name);

  /**
   * Set whether users belonging to the given role are allowed to read this
   * object. The role must already be saved on the server and its data must have
   * been fetched in order to use this method.
   *
   * @param allowed Whether the given role can read this object.
   * @param role The role to assign access.
   */
  void setReadAccessForRole(bool allowed, AVRole* role);

  /**
   * Get whether users belonging to the given role are allowed to read this
   * object. Even if this returns NO, the role may still be able to
   * read it if a parent role has read access. The role must already be saved on
   * the server and its data must have been fetched in order to use this method.
   *
   * @param role the given role
   *
   * @return true if the role has read access. false otherwise.
   */
  bool getReadAccessForRole(AVRole* role);

  /**
   * Set whether users belonging to the given role are allowed to write this
   * object. The role must already be saved on the server and its data must have
   * been fetched in order to use this method.
   *
   * @param allowed Whether the given role can write this object.
   * @param role The role to assign access.
   */
  void setWriteAccessForRole(bool allowed, AVRole* role);

  /**
   * Get whether users belonging to the given role are allowed to write this
   * object. Even if this returns NO, the role may still be able to
   * write it if a parent role has write access. The role must already be saved on
   * the server and its data must have been fetched in order to use this method.
   *
   * @param role the given role
   *
   * @return true if the role has write access. false otherwise.
   */
  bool getWriteAccessForRole(AVRole* role);

  /**
   * Sets a default ACL that will be applied to all AVObjects when they are created.
   *
   * @param acl The ACL to use as a template for all AVObjects created after setDefaultACL has been called.
   * This value will be copied and used as a template for the creation of new ACLs, so changes to the
   * instance after setDefaultACL has been called will not be reflected in new AVObjects.
   * @param currentUserAccess If true, the AVACL that is applied to newly-created AVObjects will
   * provide read and write access to the currentUser at the time of creation. If false,
   * the provided ACL will be used without modification. If acl is nil, this value is ignored.
   */
  static void setDefaultACLWithCurrentUser(AVACL* acl, bool currentUserAccess);
};

NS_AV_END

#endif  // INCLUDE_ACL_AVACL_H_
