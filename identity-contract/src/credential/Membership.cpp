/* Copyright 2025 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>
#include <vector>

#include "Types.h"
#include "Value.h"
#include "WasmExtensions.h"

#include "exchange/common/Common.h"
#include "identity/common/Credential.h"
#include "identity/common/Membership.h"

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// Class: ww::identity::AddMemberCredential
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
bool ww::identity::credential::AddMember::initialize(const ww::value::Credentail& credential)
{
    ERROR_IF_NOT(verify_schema(credential), "invalid add_membership credential claims schema");

    const char* key = credential.credentialSubject_.claims_.get_string("publicKey");
    ERROR_IF(key == nullptr, "unexpected error: failed to retrieve public key");

    publicKey_.assign(key);
    return true;
}

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// Class: ww::identity::AddMemberCredential
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
bool ww::identity::credential::AddMember::finalize(const ww::value::Credentail& credential)
{
    credential.credentialType_ = MEMBERSHIP_ADD_MEMBER_TYPE_NAME;

    ww::value::Structure claims(MEMBERSHIP_ADD_MEMBER_SCHEMA);
    claims.set_string("publicKey", publicKey_);

    credential.credentialSubject_.claims_.set(claims);
    return true;
}


// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// Class: ww::identity::RemoveMemberCredential
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
bool ww::identity::RemoveMemberCredential::initialize(const ww::value::Credentail& credential)
{
    ERROR_IF_NOT(credential.credentialType_ == credential_type_name,
                 "invalid add membership credential");
    ERROF_IF_NOT(credential.credentialSubject_.claims_.validate_schema(MEMBERSHIP_REMOVE_MEMBER_SCHEMA),
                 "invalid add membership credential claims schema");

    const char* key = credential.credentialSubject_.claims_.get_string("publicKey");
    ERROR_IF(key == nullptr, "unexpected error: failed to retrieve public key");

    publicKey_.assign(key);
    return true;
}
