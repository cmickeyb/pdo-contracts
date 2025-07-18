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

#pragma once
#include <string>
#include <vector>

#include "KeyValue.h"

#include "identity/common/Resolution.h"

namespace ww
{
namespace identity
{
    class ResolutionManager
    {
    private:
        static const std::string resolution_list_key_;

        KeyValueStore store_;

    public:
        bool list_resolutions(std::vector<std::string>& resolution_list) const;
        bool add_resolution(const ww::identity::Resolution& resolution) const;
        bool update_resolution(const ww::identity::Resolution& resolution) const;
        bool get_resolution(const std::string& resolution_id, ww::identity::Resolution& resolution) const;

        bool initialize(void);

        ResolutionManager(const KeyValueStore& store) : store_(store) {};
    };

} /* identity */
} /* ww */
