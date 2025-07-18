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

#include "Util.h"
#include "Value.h"

#include "exchange/common/Common.h"

#define RESOLUTION_SCHEMA                       \
    "{"                                         \
        SCHEMA_KW(id, "") ","                   \
        SCHEMA_KW(status, 0) ","                \
        SCHEMA_KW(positive_votes, [ "" ]) ","   \
        SCHEMA_KW(negative_votes, [ "" ]) ","   \
        SCHEMA_KW(serialized_credential, "")    \
    "}"

namespace ww
{
namespace identity
{
    enum ResolutionStatus
    {
        PENDING = 0,
        APPROVED = 1,
        REJECTED = 2,
        EXPIRED = 3,
    };

    class Resolution : public ww::exchange::SerializeableObject
    {
    private:
    public:
        std::string id_;        /* resolution identifier, base64 encoded hash of the serialized credential */
        std::string serialized_credential_; /* base64 encoded credential */

        ww::identity::ResolutionStatus status_;

        std::vector<std::string> positive_votes_; /* committee member ids with positive votes */
        std::vector<std::string> negative_votes_; /* committee member ids with negative votes */

        static bool verify_schema(const ww::value::Object& deserialized_object)
        {
            return ww::exchange::SerializeableObject::verify_schema_actual(
                deserialized_object, RESOLUTION_SCHEMA);
        };

        bool deserialize(const ww::value::Object& credential);
        bool serialize(ww::value::Value& serialized_credential) const;

        bool approve(const std::string& committee_member_id);
        bool disapprove(const std::string& committee_member_id);

        Resolution(void) : status_(ResolutionStatus::PENDING) {};
        Resolution(const std::string& serialized_credential);
    };
}
}
