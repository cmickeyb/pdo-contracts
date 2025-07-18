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

#include <string>
#include <stddef.h>
#include <stdint.h>

#include "Dispatch.h"

#include "Cryptography.h"
#include "KeyValue.h"
#include "Environment.h"
#include "Message.h"
#include "Response.h"
#include "Types.h"
#include "Value.h"
#include "WasmExtensions.h"

#include "contract/attestation.h"
#include "contract/base.h"
#include "identity/committee.h"
#include "identity/identity.h"
#include "identity/policy_agent.h"

#include "identity/common/Credential.h"
#include "identity/common/Resolution.h"
#include "identity/common/ResolutionManager.h"
#include "identity/common/Committee.h"

static KeyValueStore committee_store("committee_store");
static KeyValueStore resolution_store("resolution_store");

const std::string unassigned_owner("__UNASSIGNED__");
const std::string initial_issuer_path("__ISSUER__");

#define ASSERT_COMMITTEE_IS_INITIALIZED(_rsp_)                            \
    do {                                                                  \
        std::string owner;                                                \
        ASSERT_SUCCESS(_rsp_, ww::contract::base::get_owner(owner),       \
                       "unexpected error: failed to retrieve the owner"); \
        if (owner != unassigned_owner)                                    \
            return _rsp_.error("committee has not been initialized");     \
    } while (0)

// -----------------------------------------------------------------
// METHOD: initialize_contract
//   contract initialization method
//
// JSON PARAMETERS:
//   none
//
// RETURNS:
//   true if successfully initialized
// -----------------------------------------------------------------
bool ww::identity::committee::initialize_contract(const Environment& env)
{
    // ---------- initialize the base contract ----------
    if (! ww::identity::policy_agent::initialize_contract(env))
        return false;

    // ---------- initialize the attestation contract ----------
    if (! ww::contract::attestation::initialize_contract(env))
        return false;

    // ---------- initialize the resolution store ----------
    ww::identity::ResolutionManager resolution_manager(resolution_store);
    if (! resolution_manager.initialize())
        return false;

    return true;
}

// -----------------------------------------------------------------
// METHOD: initialize_committee
//   Initializes the committee with the given parameters.
//
// JSON PARAMETERS:
//   COMMITTEE_INITIALIZE_PARAM_SCHEMA
//
// RETURNS:
//   true if the member was added successfully, false otherwise
// -----------------------------------------------------------------
bool ww::identity::committee::initialize_committee(const Message& msg, const Environment& env, Response& rsp)
{
    // this operation is performed by the owner to initialize the
    // committee. after this all operations are performed by the
    // committee rather than the owner.
    ASSERT_SENDER_IS_OWNER(env, rsp);
    ASSERT_INITIALIZED(rsp);

    ASSERT_SUCCESS(rsp, msg.validate_schema(COMMITTEE_INITIALIZE_COMMITTEE_PARAM_SCHEMA),
                   "invalid request, missing required parameters");

    // save the ledger key, this will be used later to verify that the
    // state of the contract has been committeed to the ledger
    const std::string ledger_verifying_key(msg.get_string("ledger_verifying_key"));
    ASSERT_SUCCESS(rsp, ww::contract::attestation::set_ledger_key(ledger_verifying_key),
                   "failed to save the ledger verifying key");

    // save the initial list of committee members
    ww::identity::Committee committee(committee_store);

    ww::value::Array member_array;
    ASSERT_SUCCESS(rsp, msg.get_value("initial_members", member_array),
                   "invalid request, unable to retrieve committee member");

    size_t num_members = member_array.get_count();
    ASSERT_SUCCESS(rsp, num_members > 0,
                   "invalid request, committee must have at least one member");

    std::vector<std::string> members;
    for (size_t i = 0; i < num_members; i++)
        ASSERT_SUCCESS(rsp, committee.add_member(member_array.get_string(i)),
                       "unexpected error: failed to add member ");

    // remove ownership of the committee object; first it prevents any
    // future re-initialization of the committee, and second it
    // allows the committee to operate independently of the owner
    ASSERT_SUCCESS(rsp, ww::contract::base::set_owner(unassigned_owner),
                   "unexpected error: failed to reassign ownership");

    return rsp.success(true);
}

