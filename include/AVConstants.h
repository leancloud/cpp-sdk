/**
 * @file   AVConstants.h
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Thu Jul  3 16:38:56 2014
 *
 * @brief  constans will be located in this header file
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#ifndef INCLUDE_AVCONSTANTS_H_
#define INCLUDE_AVCONSTANTS_H_

#include <functional>
#include <string>
#include <vector>
#include "json/json.h"
#include "Utils/AVPlatformMacros.h"
#include "Utils/AVErrorUtils.h"

NS_AV_BEGIN

class AVObject;
class AVUser;

enum class AVCachePolicy {
    // Ignore Cache
    kAVCachePolicyIgnoreCache,

    // Cache Only
    kAVCachePolicyCacheOnly,

    // Network Only
    kAVCachePolicyNetworkOnly,

    // CacheElseNetwork
    kAVCachePolicyCacheElseNetwork,

    // NetworkElseCache
    kAVCachePolicyNetworkElseCache,

    // CacheThenNetwork
    kAVCachePolicyCacheThenNetwork,
};



// Errors

/*! @abstract 1: Internal server error. No information available. */
int const kAVErrorInternalServer = 1;
/*! @abstract 100: The connection to the AVOS servers failed. */
int const kAVErrorConnectionFailed = 100;
/*! @abstract 101: Object doesn't exist, or has an incorrect password. */
int const kAVErrorObjectNotFound = 101;
/*! @abstract 102: You tried to find values matching a datatype that doesn't
  support exact database matching, like an array or a dictionary. */
int const kAVErrorInvalidQuery = 102;
/*! @abstract 103: Missing or invalid classname. Classnames are case-sensitive.
  They must start with a letter, and a-zA-Z0-9_ are the only valid characters. */
int const kAVErrorInvalidClassName = 103;
/*! @abstract 104: Missing object id. */
int const kAVErrorMissingObjectId = 104;
/*! @abstract 105: Invalid key name. Keys are case-sensitive. They must start
  with a letter, and a-zA-Z0-9_ are the only valid characters. */
int const kAVErrorInvalidKeyName = 105;
/*! @abstract 106: Malformed pointer. Pointers must be arrays of a classname
  and an object id. */
int const kAVErrorInvalidPointer = 106;
/*! @abstract 107: Malformed json object. A json dictionary is expected. */
int const kAVErrorInvalidJSON = 107;
/*! @abstract 108: Tried to access a feature only available internally. */
int const kAVErrorCommandUnavailable = 108;
/*! @abstract 111: Field set to incorrect type. */
int const kAVErrorIncorrectType = 111;
/*! @abstract 112: Invalid channel name. A channel name is either an empty
  string (the broadcast channel) or contains only a-zA-Z0-9_ characters
  and starts with a letter. */
int const kAVErrorInvalidChannelName = 112;
/*! @abstract 114: Invalid device token. */
int const kAVErrorInvalidDeviceToken = 114;
/*! @abstract 115: Push is misconfigured. See details to find out how. */
int const kAVErrorPushMisconfigured = 115;
/*! @abstract 116: The object is too large. */
int const kAVErrorObjectTooLarge = 116;
/*! @abstract 119: That operation isn't allowed for clients. */
int const kAVErrorOperationForbidden = 119;
/*! @abstract 120: The results were not found in the cache. */
int const kAVErrorCacheMiss = 120;
/*! @abstract 121: Keys in NSDictionary values may not include '$' or '.'. */
int const kAVErrorInvalidNestedKey = 121;
/*! @abstract 122: Invalid file name. A file name contains only a-zA-Z0-9_.
  characters and is between 1 and 36 characters. */
int const kAVErrorInvalidFileName = 122;
/*! @abstract 123: Invalid ACL. An ACL with an invalid format was saved.
  This should not happen if you use PFACL. */
int const kAVErrorInvalidACL = 123;
/*! @abstract 124: The request timed out on the server. Typically this indicates
  the request is too expensive. */
