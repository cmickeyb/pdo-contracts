# Multiuser Test Plan #

## General Notes ##

* The identity needs to be set in the token contract context. The
  environment bindings are set, but there is no place where the
  environment bindings are expanded. The context bindings do not set
  the identity.

<!--- ================================================================= --->
## Stage 1 ##

**Goal**: Demonstrate that two users can work with the same contract.

The token contract already supports a context file. A simple factory can
import a contract collection for a specific token, write the context file,
and, then provision the token instance with the specific context file.

### Scenario ###

**Alice**:
* Create keys for alice
* Export the public keys for alice

**Issuer**:
* Create keys for blue widget
* Export blue widget public keys
* Create blue widget token issuer using the blue widget keys from the token issuer factory
* Create five blue widget tokens from the blue widget token issuer
* Import public keys for alice
* Transfer ownership of blue widget token 1 to alice
* Export the blue widget token 1 contract collection

**Alice**:
* Create the blue widget token from the token import factory
* Execute the echo method

### Tasks ###

* (DONE) Create a way to download a file
* (DONE) Use file download to export keys
* (DONE) Use file download to export contract collection
* Create the token contract import factory notebook

<!--- ================================================================= --->
## Stage 2 ##

**Goal**: Demonstrate a wallet notebook that can import a single issuer

This task is slightly more difficult than the first because we do not
have a notebook focused on client access to an issuance.

### Scenario ###

**Alice**:
* Create keys for alice
* Export the public keys for alice

**Issuer**:
* Create keys for blue marble
* Export blue marble public keys
* Create blue marble issuer using the blue marble keys
* Import public keys for alice
* Issue 50 blue marbles to alice
* Export the blue marble contract collection

**Alice**:
* Create the wallet contract
* Import the blue marble issuer contract
* Check balance

### Tasks ###

* Implement the basic wallet

### Basic Wallet ###

* Import an issuer contract
* Select a key to associate with the issuer contract
* Perform a balance check

When a contract collection is imported, a context is created. The
wallet works with the imported context. The imported context can
be modified to, for example, replace the identities used for the
different operations.

<!--- ================================================================= --->
## Final Plan ##

**Goal**: Demonstrate a wallet that can accommodate multiple contracts

### Scenario ###

Alice:
* Create keys for alice
* Export the public keys for alice

Bob:
* Create keys for bob
* Export the public keys for bob

Issuer:
* Create keys for blue marble, red marble
* Export blue marble, red marble public keys
* Create blue marble issuer using the blue marble keys
* Import public keys for alice and bob
* Issue 50 blue marbles to alice and bob
* Create the red marble issuer using the red marble keys
* Issue 500 red marbles to alice and bob
* Export the blue marble, red marble contracts

Alice:
* Import blue marble and red marble public keys
* Create wallet contract
* Import blue marble issuer
* Import red marble issuer

### Tasks ###

### Multi-Issuer Wallet ###

The wallet context is bound to an identifier and may be selected for
operations. This can be done using a prefix into the context.