// -----------------------------------------------------------------
// METHOD: add_member
//   Adds a new member to the committee. There must be an approved
//   member add resolution in place before this can be called.
//
// JSON PARAMETERS:
//   COMMITTEE_ADD_MEMBER_PARAM_SCHEMA
//
// RETURNS:
//   true if the member was added successfully, false otherwise
// -----------------------------------------------------------------
bool ww::identity::committee::add_member(const Message& msg, const Environment& env, Response& rsp)
{
    // ASSERT: Sender must be in the committee
    ASSERT_INITIALIZED(rsp);
    ASSERT_COMMITTEE_IS_INITIALIZED(rsp);

    ASSERT_SUCCESS(rsp, msg.validate_schema(COMMITTEE_ADD_MEMBER_PARAM_SCHEMA),
                   "invalid request, missing required parameters");

    ww::identity::Committee committee(committee_store);
    ASSERT_SUCCESS(rsp, committee.is_member(env.originator_id_),
                   "sender is not a member of the committee");

    // ---------- RETURN ----------
    return rsp.success(true);
}

// -----------------------------------------------------------------
// METHOD: remove_member
//   Removes a member from the committee. There must be an approved
//   member removal resolution in place before this can be called.
//
// JSON PARAMETERS:
//   COMMITTEE_REMOVE_MEMBER_PARAM_SCHEMA
//
// RETURNS:
//   true if the member was removed successfully, false otherwise
// -----------------------------------------------------------------
bool ww::identity::committee::remove_member(const Message& msg, const Environment& env, Response& rsp)
{
    // ASSERT: Sender must be in the committee
    ASSERT_INITIALIZED(rsp);
    ASSERT_COMMITTEE_IS_INITIALIZED(rsp);

    ASSERT_SUCCESS(rsp, msg.validate_schema(COMMITTEE_REMOVE_MEMBER_PARAM_SCHEMA),
                   "invalid request, missing required parameters");

    ww::identity::Committee committee(committee_store);
    ASSERT_SUCCESS(rsp, committee.is_member(env.originator_id_),
                   "sender is not a member of the committee");

    // ---------- RETURN ----------
    return rsp.success(true);
}

// -----------------------------------------------------------------
// METHOD: propose_resolution
//   Proposes a new resolution to be evaluated by the committee. The
//   resolution takes the form of a CREDENTIAL that contains the
//   claims that define the resolution.
//
// JSON PARAMETERS:
//   COMMITTEE_PROPOSE_RESOLUTION_PARAM_SCHEMA
//
// RETURNS:
//   COMMITTEE_PROPOSE_RESOLUTION_RETURN_SCHEMA
// -----------------------------------------------------------------
bool ww::identity::committee::propose_resolution(const Message& msg, const Environment& env, Response& rsp)
{
    // handle pre-conditions
    //
    // a resolution can be proposed by anyone (for the moment), we
    // may need something to prevent spam resolutions but that can
    // be addressed at a later time
    ASSERT_INITIALIZED(rsp);
    ASSERT_COMMITTEE_IS_INITIALIZED(rsp);

    ASSERT_SUCCESS(rsp, msg.validate_schema(COMMITTEE_PROPOSE_RESOLUTION_PARAM_SCHEMA),
                   "invalid request, missing required parameters");

    // Get and validate the credential parameter
    ww::value::Object credential;
    ASSERT_SUCCESS(rsp, msg.get_value("credential", credential),
                   "missing required parameter; credential");

    ww::identity::Credential resolution_credential;
    ASSERT_SUCCESS(rsp, resolution_credential.deserialize(credential),
                   "invalid credential");

    // Re-serializing the credential will ensure that the format contains
    // no additional information beyond the credential fields and provides
    // some consistent formatting
    std::string serialized_credential;
    ASSERT_SUCCESS(rsp, resolution_credential.serialize_string(serialized_credential),
                 "unable to serialize credential for signing");

    ww::identity::Resolution resolution(serialized_credential);
    ww::identity::ResolutionManager resolution_manager(resolution_store);
    ASSERT_SUCCESS(rsp, resolution_manager.add_resolution(resolution),
                   "unable to add proposed resolution");

    // ---------- RETURN ----------
    ww::value::Structure result(COMMITTEE_PROPOSE_RESOLUTION_RESULT_SCHEMA);
    ASSERT_SUCCESS(rsp, result.set_string("resolution_identifier", resolution.id_.c_str()),
                   "unable to add resolution ID to response");

    return rsp.value(result, true);
}

