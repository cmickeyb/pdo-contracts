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

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// Class: ww::identity::Committee
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// -----------------------------------------------------------------
// -----------------------------------------------------------------

#include <string>
#include <vector>

#include "Value.h"

#include "exchange/common/Common.h"
#include "identity/common/Committee.h"

const std::string ww::identity::Committee::members_key("members");

// -----------------------------------------------------------------
// save
//
// Saves the current state of the committee to the KeyValueStore.
// -----------------------------------------------------------------
bool ww::identity::Committee::save(void) const
{
    ww::value::Array members_array;

    // Serialize the updated members list
    ww::value::Array value_obj;
    for (const auto& m : members_)
        value_obj.append_string(m.c_str());

    // Store the updated members list
    std::string serialized_array;
    if (! value_obj.serialize(serialized_array))
        return false;

    return store_.set(members_key, serialized_array);
}

// -----------------------------------------------------------------
// load
//
// Loads the committee members from the KeyValueStore.
// -----------------------------------------------------------------
bool ww::identity::Committee::load(void)
{
    std::string serialized_array;
    if (!store_.get(members_key, serialized_array))
        return false;

    ww::value::Array member_array;
    if (!member_array.deserialize(serialized_array.c_str()))
        return false;

    members_.clear();

    size_t num_members = member_array.get_count();
    for (size_t i = 0; i < num_members; i++)
        members_.push_back(member_array.get_string(i));

    return true;
}

// -----------------------------------------------------------------
// is_member
//
// Checks if a given member is part of the committee.
// -----------------------------------------------------------------
bool ww::identity::Committee::is_member(const std::string& member) const
{
    return std::find(members_.begin(), members_.end(), member) != members_.end();
}

// -----------------------------------------------------------------
// add_member
//
// Adds a member to the committee if they do not already exist.
// Saves the updated committee to the store.
// -----------------------------------------------------------------
bool ww::identity::Committee::add_member(const std::string& member)
{
    if (std::find(members_.begin(), members_.end(), member) == members_.end())
    {
        members_.push_back(member);
        return save();
    }

    return false;
}

// -----------------------------------------------------------------
// remove_member
//
// Removes a member from the committee if they exist. Saves the
// updated committee to the store.
// -----------------------------------------------------------------
bool ww::identity::Committee::remove_member(const std::string& member)
{
    auto it = std::find(members_.begin(), members_.end(), member);
    if (it != members_.end())
    {
        members_.erase(it);
        return save();
    }

    return false;
}
