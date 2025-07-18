/* Copyright 2023 Intel Corporation
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

#include "KeyValue.h"
#include "Value.h"

#include "exchange/common/Common.h"
#include "identity/common/Resolution.h"
#include "identity/common/ResolutionManager.h"

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// Class: ww::identity::SigningContextManager
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
const std::string ww::identity::ResolutionManager::resolution_list_key_("__RESOLUTION_LIST__");

// -----------------------------------------------------------------
// initialize
//
// write an empty array for the resolution list, should be called
// only during contract initialization.
// -----------------------------------------------------------------
bool ww::identity::ResolutionManager::initialize(void)
{
    ww::value::Array resolution_list;
    std::string serialized_resolution_list;
    ERROR_IF_NOT(resolution_list.serialize(serialized_resolution_list),
                 "unexpected error: failed to save resolution list");

    // Save the root context to the persistent store
    return store_.set(resolution_list_key_, serialized_resolution_list);
}

// -----------------------------------------------------------------
// list_resolutions
//
// Retrieve a list of proposed resolutions. For now there is no
// filtering on the resolution status.
// -----------------------------------------------------------------
bool ww::identity::ResolutionManager::list_resolutions(
    std::vector<std::string>& resolution_list) const
{
    // check to see if the resolution is already in the list
    std::string serialized_resolution_list;
    ERROR_IF_NOT(store_.get(resolution_list_key_, serialized_resolution_list),
                 "unexpected error: failed to retrieve resolution list");

    ww::value::Array resolution_array;
    ERROR_IF_NOT(resolution_array.deserialize(serialized_resolution_list.c_str()),
                 "unexpected error: failed to deserialize resolution list");

    size_t count = resolution_array.get_count();
    for (size_t i = 0; i < count; i++)
    {
        const char* id = resolution_array.get_string(i);
        resolution_list.push_back(id);
    }

    return true;
}

// -----------------------------------------------------------------
// add_resolution
//
// Add a new resolution to the state. The resolution will be identified
// by its resolution identity. It will be added to the list and saved.
// Duplicates are rejected.
// -----------------------------------------------------------------
bool ww::identity::ResolutionManager::add_resolution(
    const ww::identity::Resolution& resolution) const
{
    // check to see if the resolution is already in the list
    std::string serialized_resolution_list;
    ERROR_IF_NOT(store_.get(resolution_list_key_, serialized_resolution_list),
                 "unexpected error: failed to retrieve resolution list");

    ww::value::Array resolution_list;
    ERROR_IF_NOT(resolution_list.deserialize(serialized_resolution_list.c_str()),
                 "unexpected error: failed to deserialize resolution list");

    size_t count = resolution_list.get_count();
    for (size_t i = 0; i < count; i++)
    {
        const char* id = resolution_list.get_string(i);
        ERROR_IF_NULL(id, "unexpected error: wrong type of resolution id");
        ERROR_IF(resolution.id_ == id, "duplicate resolution found");
    }

    // add the serialized resolution to the state
    std::string serialized_resolution;
    ERROR_IF_NOT(resolution.serialize_string(serialized_resolution),
                 "unexpected error: failed to serialize resolution");
    ERROR_IF_NOT(store_.set(resolution.id_, serialized_resolution),
                 "unexpected error: failed to save resolution");
    CONTRACT_SAFE_LOG(3, "stored resolution: %s", resolution.id_.c_str());

    // add the resolution id to the list
    ERROR_IF_NOT(resolution_list.append_string(resolution.id_.c_str()),
                 "unexpected error: failed to save resolution id");
    ERROR_IF_NOT(resolution_list.serialize(serialized_resolution_list),
                 "unexpected error: failed to serialize resolution list");
    ERROR_IF_NOT(store_.set(resolution_list_key_, serialized_resolution_list),
                 "unexpected error: failed to save resolution list");

    return true;
}

// -----------------------------------------------------------------
// update_resolution
//
// Update a resolution in the state. The resolution will be identified
// by its resolution identity.
// -----------------------------------------------------------------
bool ww::identity::ResolutionManager::update_resolution(
    const ww::identity::Resolution& resolution) const
{
    // add the serialized resolution to the state
    std::string serialized_resolution;
    ERROR_IF_NOT(resolution.serialize_string(serialized_resolution),
                 "unexpected error: failed to serialize resolution");
    ERROR_IF_NOT(store_.set(resolution.id_, serialized_resolution),
                 "unexpected error: failed to save resolution");

    return true;
}

// -----------------------------------------------------------------
// get_resolution
//
// Get a proposed resolution from the state.
// -----------------------------------------------------------------
bool ww::identity::ResolutionManager::get_resolution(
    const std::string& resolution_id,
    ww::identity::Resolution& resolution) const
{
    CONTRACT_SAFE_LOG(3, "get resolution: <%s>", resolution_id.c_str());

    // add the serialized resolution to the state
    std::string serialized_resolution;
    ERROR_IF_NOT(store_.get(resolution_id, serialized_resolution),
                 "failed to find resolution");
    ERROR_IF_NOT(resolution.deserialize_string(serialized_resolution),
                 "unexpected error: failed to deserialize resolution");

    return true;
}