// -----------------------------------------------------------------
// METHOD: approve_resolution
//   Approves a proposed resolution. The sender must be a member of
//   the committee and the resolution must be in the proposed state.
//
// JSON PARAMETERS:
//   COMMITTEE_APPROVE_RESOLUTION_PARAM_SCHEMA
//
// RETURNS:
//   true if the resolution was approved successfully, false otherwise
// -----------------------------------------------------------------
bool ww::identity::committee::approve_resolution(const Message& msg, const Environment& env, Response& rsp)
{
    // check pre-conditions
    ASSERT_INITIALIZED(rsp);
    ASSERT_COMMITTEE_IS_INITIALIZED(rsp);

    ww::identity::Committee committee(committee_store);
    ASSERT_SUCCESS(rsp, committee.is_member(env.originator_id_),
                   "sender is not a member of the committee");

    // process parameters
    ASSERT_SUCCESS(rsp, msg.validate_schema(COMMITTEE_APPROVE_RESOLUTION_PARAM_SCHEMA),
                   "invalid request, missing required parameters");

    // pull the resolution ID from the message
    const char* resolution_id_param = msg.get_string("resolution_identifier");
    ASSERT_SUCCESS(rsp, resolution_id_param != nullptr,
                   "invalid request, missing required parameter: resolution_identitifer");

    const std::string resolution_id(resolution_id_param);

    ww::identity::Resolution resolution;
    ww::identity::ResolutionManager resolution_manager(resolution_store);
    ASSERT_SUCCESS(rsp, resolution_manager.get_resolution(resolution_id, resolution),
                   "unable to retrieve the resolution");

    // this can "fail" for a number of reasons, including the resolution
    // is no longer pending (voting is closed) or that the voter has already
    // registered a vote
    ASSERT_SUCCESS(rsp, resolution.approve(env.originator_id_),
                   "failed to record approval vote");

    // check to see if the resolution is now in the approved state and
    // update its status
    ASSERT_SUCCESS(rsp, committee_vote_function(committee, resolution),
                   "unexpected error: failed to check the committee vote function");

    // save the updated resolution
    ASSERT_SUCCESS(rsp, resolution_manager.update_resolution(resolution),
                   "unexpected error: unable to update the resolution status");

    // ---------- RETURN ----------
    return rsp.success(true);
}

// -----------------------------------------------------------------
// METHOD: disapprove_resolution
//   Disapproves a proposed resolution. The sender must be a member
//   of the committee and the resolution must be in the proposed state.
//
// JSON PARAMETERS:
//   COMMITTEE_DISAPPROVE_RESOLUTION_PARAM_SCHEMA
//
// RETURNS:
//   true if the resolution was disapproved successfully, false otherwise
// -----------------------------------------------------------------
bool ww::identity::committee::disapprove_resolution(const Message& msg, const Environment& env, Response& rsp)
{
    // handle pre-conditions
    ASSERT_INITIALIZED(rsp);
    ASSERT_COMMITTEE_IS_INITIALIZED(rsp);

    ww::identity::Committee committee(committee_store);
    ASSERT_SUCCESS(rsp, committee.is_member(env.originator_id_),
                   "sender is not a member of the committee");

    // process parameters
    ASSERT_SUCCESS(rsp, msg.validate_schema(COMMITTEE_DISAPPROVE_RESOLUTION_PARAM_SCHEMA),
                   "invalid request, missing required parameters");

    // pull the resolution ID from the message
    const char* resolution_id_param = msg.get_string("resolution_identifier");
    ASSERT_SUCCESS(rsp, resolution_id_param != nullptr,
                   "invalid request, missing required parameter: resolution_identitifer");

    const std::string resolution_id(resolution_id_param);

    ww::identity::Resolution resolution;
    ww::identity::ResolutionManager resolution_manager(resolution_store);
    ASSERT_SUCCESS(rsp, resolution_manager.get_resolution(resolution_id, resolution),
                   "unable to retrieve the resolution");

    // this can "fail" for a number of reasons, including the resolution
    // is no longer pending (voting is closed) or that the voter has already
    // registered a vote
    ASSERT_SUCCESS(rsp, resolution.disapprove(env.originator_id_),
                   "failed to record disapproval vote");

    // check to see if the resolution is now in the rejected state
    // and update its status
    ASSERT_SUCCESS(rsp, committee_vote_function(committee, resolution),
                   "unexpected error: failed to check the committee vote function");

    // save the updated resolution
    ASSERT_SUCCESS(rsp, resolution_manager.update_resolution(resolution),
                   "unexpected error: unable to update the resolution status");

    // ---------- RETURN ----------
    return rsp.success(true);
}

