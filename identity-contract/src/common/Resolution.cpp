/* Copyright 2024 Intel Corporation
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

#include "exchange/common/Common.h"
#include "identity/common/Credential.h"
#include "identity/common/Resolution.h"
#include "identity/crypto/Crypto.h"

// -----------------------------------------------------------------
// -----------------------------------------------------------------
bool ww::identity::Resolution::deserialize(const ww::value::Object& serialized_object)
{
    if (! ww::identity::Resolution::verify_schema(serialized_object))
        return false;

    // Required fields
    const char *id = serialized_object.get_string("id");
    ERROR_IF_NULL(id, "unexpected error: missing 'id' in Resolution object");
    id_.assign(id);

    const char *serialized_credential = serialized_object.get_string("serialized_credential");
    ERROR_IF_NULL(serialized_credential, "unexpected error: missing 'serialized_credential' in Resolution object");
    serialized_credential_.assign(serialized_credential);

    double v = serialized_object.get_number("status");
    ERROR_IF(v < ww::identity::ResolutionStatus::PENDING || v > ww::identity::ResolutionStatus::EXPIRED,
             "unexpected error: invalid value for 'status' in Resolution object");
    status_ = static_cast<ww::identity::ResolutionStatus>(static_cast<int>(v));

    ww::value::Array positive_votes;
    serialized_object.get_value("positive_votes", positive_votes);
    size_t positive_votes_size = positive_votes.get_count();
    for (size_t i = 0; i < positive_votes_size; ++i)
    {
        const char *voter = positive_votes.get_string(i);
        ERROR_IF_NULL(voter, "unexpected error: missing value in 'positive_votes' array in Resolution object");
        positive_votes_.push_back(voter);
    }

    ww::value::Array negative_votes;
    serialized_object.get_value("negative_votes", negative_votes);
    size_t negative_votes_size = negative_votes.get_count();
    for (size_t i = 0; i < negative_votes_size; ++i)
    {
        const char *voter = negative_votes.get_string(i);
        ERROR_IF_NULL(voter, "unexpected error: missing value in 'negative_votes' array in Resolution object");
        negative_votes_.push_back(voter);
    }

    return true;
}

// -----------------------------------------------------------------
// -----------------------------------------------------------------
bool ww::identity::Resolution::serialize(ww::value::Value& serialized_object) const
{
    ww::value::Structure serializer(RESOLUTION_SCHEMA);

    // Required fields
    ERROR_IF_NOT(serializer.set_string("id", id_.c_str()),
                 "unexpected error: failed to set 'id' in Resolution object");
    ERROR_IF_NOT(serializer.set_string("serialized_credential", serialized_credential_.c_str()),
                 "unexpected error: failed to set 'serialized_credential' in Resolution object");
    ERROR_IF_NOT(serializer.set_number("status", static_cast<double>(status_)),
                 "unexpected error: failed to set 'status' in Resolution object");

    ww::value::Array positive_votes;
    for (const auto& voter : positive_votes_)
    {
        ERROR_IF_NOT(positive_votes.append_string(voter.c_str()),
                     "unexpected error: failed to add value to 'positive_votes' array in Resolution object");
    }
    serializer.set_value("positive_votes", positive_votes);

    ww::value::Array negative_votes;
    for (const auto& voter : negative_votes_)
    {
        ERROR_IF_NOT(negative_votes.append_string(voter.c_str()),
                     "unexpected error: failed to add value to 'negative_votes' array in Resolution object");
    }
    serializer.set_value("negative_votes", negative_votes);

    serialized_object.set(serializer);
    return true;
}

// -----------------------------------------------------------------
// -----------------------------------------------------------------
bool ww::identity::Resolution::approve(const std::string& committee_member_id)
{
    if (status_ != ww::identity::ResolutionStatus::PENDING)
        return false;

    // check for a duplicate approval
    if (std::find(positive_votes_.begin(), positive_votes_.end(), committee_member_id) != positive_votes_.end())
        return false; // already voted positively

    // check for a duplicate disapproval
    if (std::find(negative_votes_.begin(), negative_votes_.end(), committee_member_id) != negative_votes_.end())
        return false; // already voted negatively

    positive_votes_.push_back(committee_member_id);
    return true;
}

// -----------------------------------------------------------------
// -----------------------------------------------------------------
bool ww::identity::Resolution::disapprove(const std::string& committee_member_id)
{
    if (status_ != ww::identity::ResolutionStatus::PENDING)
        return false;

    // check for a duplicate approval
    if (std::find(positive_votes_.begin(), positive_votes_.end(), committee_member_id) != positive_votes_.end())
        return false; // already voted positively

    // check for a duplicate disapproval
    if (std::find(negative_votes_.begin(), negative_votes_.end(), committee_member_id) != negative_votes_.end())
        return false; // already voted negatively

    negative_votes_.push_back(committee_member_id);
    return true;
}

// -----------------------------------------------------------------
// constructor
//
// This constructor is used to create a new resolution from a
// serialized credential.  It hashes/base64 encodes the serialized
// credential to create a unique resolution id.
// -----------------------------------------------------------------
ww::identity::Resolution::Resolution(const std::string& serialized_credential)
{
    status_ = ww::identity::ResolutionStatus::PENDING;
    serialized_credential_ = serialized_credential;

    // Hash the encoded credential to create the resolution id
    ww::types::ByteArray serialized_credential_ba(serialized_credential.begin(), serialized_credential.end());
    ww::types::ByteArray resolution_id_ba;

    pdo_contracts::crypto::SHA256Hash(serialized_credential_ba, resolution_id_ba);
    ww::crypto::b64_encode(resolution_id_ba, id_);
}
