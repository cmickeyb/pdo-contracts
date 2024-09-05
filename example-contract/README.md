<!---
Licensed under Creative Commons Attribution 4.0 International License
https://creativecommons.org/licenses/by/4.0/
--->

**The protocols and software are for reference purposes only and not intended for production usage.**

# Example Contract Family #

This directory contains an example contract family that can be used as
the basis for implementing your own contract families. This file will
walk you through the process of creating a new contract family.

Note that this description is not intended to be prescriptive. There
are many ways to build and deploy a contract. This directory simply
describes one way that we have found relatively easy to use.

# What is a Contract Family? #

A contract family is a collection of PDO contracts that work together
to create an "application". For example, the contracts in the Exchange
contract family, provide contracts to create digital asset like "red
marbles" with a veriable trust chain and contracts to use those assets
to purchase or exchange goods, services, or other assets. It is the
collection of contracts intentionally designed to work together that
we call a contract family.

Operationally, a contract family consists of a set of PDO contracts
and instructions for how to build them, configuration files that help
to specify the relationship between contracts or contract objects, and
plugins that makes it easier to use the contracts through a PDO shell,
a bash shell, or a Jupyter notebook.

## File System Layout ##

* The root directory of the contract family generally contains
  information useful for building and deploying contracts in the
  family.

* The `context` directory contains context files that describe the
  relationship between contract objects and help to coordinate the
  configuration of dependent contract objects.

* The `contracts` directory contains one source file for each type of
  contract in the contract family. The source file specifies the set of
  methods that are associated with the contract.

* The `docs` directory generally contains interface specifications for
  the methods in the contract. It may also contain the templates for
  Jupyter notebooks used to interact with the contracts.

* The `etc` directory contains basic configuration information. When
  you customize your own contract family, you will need to update
  information about the mapping between contract types and compiled
  contract code.

* The `example` directory in this specific contract family contains
  code and definitions that are used by the source files in the
  `contracts` directory. That is, the source files in the `contracts`
  directory tend to specify methods that are defined in the source
  files in the `example` directory. The use of a separate `example`
  directory makes it possible to share code between contracts and even
  between contract families.

* The `pdo` directory is the root of the Python modules associated
  with the contract family. The Python modules contain plugins for the
  PDO and bash shells, and utility functions that can be invoked from
  a Jupyter notebook.

* The `scripts` directory may optionally contain PDO shell scripts
  that will be installed with the deployed contracts.

* The `test` directory defines system tests for the contract family
  that will be automatically run by the contracts CI system. Tests are
  broken into two parts, one for running a set of commands in a single
  PDO shell invocation and one for running a series of bash shell
  commands. Note that the two are similar but not the same. The PDO
  shell will process transaction commits asynchronously while the bash
  shell tests commit synchronously.

# How to Create a New Contract Family #

# How to Add a New Contract to the Contract Family #

## How to Implement Contract Methods ##

## How to Define a Contract ##

## How to Write a Plugin for a Contract ##