// -----------------------------------------------------------------
// METHOD: list_resolutions
//   Lists all resolutions that are currently in the proposed or
//   approved state. This is useful for querying the current
//   state of the committee.
//
// JSON PARAMETERS:
//   COMMITTEE_LIST_RESOLUTIONS_PARAM_SCHEMA
//
// RETURNS:
//   COMMITTEE_LIST_RESOLUTIONS_RETURN_SCHEMA
// -----------------------------------------------------------------
bool ww::identity::committee::list_resolutions(const Message& msg, const Environment& env, Response& rsp)
{
    // process pre-conditions
    //
    // for now we allow anyone to list resolutions, this may change
    // if, for example, resolutions are sensitive or confidential
    // in which case we would restrict this to committee members
    ASSERT_INITIALIZED(rsp);
    ASSERT_COMMITTEE_IS_INITIALIZED(rsp);

    // process parameters
    ASSERT_SUCCESS(rsp, msg.validate_schema(COMMITTEE_LIST_RESOLUTIONS_PARAM_SCHEMA),
                   "invalid request, missing required parameters");

    // get the list of resolutions from the resolution store
    std::vector<std::string> resolution_ids;
    ww::identity::ResolutionManager resolution_manager(resolution_store);
    ASSERT_SUCCESS(rsp, resolution_manager.list_resolutions(resolution_ids),
                   "unexpected error: unable to retrieve resolutions");

    // ---------- RETURN ----------
    ww::value::Array resolution_list;
    for (const auto& resolution_id : resolution_ids)
        resolution_list.append_string(resolution_id.c_str());

    ww::value::Structure result(COMMITTEE_LIST_RESOLUTIONS_RESULT_SCHEMA);
    ASSERT_SUCCESS(rsp, result.set_value("resolution_identifiers", resolution_list),
                   "unexpected error: unable to add resolution IDs to response");

    return rsp.value(result, false);
}

// -----------------------------------------------------------------
// METHOD: resolution_status
//   Get the status of a resolution. Provides a means of checking
//   whether a resolution is in the proposed, approved, or rejected.
//
// JSON PARAMETERS:
//   COMMITTEE_RESOLUTION_STATUS_PARAM_SCHEMA
//
// RETURNS:
//   COMMITTEE_RESOLUTION_STATUS_RETURN_SCHEMA
// -----------------------------------------------------------------
bool ww::identity::committee::resolution_status(const Message& msg, const Environment& env, Response& rsp)
{
    // handle pre-conditions
    //
    // for now we allow anyone to check the status of a resolution,
    // this seems reasonable since the resolution id is could be limited
    // to committee and proposer IF the resolution list is limited to
    // committee members
    ASSERT_INITIALIZED(rsp);
    ASSERT_COMMITTEE_IS_INITIALIZED(rsp);

    // process parameters
    ASSERT_SUCCESS(rsp, msg.validate_schema(COMMITTEE_RESOLUTION_STATUS_PARAM_SCHEMA),
                   "invalid request, missing required parameters");

    const char* resolution_id_param = msg.get_string("resolution_identifier");
    ASSERT_SUCCESS(rsp, resolution_id_param != nullptr,
                   "invalid request, missing required parameter: resolution_identitifer");

    const std::string resolution_id(resolution_id_param);

    // Retrieve the resolution from the store
    ww::identity::Resolution resolution;
    ww::identity::ResolutionManager resolution_manager(resolution_store);
    ASSERT_SUCCESS(rsp, resolution_manager.get_resolution(resolution_id, resolution),
                   "unable to retrieve the resolution");

    ww::identity::Credential resolution_credential;
    ASSERT_SUCCESS(rsp, resolution_credential.deserialize_string(resolution.serialized_credential_),
                   "unexpected error: unable to deserialize resolution credential");

    // ---------- RETURN ----------
    ww::value::Structure result(COMMITTEE_RESOLUTION_STATUS_RESULT_SCHEMA);
    ASSERT_SUCCESS(rsp, result.set_number("status", resolution.status_),
                   "unexpected error: unable to add resolution status to response");
    ASSERT_SUCCESS(rsp, result.set_string("resolution_identifier", resolution.id_.c_str()),
                   "unexpected error: unable to add resolution ID to response");

    ww::value::Value credential_value;
    ASSERT_SUCCESS(rsp, resolution_credential.serialize(credential_value),
                   "unexpected error: unable to serialize resolution credential");
    ASSERT_SUCCESS(rsp, result.set_value("credential", credential_value),
                   "unexpected error: unable to add resolution credential to response");

    return rsp.value(result, false);
}

