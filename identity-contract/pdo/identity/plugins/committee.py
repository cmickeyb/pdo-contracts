# Copyright 2023 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import json
import logging

from pdo.contract import invocation_request
from pdo.submitter.create import create_submitter

import pdo.client.builder as pbuilder
import pdo.client.builder.command as pcommand
import pdo.client.builder.contract as pcontract
import pdo.client.builder.shell as pshell
import pdo.client.commands.contract as pcontract_cmd
import pdo.common.utility as putils

import pdo.identity.plugins.identity as identity_plugin
import pdo.identity.plugins.signature_authority as signature_plugin

__all__ = [
    'op_initialize',
    'op_get_verifying_key',
    'op_get_extended_verifying_key',
    'op_verify_credential',

    'op_initialize_committee',
    'op_add_member',
    'op_remove_member',
    'op_propose_resolution',
    'op_approve_resolution',
    'op_disapprove_resolution',
    'op_list_resolutions',
    'op_resolution_status',
    'op_issue_resolution_credential',

    'cmd_verify_credential',
    'cmd_get_verifying_key',

    'cmd_propose_resolution',
    'cmd_vote_on_resolution',
    'cmd_list_resolutions',
    'cmd_get_resolution',
    'cmd_issue_resolution_credential',
    'cmd_create_committee',

    'do_committee',
    'do_committee_contract',
    'load_commands',
]

op_initialize = identity_plugin.op_initialize
op_get_verifying_key = identity_plugin.op_get_verifying_key
op_get_extended_verifying_key = identity_plugin.op_get_extended_verifying_key
op_verify_credential = signature_plugin.op_verify_credential

cmd_verify_credential = signature_plugin.cmd_verify_credential
cmd_get_verifying_key = identity_plugin.cmd_get_verifying_key

logger = logging.getLogger(__name__)

__status_enum__ = ['PENDING', 'APPROVED', 'DISAPPROVED', 'EXPIRED']

# -----------------------------------------------------------------
# -----------------------------------------------------------------
class op_initialize_committee(pcontract.contract_op_base) :

    name = "initialize_committee"
    help = "Initialize a committee"

    @classmethod
    def add_arguments(cls, subparser) :
        subparser.add_argument(
            '-m', '--members',
            help='List of committee members to initialize the committee with',
            type=pbuilder.invocation_parameter,
            nargs='+',
            required=True)

        subparser.add_argument(
            '-l', '--ledger-key',
            help='ledger verifying key',
            type=pbuilder.invocation_parameter,
            required=True)

    @classmethod
    def invoke(cls, state, session_params, ledger_key, members, **kwargs) :
        session_params['commit'] = True

        params = {
            'ledger_verifying_key' : ledger_key,
            'initial_members' : members,
        }

        message = invocation_request('initialize_committee', **params)
        result = pcontract_cmd.send_to_contract(state, message, **session_params)
        cls.log_invocation(message, result)

        return result

# -----------------------------------------------------------------
# -----------------------------------------------------------------
class op_add_member(pcontract.contract_op_base) :

    name = "add_member"
    help = "Add a committee member based on an approved membership change resolution"

    @classmethod
    def add_arguments(cls, subparser) :
        subparser.add_argument(
            '-l', '--ledger-attestation',
            help='attestation from the ledger that the current state of the committee is committed',
            type=pbuilder.invocation_parameter,
            required=True)

        subparser.add_argument(
            '-r', '--resolution-id',
            help='The ID of the resolution to issue a credential for',
            type=str,
            required=True)

    @classmethod
    def invoke(cls, state, session_params, ledger_attestation, resolution_id, **kwargs) :
        session_params['commit'] = True

        params = {
            'ledger_signature' : ledger_attestation,
            'resolution_identifier' : resolution_id,
        }

        message = invocation_request('add_member', **params)
        result = pcontract_cmd.send_to_contract(state, message, **session_params)
        cls.log_invocation(message, result)

        return result