int const kAVErrorTimeout = 124;
/*! @abstract 125: The email address was invalid. */
int const kAVErrorInvalidEmailAddress = 125;
/*! @abstract 137: A unique field was given a value that is already taken. */
int const kAVErrorDuplicateValue = 137;
/*! @abstract 139: Role's name is invalid. */
int const kAVErrorInvalidRoleName = 139;
/*! @abstract 140: Exceeded an application quota.  Upgrade to resolve. */
int const kAVErrorExceededQuota = 140;
/*! @abstract 141: Cloud Code script had an error. */
int const kAVScriptError = 141;
/*! @abstract 142: Cloud Code validation failed. */
int const kAVValidationError = 142;
/*! @abstract 143: Product purchase receipt is missing */
int const kAVErrorReceiptMissing = 143;
/*! @abstract 144: Product purchase receipt is invalid */
int const kAVErrorInvalidPurchaseReceipt = 144;
/*! @abstract 145: Payment is disabled on this device */
int const kAVErrorPaymentDisabled = 145;
/*! @abstract 146: The product identifier is invalid */
int const kAVErrorInvalidProductIdentifier = 146;
/*! @abstract 147: The product is not found in the App Store */
int const kAVErrorProductNotFoundInAppStore = 147;
/*! @abstract 148: The Apple server response is not valid */
int const kAVErrorInvalidServerResponse = 148;
/*! @abstract 149: Product fails to download due to file system error */
int const kAVErrorProductDownloadFileSystemFailure = 149;
/*! @abstract 150: Fail to convert data to image. */
int const kAVErrorInvalidImageData = 150;
/*! @abstract 151: Unsaved file. */
int const kAVErrorUnsavedFile = 151;
/*! @abstract 153: Fail to delete file. */
int const kAVErrorFileDeleteFailure = 153;

/*! @abstract 200: Username is missing or empty */
int const kAVErrorUsernameMissing = 200;
/*! @abstract 201: Password is missing or empty */
int const kAVErrorUserPasswordMissing = 201;
/*! @abstract 202: Username has already been taken */
int const kAVErrorUsernameTaken = 202;
/*! @abstract 203: Email has already been taken */
int const kAVErrorUserEmailTaken = 203;
/*! @abstract 204: The email is missing, and must be specified */
int const kAVErrorUserEmailMissing = 204;
/*! @abstract 205: A user with the specified email was not found */
int const kAVErrorUserWithEmailNotFound = 205;
/*! @abstract 206: The user cannot be altered by a client without
  the session. */
int const kAVErrorUserCannotBeAlteredWithoutSession = 206;
/*! @abstract 207: Users can only be created through sign up */
int const kAVErrorUserCanOnlyBeCreatedThroughSignUp = 207;
/*! @abstract 208: An existing Facebook account already linked to
  another user. */
int const kAVErrorFacebookAccountAlreadyLinked = 208;
/*! @abstract 208: An existing account already linked to another user. */
int const kAVErrorAccountAlreadyLinked = 208;
/*! @abstract 209: User ID mismatch */
int const kAVErrorUserIdMismatch = 209;
/*! @abstract 210: The username and password mismatch. */
int const kAVErrorUsernamePasswordMismatch = 210;
/*! @abstract 211: Could not find user. */
int const kAVErrorUserNotFound = 211;
/*! @abstract 250: Facebook id missing from request */
int const kAVErrorFacebookIdMissing = 250;
/*! @abstract 250: Linked id missing from request */
int const kAVErrorLinkedIdMissing = 250;
/*! @abstract 251: Invalid Facebook session */
int const kAVErrorFacebookInvalidSession = 251;
/*! @abstract 251: Invalid linked session */
int const kAVErrorInvalidLinkedSession = 251;

typedef std::function<void(bool const&, AVError const&)> AVBooleanResultCallback;
typedef std::function<void(int const&, AVError const&)> AVIntegerResultCallback;
typedef std::function<void(std::istream const&, AVError const&)> AVDataStreamResultCallback;
typedef std::function<void(std::string const&, AVError const&)> AVStringResultCallback;
typedef std::function<void(int const&)> AVProgressCallback;
typedef std::function<void(Json::Value const&, AVError const&)> AVIdResultCallback;
typedef std::function<void(std::vector<AVObject> const&, AVError const&)> AVVectorResultCallback;
typedef std::function<void(AVObject const&, AVError const&)> AVObjectResultCallback;
typedef std::function<void(AVUser const&, AVError const&)> AVUserResultCallback;


NS_AV_END

#endif  // INCLUDE_AVCONSTANTS_H_
