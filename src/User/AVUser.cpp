/**
 * @file   AVUser.cpp
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Wed Aug 27 17:22:15 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#include "User/AVUser.h"

#include <string>

#include "Request/AVPaasClient.h"
#include "Utils/AVStringUtils.h"

NS_AV_BEGIN

AVUser::AVUser():sessionToken(),
                 username(),
                 password(),
                 email() {
  this->className = "_User";
}

AVUser* AVUser::currentUser() {
  AVUser* user = AVPaasClient::sharedInstance()->currentUser;
  if (user != nullptr) {
    return user;
  } else {
    user = AVUser::user();
    AVPaasClient::sharedInstance()->currentUser = user;

    return user;
  }
}

bool AVUser::isAuthenticated() {
  if (this->sessionToken.length() > 0) {
    return true;
  }

  return false;
}

AVUser* AVUser::user() {
  AVUser* user = new AVUser();

  return user;
}

void AVUser::release() {
  AV_SAFE_DELETE(this);
}

void AVUser::requestEmailVerifyAndCallback(std::string email,
                                           AVBooleanResultCallback callback) {
  Json::Value parameters;
  parameters["email"] = email;
  AVPaasClient::sharedInstance()->
      postObject("requestEmailVerify",
                 parameters, [&](Json::Value const & root, AVError const & error){
                   if (error.domain.length() == 0) {
                     callback(true, error);
                   } else {
                     callback(false, error);
                   }
                 });
}

void AVUser::signUp() {
  if (!this->isAuthenticated() &&
      this->username.length() > 0 &&
      this->password.length() > 0) {
    this->setObjectForKey(this->username, "username");
    this->setObjectForKey(this->password, "password");

    if (this->email.length() > 0) {
      this->setObjectForKey(this->email, "email");
    }

    this->saveInBackgroundWithCallback([&](bool const& succeeded, AVError const& error){
        if (succeeded) {
          this->sessionToken = this->localData["sessionToken"].asString();

          AVPaasClient::sharedInstance()->currentUser = this;
        }
      });
  }
}

void AVUser::updatePasswordWithCallback(std::string oldPassword,
                                        std::string newPassword,
                                        AVIdResultCallback callback) {
  if (this->isAuthenticated() &&
      oldPassword.length() > 0 &&
      newPassword.length() > 0) {
    std::string path = StringUtils::string_format("users/%s/updatePassword", this->objectId);
    Json::Value parameters;
    parameters["old_password"] = oldPassword;
    parameters["new_password"] = newPassword;

    AVPaasClient::sharedInstance()->
        putObject(path,
                  parameters,
                  this->sessionToken,
                  [&](Json::Value const & root, AVError const & error){
                    callback(root, error);
                  });
  } else {
    if (!this->isAuthenticated()) {
      Json::Value root;
      AVError error(kAVErrorDomain, kAVErrorUserCannotBeAlteredWithoutSession);
      callback(root, error);
    } else if (!(oldPassword.length() > 0 && newPassword.length() > 0)) {
      Json::Value root;
      AVError error(kAVErrorDomain, kAVErrorUserPasswordMissing);
      callback(root, error);
    }
  }
}

AVUser* AVUser::loginWithUsernameAndPassword(std::string username,
                                             std::string password) {
  if (username.length() > 0 && password.length() > 0) {
    std::string path = "login";
    Json::Value parameters;
    parameters["username"] = username;
    parameters["password"] = password;

    AVUser* user = AVUser::user();
    user->username = username;
    user->password = password;
    AVPaasClient::sharedInstance()->
        postObject(path,
                   parameters,
                   [&](Json::Value const & root, AVError const & error){
                     if (error.domain.length() == 0) {
                       user->objectId = root["objectId"].asString();
                       user->username = root["username"].asString();
                       user->sessionToken = root["sessionToken"].asString();
                       user->email = root["email"].asString();

                       AVPaasClient::sharedInstance()->currentUser = user;
                     } else {
                       user = nullptr;
                     }
                   });

    return user;
  }

  return nullptr;
}

void AVUser::logOut() {
  AVPaasClient::sharedInstance()->currentUser = nullptr;
}

void AVUser::requestPasswordResetForEmail(std::string email) {
  std::string path = "requestPasswordReset";
  Json::Value parameters;
  parameters["email"] = email;

  AVPaasClient::sharedInstance()->
      postObject(path,
                 parameters,
                 [&](Json::Value const & root, AVError const & error){
                   // do nothing
                 });
}

AVQuery* AVUser::query() {
  AVQuery* query = AVQuery::queryWithClassName("_User");

  return query;
}

NS_AV_END