# -----------------------------------------------------------------
# -----------------------------------------------------------------
class op_remove_member(pcontract.contract_op_base) :

    name = "remove_member"
    help = "Remove a committee member based on an approved membership change resolution"

    @classmethod
    def add_arguments(cls, subparser) :
        subparser.add_argument(
            '-l', '--ledger-attestation',
            help='attestation from the ledger that the current state of the committee is committed',
            type=pbuilder.invocation_parameter,
            required=True)

        subparser.add_argument(
            '-r', '--resolution-id',
            help='The ID of the resolution to issue a credential for',
            type=str,
            required=True)

    @classmethod
    def invoke(cls, state, session_params, ledger_attestation, resolution_id, **kwargs) :
        session_params['commit'] = True

        params = {
            'ledger_signature' : ledger_attestation,
            'resolution_identifier' : resolution_id,
        }

        message = invocation_request('remove_member', **params)
        result = pcontract_cmd.send_to_contract(state, message, **session_params)
        cls.log_invocation(message, result)

        return result

# -----------------------------------------------------------------
# -----------------------------------------------------------------
class op_propose_resolution(pcontract.contract_op_base) :

    name = "propose_resolution"
    help = "Propose a new resolution"

    @classmethod
    def add_arguments(cls, subparser) :
        subparser.add_argument(
            '-c', '--credential',
            help='The credential to sign (JSON)',
            type=pbuilder.invocation_parameter,
            required=True)

    @classmethod
    def invoke(cls, state, session_params, credential, **kwargs) :
        session_params['commit'] = True

        params = {
            'credential' : credential,
        }

        message = invocation_request('propose_resolution', **params)
        result = pcontract_cmd.send_to_contract(state, message, **session_params)
        cls.log_invocation(message, result)

        return result

# -----------------------------------------------------------------
# -----------------------------------------------------------------
class op_approve_resolution(pcontract.contract_op_base) :

    name = "approve_resolution"
    help = "Record an approval for a proposed resolution"

    @classmethod
    def add_arguments(cls, subparser) :
        subparser.add_argument(
            '-r', '--resolution-id',
            help='The ID of the resolution to issue a credential for',
            type=str,
            required=True)

    @classmethod
    def invoke(cls, state, session_params, resolution_id, **kwargs) :
        session_params['commit'] = True

        params = {
            'resolution_identifier' : resolution_id,
        }

        message = invocation_request('approve_resolution', **params)
        result = pcontract_cmd.send_to_contract(state, message, **session_params)
        cls.log_invocation(message, result)

        return result

# -----------------------------------------------------------------
# -----------------------------------------------------------------
class op_disapprove_resolution(pcontract.contract_op_base) :

    name = "disapprove_resolution"
    help = "Record a disapproval vote for a resolution"

    @classmethod
    def add_arguments(cls, subparser) :
        subparser.add_argument(
            '-r', '--resolution-id',
            help='The ID of the resolution to issue a credential for',
            type=str,
            required=True)

    @classmethod
    def invoke(cls, state, session_params, resolution_id, **kwargs) :
        session_params['commit'] = True

        params = {
            'resolution_identifier' : resolution_id,
        }

        message = invocation_request('disapprove_resolution', **params)
        result = pcontract_cmd.send_to_contract(state, message, **session_params)
        cls.log_invocation(message, result)

        return result

# -----------------------------------------------------------------
# -----------------------------------------------------------------
class op_list_resolutions(pcontract.contract_op_base) :

    name = "list_resolutions"
    help = "Get a list of all resolutions proposed to the committee"

    @classmethod
    def add_arguments(cls, subparser) :
        pass

    @classmethod
    def invoke(cls, state, session_params, **kwargs) :
        session_params['commit'] = False

        params = {}

        message = invocation_request('list_resolutions', **params)
        result = pcontract_cmd.send_to_contract(state, message, **session_params)
        cls.log_invocation(message, result)

        return result

