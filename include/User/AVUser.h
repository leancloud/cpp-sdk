/**
 * @file   AVUser.h
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Fri Jul 18 17:40:23 2014
 *
 * @brief  AVUser
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#ifndef INCLUDE_USER_AVUSER_H_
#define INCLUDE_USER_AVUSER_H_

#include <string>
#include "AVObject/AVObject.h"
#include "Query/AVQuery.h"
#include "Utils/AVPlatformMacros.h"
#include "AVConstants.h"

NS_AV_BEGIN

class AVUser : public AVObject {
private:
  AVUser();

public:
  /**
   * The session token for the AVUser. This is set by the server upon successful authentication.
   *
   */
  std::string sessionToken;

  /**
   * The username for the AVUser.
   *
   */
  std::string username;

  /**
   * The password for the AVUser. This will not be filled in from the server with
   * the password. It is only meant to be set.
   *
   */
  std::string password;

  /**
   * The email for the AVUser.
   *
   */
  std::string email;

public:
  /**
   * Gets the currently logged in user from disk and returns an instance of it.
   *
   * @return a AVUser that is the currently logged in user. If there is none, returns nullptr.
   */
  static AVUser* currentUser();

  /**
   * Whether the user is an authenticated object for the device. An authenticated AVUser is one that is obtained via
   * a signUp or logIn method. An authenticated object is required in order to save (with altered values) or delete it.
   *
   * @return whether the user is authenticated.
   */
  bool isAuthenticated();

  /**
   * Creates a new AVUser object.
   *
   * @return a new AVUser object.
   */
  static AVUser* user();

  /**
   * Request to re-send email for verification.
   *
   * @param email The address of email
   * @param callback The callback function
   *
   */
  static void requestEmailVerifyAndCallback(std::string email,
                                            AVBooleanResultCallback callback);

  /**
   * release memory.
   *
   */
  void release();

  /**
   * Signs up the user. Make sure that password and username are set. This will also enforce that the username isn't already taken.
   *
   */
  void signUp();

  /**
   * update user's password with callback
   *
   * @param oldPassword old password
   * @param newPassword new password
   * @param callback The callback to be executed
   */
  void updatePasswordWithCallback(std::string oldPassword,
                                  std::string newPassword,
                                  AVIdResultCallback callback);

  /**
   * Makes a request to login a user with specified credentials. Returns an instance
   * of the successfully logged in AVUser. This will also cache the user locally so
   * that calls to userFromCurrentUser will use the latest logged in user.
   *
   * @param username The username of the user.
   * @param password The password of the user.
   *
   * @return an instance of the AVUser on success. If login failed for either wrong password or wrong username, returns nullptr.
   */
  static AVUser* loginWithUsernameAndPassword(std::string username,
                                              std::string password);

  /**
   * Logs out the currently logged in user on disk.
   *
   */
  static void logOut();

  /**
   * Send a password reset request for a specified email. If a user account exists with that email,
   * an email will be sent to that address with instructions on how to reset their password.
   *
   * @param email
   */
  static void requestPasswordResetForEmail(std::string email);

  /**
   * Creates a query for AVUser objects.
   *
   * @return An instance of AVQuery
   */
  static AVQuery* query();
};

NS_AV_END

#endif  // INCLUDE_USER_AVUSER_H_
