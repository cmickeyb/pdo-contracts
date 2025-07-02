# Copyright 2025 Intel Corporation
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

import pdo.client.builder.shell as pshell
import pdo.client.builder.script as pscript

import pdo.common.crypto as pcrypto

logger = logging.getLogger(__name__)

__all__ = [
    'cmd_extract_credential',
    'do_credential',
    'load_commands',
]

## -----------------------------------------------------------------
## -----------------------------------------------------------------
class cmd_extract_credential(pscript.script_command_base) :
    name = "extract"
    help = "extract the credential from a verifiable credential"

    @classmethod
    def add_arguments(cls, subparser) :
        subparser.add_argument(
            '--signed-credential',
            help='The name of the file where the signed credential is stored',
            type=str,
            required=True)

        subparser.add_argument(
            '--credential',
            help='The name of the file where the extracted credential will be stored',
            type=str)

    @classmethod
    def invoke(cls, state, bindings, signed_credential, credential=None, **kwargs) :

        with open(signed_credential, "r") as fp :
            signed_credential_data = json.load(fp)

        serialized_credential = signed_credential_data['serializedCredential']
        deserialized_credential = pcrypto.byte_array_to_string(pcrypto.base64_to_byte_array(serialized_credential))

        if credential :
            with open(credential, "w") as cfp :
                cfp.write(deserialized_credential)
        else :
            cls.display(deserialized_credential)

        return deserialized_credential

## -----------------------------------------------------------------
## Create the generic, shell independent version of the aggregate command
## -----------------------------------------------------------------
__subcommands__ = [
    cmd_extract_credential,
]
do_credential = pscript.create_shell_command('credential', __subcommands__)

## -----------------------------------------------------------------
## Enable binding of the shell independent version to a pdo-shell command
## -----------------------------------------------------------------
def load_commands(cmdclass) :
    pshell.bind_shell_command(cmdclass, 'credential', do_credential)
