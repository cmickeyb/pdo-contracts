# ---
# jupyter:
#   jupytext:
#     text_representation:
#       extension: .py
#       format_name: percent
#       format_version: '1.3'
#       jupytext_version: 1.16.1
#   kernelspec:
#     display_name: Python 3 (ipykernel)
#     language: python
#     name: python3
# ---

# %% [markdown]
# *WORK IN PROGRESS*
# # Token Import Factory
#
# Use this notebook to import a token from a contract collection file.

# %%
import os
import pdo.contracts.jupyter as pc_jupyter
import IPython.display as ip_display

pc_jupyter.load_ipython_extension(get_ipython())

# %% [markdown]
# ## Configure Token Contract
#
# This section enables customization of the wallet and the issuer with which it is associated.
#
# * identity : the identity of the creator of the asset type
# * token_import_file : the name of the contract import file for an existing exchange
# * token_context_file : the name of the context file to use for the exchange contract
#
# Note that the notebook assumes that there is a key file for the identity of the form: `${keys}/${identity}_private.pem`.

# %%
token_owner = input('Identity to use for interacting with the token: ')
token_class = input('Class of tokens to import: ')
token_name = input('Name of the token [token_1]: ') or 'token_1'
token_import_file = input('Name of the token import file: ')

context_file = '${etc}/${token_class}_context.toml'
# %% [markdown]
# ### Initialize the PDO Environment
#
# Initialize the PDO environment. This assumes that a functional PDO configuration is in place and
# that the PDO virtual environment has been activated. In particular, ensure that the groups file
# and eservice database have been configured correctly.
#
# For the most part, no modifications should be required below.
# %%
(state, bindings) = pc_jupyter.initialize_environment(token_owner)
_ = pc_jupyter.import_contract_collection(state, bindings, context_file, token_import_file)

# %% [markdown]
# ## Create the Token

# %%
instance_parameters = {
    'token_owner' : token_owner,
    'token_class' : token_class,
    'token_name' : token_name,
    'context_file' : context_file,
}

instance_file = pc_jupyter.instantiate_notebook_from_template(token_class, 'token', parameters)
ip_display.display(ip_display.Markdown('[Token {}]({})'.format(parameters['token_name'], instance_file)))
