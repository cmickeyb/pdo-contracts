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

namespace ww
{
namespace identity
{
namespace credential
{
    // Credential base provides a base class for operational credentials and
    // a means of providing polymorphism
    class CredentialBase
    {
    protected:
        static bool verify_schema_actual(
            const ww::identity::Credential& credential,
            const char* type,
            const char* schema)
            {
                ERROR_IF_NOT(credential.credentialType_ == type, "invalid credential type");
                ERROF_IF_NOT(credential.credentialSubject_.claims_.validate_schema(schema),
                             "credential does not match expected schema");
                return true;
            };

    public:
        // the initialize method copies information from the credential
        // JSON object into the credential class
        bool initialize(const ww::identity::Credential& credential) = 0;

        // the finalize method copies information from the credential class
        // into the credential JSON object
        bool finalize(ww::identity::Credential& credential) = 0;
    };
}
