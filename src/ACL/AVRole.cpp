/**
 * @file   AVRole.cpp
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Thu Sep 18 17:38:44 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#include "ACL/AVRole.h"

#include <string>
#include <vector>

#include "ACL/AVACL.h"
#include "AVObject/AVRelation.h"
#include "Query/AVQuery.h"

NS_AV_BEGIN

AVRole::AVRole():name() {
  this->className = "_Role";
}

AVRole* AVRole::roleWithName(std::string name) {
  AVRole* role = new AVRole();
  role->name = name;

  return role;
}

AVRole* AVRole::roleWithNameAndACL(std::string name, AVACL* acl) {
  AVRole* role = new AVRole();
  role->name = name;
  role->setACL(acl);

  return role;
}

void AVRole::release() {
  AV_SAFE_DELETE(this);
}

AVRelation* AVRole::users() {
  std::string usersTag = "users";
  std::vector<AVObject*> objects = this->addedRelationData[usersTag];
  AVObject* target = nullptr;

  if (objects.size() > 0) {
    target = objects[0];
  }

  AVRelation* relation = new AVRelation();
  relation->parent = this;
  relation->key = usersTag;

  if (target != nullptr) {
    relation->targetClass = target->className;
  }

  return relation;
}

AVRelation* AVRole::roles() {
  return nullptr;
}

AVQuery* AVRole::query() {
  AVQuery* query = AVQuery::queryWithClassName("_Role");
  return query;
}

NS_AV_END
