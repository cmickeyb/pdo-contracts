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

#include "KeyValue.h"
#include "exchange/common/Common.h"

namespace ww
{
namespace identity
{
    class Committee
    {
    protected:
        static const std::string members_key;
        KeyValueStore store_;

        std::vector<std::string> members_;

    public:
        bool save(void) const;
        bool load(void);

        bool is_member(const std::string& member) const;
        bool add_member(const std::string& member);
        bool remove_member(const std::string& member);

        bool get_members(std::vector<std::string>& members) const { members = members_; return true; };

        Committee(const KeyValueStore& store) : store_(store) { load(); };
    };
}
}
