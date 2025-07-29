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

#pragma once
#include <string>
#include <vector>

#include "Types.h"
#include "Util.h"
#include "Value.h"

#include "exchange/common/Common.h"
#include "identity/common/Credential.h"
#include "identity/credential/Base.h"

#define MEMBERSHIP_ADD_MEMBER_TYPE_NAME "AddMemberCredential"
#define MEMBERSHIP_ADD_MEMBER_SCHEMA            \
    "{"                                         \
        SCHEMA_KW(publicKey, "")                \
    "}"

#define MEMBERSHIP_REMOVE_MEMBER_TYPE_NAME "RemoveMemberCredential"
#define MEMBERSHIP_REMOVE_MEMBER_SCHEMA         \
    "{"                                         \
        SCHEMA_KW(publicKey, "")                \
    "}"

namespace ww
{
namespace identity
{
namespace credential
{
    // Define the AddMember credential used in the committee contract to
    // add members to the committee
    class AddMember : CredentialBase
    {
    public:

        std::string publicKey_;

        static bool verify_schema(const ww::identity::Credential& credential)
        {
            return verify_schema_actual(credential, MEMBERSHIP_ADD_MEMBER_TYPE_NAME, MEMBERSHIP_ADD_MEMBER_SCHEMA);
        }

        bool initialize(const ww::identity::Credential& credential);
        bool finalize(const ww::identity::Credential& credential);
    };

    // Define the RemoveMember credential used in the committee contract to
    // remove members from the committee
    class RemoveMember
    {
    protected:
        static const std::string credential_type_name;

    public:

        std::string publicKey_;

        static bool verify_schema(const ww::identity::Credential& credential)
        {
            return verify_schema_actual(credential, MEMBERSHIP_REMOVE_MEMBER_TYPE_NAME, MEMBERSHIP_REMOVE_MEMBER_SCHEMA);
        }

        bool initialize(const ww::identity::Credential& credential);
        bool finalize(const ww::identity::Credential& credential);
    };


}