# -----------------------------------------------------------------
# -----------------------------------------------------------------
class op_resolution_status(pcontract.contract_op_base) :

    name = "resolution_status"
    help = "Check the status of a proposed resolution"

    @classmethod
    def add_arguments(cls, subparser) :
        subparser.add_argument(
            '-r', '--resolution-id',
            help='The ID of the resolution to issue a credential for',
            type=str,
            required=True)

    @classmethod
    def invoke(cls, state, session_params, resolution_id, **kwargs) :
        session_params['commit'] = False

        params = {
            'resolution_identifier' : resolution_id,
        }

        message = invocation_request('resolution_status', **params)
        result = pcontract_cmd.send_to_contract(state, message, **session_params)
        cls.log_invocation(message, result)

        return result


# -----------------------------------------------------------------
# -----------------------------------------------------------------
class op_issue_resolution_credential(pcontract.contract_op_base) :

    name = "issue_resolution_credential"
    help = "Issue a verifiable credential for an approved resolution"

    @classmethod
    def add_arguments(cls, subparser) :
        subparser.add_argument(
            '-l', '--ledger-attestation',
            help='attestation from the ledger that the current state of the committee is committed',
            type=pbuilder.invocation_parameter,
            required=True)

        subparser.add_argument(
            '-r', '--resolution-id',
            help='The ID of the resolution to issue a credential for',
            type=str,
            required=True)

    @classmethod
    def invoke(cls, state, session_params, ledger_attestation, resolution_id, **kwargs) :
        session_params['commit'] = False

        params = {
            'ledger_signature' : ledger_attestation,
            'resolution_identifier' : resolution_id
        }

        message = invocation_request('issue_resolution_credential', **params)
        result = pcontract_cmd.send_to_contract(state, message, **session_params)
        cls.log_invocation(message, result)

        return result

# XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
# XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

# -----------------------------------------------------------------
# -----------------------------------------------------------------
class cmd_propose_resolution(pcommand.contract_command_base) :
    name = "propose_resolution"
    help = "Propose a new resolution for committee approval"

    @classmethod
    def add_arguments(cls, subparser) :
        subparser.add_argument(
            '-c', '--credential',
            help='The name of the file containing the proposed resolution',
            type=str,
            required=True)

    @classmethod
    def invoke(cls, state, context, credential, **kwargs) :
        save_file = pcontract_cmd.get_contract_from_context(state, context)
        if not save_file :
            raise ValueError('signature authority contract must be created and initialized')

        # read the resolution from the credential file
        with open(credential, "r") as fp :
            credential_data = json.load(fp)

        # and invoke the contract operation to propose the resolution
        session = pbuilder.SessionParameters(save_file=save_file)
        results = pcontract.invoke_contract_op(
            op_propose_resolution,
            state, context, session,
            credential=credential_data,
            **kwargs)

        results = json.loads(results)
        cls.display(results['resolution_identifier'])

        return True

# -----------------------------------------------------------------
# -----------------------------------------------------------------
class cmd_vote_on_resolution(pcommand.contract_command_base) :
    name = "vote"
    help = "Register a vote on a proposed resolution"

    @classmethod
    def add_arguments(cls, subparser) :
        subparser.add_argument(
            '-r', '--resolution-id',
            help='The ID of the resolution to issue a credential for',
            type=str,
            required=True)
        subparser.add_argument(
            '--approve',
            help='Approve the resolution',
            action='store_true')
        subparser.add_argument(
            '--disapprove',
            help='Disapprove the resolution',
            action='store_false',
            dest='approve')

    @classmethod
    def invoke(cls, state, context, approve, resolution_id, **kwargs) :
        save_file = pcontract_cmd.get_contract_from_context(state, context)
        if not save_file :
            raise ValueError('signature authority contract must be created and initialized')

        operation = op_approve_resolution if approve else op_disapprove_resolution

        # and invoke the contract operation to propose the resolution
        session = pbuilder.SessionParameters(save_file=save_file)
        resolution_id = pcontract.invoke_contract_op(
            operation,
            state, context, session,
            resolution_id=resolution_id,
            **kwargs)

        cls.display('registered {} vote for resolution {}'.format('approval' if approve else 'disapproval', resolution_id))
        return True