// -----------------------------------------------------------------
// METHOD: issue_resolution
//   Issues a resolution that has been approved by the committee.
//   The resolution is issued as a VERIFIABLE_CREDENTIAL that can be
//   used to prove the resolution to other parties.
//
// JSON PARAMETERS:
//   COMMITTEE_ISSUE_RESOLUTION_PARAM_SCHEMA
//
// RETURNS:
//   VERIFIABLE_CREDENTIAL_SCHEMA
// -----------------------------------------------------------------
bool ww::identity::committee::issue_resolution_credential(
    const Message& msg, const Environment& env, Response& rsp)
{
    // handle pre-conditions
    ASSERT_INITIALIZED(rsp);
    ASSERT_COMMITTEE_IS_INITIALIZED(rsp);

    ww::identity::Committee committee(committee_store);
    ASSERT_SUCCESS(rsp, committee.is_member(env.originator_id_),
                   "sender is not a member of the committee");

    // process parameters
    ASSERT_SUCCESS(rsp, msg.validate_schema(COMMITTEE_ISSUE_RESOLUTION_CREDENTIAL_PARAM_SCHEMA),
                   "invalid request, missing required parameters");

    // ---------- Verify that the state has been committeed ----------

    // This state must be committed to ensure that the resulting credential
    // vote is valid and can be verified by the ledger.

    std::string ledger_key;
    if (! ww::contract::attestation::get_ledger_key(ledger_key) && ledger_key.length() > 0)
        return rsp.error("contract has not been initialized");

    const std::string ledger_signature(msg.get_string("ledger_signature"));

    ww::types::ByteArray buffer;
    std::copy(env.contract_id_.begin(), env.contract_id_.end(), std::back_inserter(buffer));
    std::copy(env.state_hash_.begin(), env.state_hash_.end(), std::back_inserter(buffer));

    ww::types::ByteArray signature;
    if (! ww::crypto::b64_decode(ledger_signature, signature))
        return rsp.error("failed to decode ledger signature");
    if (! ww::crypto::ecdsa::verify_signature(buffer, ledger_key, signature))
        return rsp.error("failed to verify ledger signature");

    // ---------- Verify that the resolution is approved ----------

    // Now we know that the state has been committed, we can check the
    // resolution status to ensure that it is in the approved state.
    const char* resolution_id_param = msg.get_string("resolution_identifier");
    ASSERT_SUCCESS(rsp, resolution_id_param != nullptr,
                   "invalid request, missing required parameter: resolution_identitifer");

    const std::string resolution_id(resolution_id_param);

    ww::identity::Resolution resolution;
    ww::identity::ResolutionManager resolution_manager(resolution_store);
    ASSERT_SUCCESS(rsp, resolution_manager.get_resolution(resolution_id, resolution),
                   "unable to retrieve the resolution");

    ASSERT_SUCCESS(rsp, resolution.status_ == ww::identity::ResolutionStatus::APPROVED,
                   "resolution is not in the approved state, cannot issue credential");

    // ---------- Generate the credential ----------
    ww::identity::Credential credential;
    ASSERT_SUCCESS(rsp, credential.deserialize_string(resolution.serialized_credential_),
                   "unexpected error: unable to deserialize resolution credential");

    ww::identity::VerifiableCredential vc;
    ASSERT_SUCCESS(rsp, ww::identity::policy_agent::issue_credential(resolution_id, env.contract_id_, credential, vc),
        "unexpected error: failed to create the new credential");

    ww::value::Object serialized_vc;
    ASSERT_SUCCESS(rsp, vc.serialize(serialized_vc),
                   "unexpected error: failed to serialized the credential");

    return rsp.value(serialized_vc, false);
}
