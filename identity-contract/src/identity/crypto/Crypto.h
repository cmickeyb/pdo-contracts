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

#include "Types.h"
#include "identity/crypto/CryptoInternal.h"

#include <memory>

typedef bool (*HashFunctionType)(const ww::types::ByteArray& message, ww::types::ByteArray& hash);

namespace pdo_contracts
{
namespace crypto
{
    const unsigned int PBDK_Iterations = 10000;

    bool SHA256Hash(
        const ww::types::ByteArray& message, ww::types::ByteArray& hash);
    bool SHA256HMAC(
        const ww::types::ByteArray& message, const ww::types::ByteArray& key, ww::types::ByteArray& hmac);

    bool SHA384Hash(
        const ww::types::ByteArray& message, ww::types::ByteArray& hash);
    bool SHA384HMAC(
        const ww::types::ByteArray& message, const ww::types::ByteArray& key, ww::types::ByteArray& hmac);

    bool SHA512Hash(
        const ww::types::ByteArray& message, ww::types::ByteArray& hash);
    bool SHA512HMAC(
        const ww::types::ByteArray& message, const ww::types::ByteArray& key, ww::types::ByteArray& hmac);

    bool SHA512PasswordBasedKeyDerivation(
        const std::string& password, const ww::types::ByteArray& salt, ww::types::ByteArray& hmac);
}
}