# -----------------------------------------------------------------
# -----------------------------------------------------------------
class cmd_list_resolutions(pcommand.contract_command_base) :
    name = "list_resolutions"
    help = "List resolutions proposed to the committee"

    @classmethod
    def add_arguments(cls, subparser) :
        subparser.add_argument(
            '-r', '--resolution-id',
            help='The ID of the resolution to issue a credential for',
            type=str,
            dest='resolution_ids',
            nargs='+')

    @classmethod
    def invoke(cls, state, context, resolution_ids, **kwargs) :
        save_file = pcontract_cmd.get_contract_from_context(state, context)
        if not save_file :
            raise ValueError('signature authority contract must be created and initialized')

        # and invoke the contract operation to list the resolutions
        session = pbuilder.SessionParameters(save_file=save_file)
        if not resolution_ids :
            result = pcontract.invoke_contract_op(
                op_list_resolutions,
                state, context, session,
                **kwargs)

            result = json.loads(result)
            resolution_ids = result['resolution_identifiers']

        cls.display("Proposed Resolutions:")
        for resolution_id in resolution_ids :
            resolution_data = pcontract.invoke_contract_op(
                op_resolution_status,
                state, context, session,
                resolution_id=resolution_id,
                **kwargs)

            resolution_data = json.loads(resolution_data)
            status = __status_enum__[resolution_data["status"]]
            cls.display(f'\t{resolution_id}\t{status}')

        return True

# -----------------------------------------------------------------
# -----------------------------------------------------------------
class cmd_get_resolution(pcommand.contract_command_base) :
    name = "get_resolution"
    help = "Get the credential associates with a proposed resolution"

    @classmethod
    def add_arguments(cls, subparser) :
        subparser.add_argument(
            '--credential',
            help='The name of the file where the proposed resolution will be stored',
            type=str,
            dest='credential_file',
            required=True)

        subparser.add_argument(
            '-r', '--resolution-id',
            help='The ID of the resolution to issue a credential for',
            type=str,
            required=True)

    @classmethod
    def invoke(cls, state, context, credential_file, resolution_id, **kwargs) :
        save_file = pcontract_cmd.get_contract_from_context(state, context)
        if not save_file :
            raise ValueError('signature authority contract must be created and initialized')

        # and invoke the contract operation to list the resolutions
        session = pbuilder.SessionParameters(save_file=save_file)
        resolution_data = pcontract.invoke_contract_op(
            op_resolution_status,
            state, context, session,
            resolution_id=resolution_id,
            **kwargs)

        resolution_data = json.loads(resolution_data)
        credential = resolution_data['credential']

        with open(credential_file, "w") as fp :
            json.dump(credential, fp, indent=4)

        status = __status_enum__[resolution_data['status']]
        cls.display(f'saved resolution {resolution_id} with status {status} to {credential_file}')

        return True

# -----------------------------------------------------------------
# -----------------------------------------------------------------
class cmd_issue_resolution_credential(pcommand.contract_command_base) :
    name = "issue_credential"
    help = "Issue a credential for an approved resolution"

    @classmethod
    def add_arguments(cls, subparser) :
        subparser.add_argument(
            '--issued-credential',
            help='The name of the file where the issued credential will be stored',
            type=str,
            required=True)

        subparser.add_argument(
            '-r', '--resolution-id',
            help='The ID of the resolution to issue a credential for',
            type=str,
            required=True)

    @classmethod
    def invoke(cls, state, context, issued_credential, resolution_id, **kwargs) :
        save_file = pcontract_cmd.get_contract_from_context(state, context)
        if not save_file :
            raise ValueError('committee contract must be created and initialized')

        session = pbuilder.SessionParameters(save_file=save_file)
        contract_object = pcontract_cmd.get_contract(state, save_file)

        # get all the information necessary to register this contract as an endpoint with the guardian
        ledger_submitter = create_submitter(state.get(['Ledger']))
        ledger_attestation = ledger_submitter.get_current_state_hash(contract_object.contract_id)
        print("ledger attestation: {}".format(ledger_attestation))

        issued_credential_data = pcontract.invoke_contract_op(
            op_issue_resolution_credential,
            state, context, session,
            ledger_attestation=ledger_attestation['signature'],
            resolution_id=resolution_id,
            **kwargs)

        with open(issued_credential, "w") as fp :
            fp.write(issued_credential_data)

        cls.display('saved issued credential to {}'.format(issued_credential))
        return True

