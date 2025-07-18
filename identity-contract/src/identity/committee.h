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

#include "Environment.h"
#include "Message.h"
#include "Response.h"
#include "Value.h"

#include "common/Committee.h"
#include "common/Credential.h"
#include "common/Resolution.h"
#include "common/VerifyingContext.h"

// Initialize the committee, the ledger verifying key is required to
// ensure that the state of the committee is committed to the ledger
// before issuing any approved resolutions
#define COMMITTEE_INITIALIZE_COMMITTEE_PARAM_SCHEMA     \
    "{"                                                 \
        SCHEMA_KW(ledger_verifying_key, "") ","         \
        SCHEMA_KW(initial_members, [ "" ])              \
    "}"

// Add member request is invoked when an add member resolution has
// been approved. The parameter is the resolution identifier for the
// approved resolution. There is no return value.
#define COMMITTEE_ADD_MEMBER_PARAM_SCHEMA       \
    "{"                                         \
        SCHEMA_KW(resolution_identifier, "")    \
    "}"

// Remove member request is invoked when a remove member resolution
// has been approved. The parameter is the resolution identifier for
// the approved resolution. There is no return value.
#define COMMITTEE_REMOVE_MEMBER_PARAM_SCHEMA    \
    "{"                                         \
        SCHEMA_KW(resolution_identifier, "")    \
    "}"

// Propose resolution request is invoked when a resolution
// is proposed for a committee vote. The resolution takes
// the form of a credential without verification. The result
// of the operation is a unique identifier for the resolution
// that can be used for future voting.
#define COMMITTEE_PROPOSE_RESOLUTION_PARAM_SCHEMA       \
    "{"                                                 \
        SCHEMA_KWS(credential, CREDENTIAL_SCHEMA)       \
    "}"

#define COMMITTEE_PROPOSE_RESOLUTION_RESULT_SCHEMA      \
    "{"                                                 \
        SCHEMA_KW(resolution_identifier, "")            \
    "}"

// Resolution status provides a means to look at a
// resolution and its voting status, the status returned
// will be one of the following: approved, disapproved,
// pending, or expired.
#define COMMITTEE_RESOLUTION_STATUS_PARAM_SCHEMA        \
    "{"                                                 \
        SCHEMA_KW(resolution_identifier, "")            \
    "}"

#define COMMITTEE_RESOLUTION_STATUS_RESULT_SCHEMA       \
    "{"                                                 \
        SCHEMA_KW(resolution_identifier, "") ","        \
        SCHEMA_KW(status, 0) ","                        \
        SCHEMA_KWS(credential, CREDENTIAL_SCHEMA)       \
    "}"


// Approve and disapprove resolution request represents a positive or
// negative vote for a resolution. If sufficient votes have been
// accrued then the resolution may be acted on (or a verifiable
// credential generated for the resolution.
#define COMMITTEE_APPROVE_RESOLUTION_PARAM_SCHEMA       \
    "{"                                                 \
        SCHEMA_KW(resolution_identifier, "")               \
    "}"

#define COMMITTEE_DISAPPROVE_RESOLUTION_PARAM_SCHEMA    \
    "{"                                                 \
        SCHEMA_KW(resolution_identifier, "")               \
    "}"

#define COMMITTEE_LIST_RESOLUTIONS_PARAM_SCHEMA \
    "{"                                         \
    "}"

#define COMMITTEE_LIST_RESOLUTIONS_RESULT_SCHEMA        \
    "{"                                                 \
        SCHEMA_KW(resolution_identifiers, [ "" ])       \
    "}"

// Issue resolution credential is invoked when a resolution has been
// approved. A verifiable credential is generated for the resolution.
// The current contract state must be committed to the ledger before
// the credential is issued. The ledger signature is used to ensure
// that the contract state is committed to the ledger.
#define COMMITTEE_ISSUE_RESOLUTION_CREDENTIAL_PARAM_SCHEMA      \
    "{"                                                         \
        SCHEMA_KW(ledger_signature,"") ","                      \
        SCHEMA_KW(resolution_identifier, "")                    \
    "}"

#define COMMITTEE_ISSUE_RESOLUTION_CREDENTIAL_RESULT_SCHEMA      \
    VERIFIABLE_CREDENTIAL_SCHEMA

namespace ww
{
namespace identity
{
namespace committee
{
    // Policy Agent contract methods
    bool initialize_contract(const Environment& env);

    bool initialize_committee(const Message& msg, const Environment& env, Response& rsp);

    bool add_member(const Message& msg, const Environment& env, Response& rsp);
    bool remove_member(const Message& msg, const Environment& env, Response& rsp);

    bool propose_resolution(const Message& msg, const Environment& env, Response& rsp);
    bool approve_resolution(const Message& msg, const Environment& env, Response& rsp);
    bool disapprove_resolution(const Message& msg, const Environment& env, Response& rsp);

    bool list_resolutions(const Message& msg, const Environment& env, Response& rsp);
    bool resolution_status(const Message& msg, const Environment& env, Response& rsp);

    bool issue_resolution_credential(const Message& msg, const Environment& env, Response& rsp);

    // This function must be defined by the contract, this is not a very clean way to do
    // this but WASM does not seem to support function pointers very well; the default
    // implmentation uses a simple majority vote.
    bool committee_vote_function(const ww::identity::Committee& committee, ww::identity::Resolution& resolution);
}; // policy_agent
}; // identity
}; // ww
