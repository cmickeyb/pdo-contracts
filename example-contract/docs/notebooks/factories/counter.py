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
# # Counter Factory
#
# This notebook simplifies the creation of an instance of a token issuer for a digital asset (in
# this case assets are 24bit bmp images). Update the token configuration information then evaluate
# the notebook to create a new token issuer. That token issuer will be able to mint tokens.

# %%
import os
import pdo.contracts.jupyter as pc_jupyter
import IPython.display as ip_display

pc_jupyter.load_ipython_extension(get_ipython())

try : state
except NameError:
    (state, bindings) = pc_jupyter.initialize_environment('unknown')

# %% [markdown]
# ## Configure Counter Information
#

# %%
ip_display.display(pc_jupyter.example_jupyter.CounterWidget(state, bindings))