# -----------------------------------------------------------------
# -----------------------------------------------------------------
class cmd_create_committee(pcommand.contract_command_base) :
    name = "create"
    help = "script to create a new policy agent"

    @classmethod
    def add_arguments(cls, subparser) :
        subparser.add_argument('-c', '--contract-class', help='Name of the contract class', type=str)
        subparser.add_argument('-e', '--eservice-group', help='Name of the enclave service group to use', type=str)
        subparser.add_argument('-f', '--save-file', help='File where contract data is stored', type=str)
        subparser.add_argument('-p', '--pservice-group', help='Name of the provisioning service group to use', type=str)
        subparser.add_argument('-r', '--sservice-group', help='Name of the storage service group to use', type=str)
        subparser.add_argument('--source', help='File that contains contract source code', type=str)
        subparser.add_argument('--extra', help='Extra data associated with the contract file', nargs=2, action='append')

        subparser.add_argument(
            '-d', '--description',
            help='Description of the asset described by the identity',
            type=str,
            required=True)

        subparser.add_argument(
            '--members',
            help='List of committee members to initialize the committee with, key files must be in the search path',
            type=pbuilder.invocation_parameter,
            nargs='+',
            required=True)

    @classmethod
    def invoke(cls, state, context, description, members, **kwargs) :
        save_file = pcontract_cmd.get_contract_from_context(state, context)
        if save_file :
            return save_file

        # get the keys for the initial committee members
        keypath = state.get(['Key', 'SearchPath'])

        member_keys = []
        for member in members :
            keyfile = putils.find_file_in_path("{0}_public.pem".format(member), keypath)
            with open (keyfile, "r") as myfile:
                verifying_key = myfile.read()
            member_keys.append(verifying_key)

        # create the policy agent contract
        save_file = pcontract_cmd.create_contract_from_context(state, context, 'policy_agent', **kwargs)
        context['save_file'] = save_file

        session = pbuilder.SessionParameters(save_file=save_file)
        contract_object = pcontract_cmd.get_contract(state, save_file)

        # get all the information necessary to register this contract as an endpoint with the guardian
        ledger_submitter = create_submitter(state.get(['Ledger']))
        ledger_key = ledger_submitter.get_ledger_info()

        # initialize the identity portion of the contract object
        pcontract.invoke_contract_op(
            op_initialize,
            state, context, session,
            description,
            **kwargs)

        # initialize the committee portion of the contract object
        pcontract.invoke_contract_op(
            op_initialize_committee,
            state, context, session,
            ledger_key=ledger_key,
            members=member_keys,
            **kwargs)

        cls.display('created committee in {}'.format(save_file))
        return save_file

# -----------------------------------------------------------------
# Create the generic, shell independent version of the aggregate command
# -----------------------------------------------------------------
__operations__ = [
    op_initialize,
    op_get_verifying_key,
    op_get_extended_verifying_key,
    op_verify_credential,
    op_initialize_committee,
    op_add_member,
    op_remove_member,
    op_propose_resolution,
    op_approve_resolution,
    op_disapprove_resolution,
    op_list_resolutions,
    op_resolution_status,
    op_issue_resolution_credential,
]

do_committee_contract = pcontract.create_shell_command('committee_contract', __operations__)

__commands__ = [
    cmd_verify_credential,
    cmd_get_verifying_key,
    cmd_propose_resolution,
    cmd_vote_on_resolution,
    cmd_list_resolutions,
    cmd_get_resolution,
    cmd_issue_resolution_credential,
    cmd_create_committee,
]

do_committee = pcommand.create_shell_command('committee', __commands__)

# -----------------------------------------------------------------
# Enable binding of the shell independent version to a pdo-shell command
# -----------------------------------------------------------------
def load_commands(cmdclass) :
    pshell.bind_shell_command(cmdclass, 'committee', do_committee)
    pshell.bind_shell_command(cmdclass, 'committee_contract', do_committee_contract)
