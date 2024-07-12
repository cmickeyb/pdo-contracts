<!---
Licensed under Creative Commons Attribution 4.0 International License
https://creativecommons.org/licenses/by/4.0/
--->

**The protocols and software are for reference purposes only and not intended for production usage.**

# Softball Roster Verification #

## Problem ##

A softball tournament director will allow a team to play in a
tournament only if the team exclusively rosters coaches that meet
minimum certification (e.g. education and safety) and insurance
requirements, and players that are of an appropriate age and also meet
minimum insurance requirements. This task is complicated by the large
number of organizations that provide certification, insurance and age
verification. Nationally, organizations like USA Softball, Premier
Girls Fastpitch, Little League, and Alliance Fastpitch provide a
common set of rules for local affiliates or regional leagues. The
local affiliates, generally at the city or county level, often oversee
coach certification and age verification.

As a result, a tournament director who wants to offer participation to
the broadest possible set of teams must handle information from
potentially hundreds of different sources. Verifiying each roster
requires significant time and is often error prone. Further, the
tournament director gets access to private information that is,
strictly speaking, not necessary to determine that the roster meets
the appropriate criteria. For example, concerns have been expressed
about access to birthday, address, and parent names for children.

## Goal ##

Create a policy agent that will issue a verifiable credential for a
softball roster that meets certain requirements. The policy agent will
accept as authentic, verifiable credentials from specific
issuers. Trust in these issuers is handled offline.

Note: A practical system would likely be hierarchical with each each
national organization providing verification of its own local
associations; in this case the policy agent would establish trust
relationships with the national organizations and then, indirectly,
with each of the local assocations.

The policy agent will determine that the roster meets the minimum
criteria and will create a verifiable credential for the
roster. Private information that may be used to determine that the
roster meets the criteria will not be exposed to the tournament
director.

## Criteria ##

* Players must be of an appropriate age (under 18 as of January 1)
* Players must be insured for a minimum of $100,000 liability
* Coaches must prove successful concussion training
* Coaches must prove a successful background check
* Coaches must be insured for a minimum of $500,000 liability

## Demonstration ##

### Initial Roles ###

#### Local Softball Assocation ####

The local softball association is a signing/issuing authority. It
validates claims that a player or coach meets local requirements for
participation. Initially, the assumption is that a human being
verifies the participants age and address (that is, verifies that the
player meets residential requirements for the association), that all
fees are paid, and that the player is appropriately insured
(generally, fees cover the cost of insurance). That is, the
association contract object is simply a digital representation of a
human decision to approve a player or coach for local participation.

The credential that is created proves approval for local participation
for a player or coach. This includes information about player
eligibility (e.g. this player may play in any tournament for 16 year
olds and under) and insurance coverage. For coaches it will include
certifications (e.g. concussion training)and insurance coverage. For
both players and coaches, the credential will include information to
identify the holder.

The credential created may be presented at any local event to prove
eligibility for participation in an event. It may also be presented at
a national event that accepts the credentialling of the local
association.

The local association contract object provides all of the methods from
an identity contract object. In addition it provides methods to issue
eligibility credentials for players and coaches. The eligibility
issuer for players takes player data as input and returns a verfiable
credential of type `#/pdo-contracts/identity/roster/player`. Input
parameters include:

* player's identity contract identifier
* player's name
* player's address
* player's birthday
* insurance claim (`#/pdo-contracts/identity/roster/insurance`)
* player's photograph as an image contract identifier

Similarly, the eligibility issuer for coaches takes coach data as
input and returns a verifiable credential of type
`#/pdo--contracts/identity/roster/coach`. Input parameters include:

* coach's identity contract identifier
* coach's name
* coach's address
* list of certifications
* insurance claim (`#/pdo-contracts/identity/roster/insurance`)
* coach's photograph as an image contract identifier

#### National Softball Association ####

The national softball association is a policy agent. It manages trust
relationships with local associations. Effectively, the national
association accepts as legitimate any credential issued by one of the
configured local associations. In addition, the national association
implements its own policies for player and coach eligibility. For the
purpose of this demonstration, we'll assume that the national
association requires a minimum level of liability insurance and
requires coach certifications for concussion training and background
check.

The national assocation contract object provides APIs for managing the
set of trusted local associations. We are not concerned, for the
moment, with concerns about revocation. We assume that a credential
issued by the national association is valid even if the trust network
evolves.

The policy interface takes a package of credentials for players and
coaches, ensures that the credentials come from a trusted local
association, verifies that the eligibility requirements are met, and
then issues a roster credential. The roster credential contains a list
of players and coaches, each with information that can be used to
identify the individual. That credential can be presented to a
national tournament director as proof of eligibility.

The national association contract object provides all of the methods
from an identity contract object. In addition, it provides methods to
manage the list of associations trusted to provide player credentials
and to issue a roster verifiable credential. The method to add a
trusted local association takes an `identity_key` for the local
association as a parameter. The method to issue a roster takes a
verifiable presentation that contains verifiable credentials for each
of the players, a verifiable presentation that contains verifiable
credentials for each of the coaches, and information about the
tournament where the roster will be used such as the date of the
tournament and the age classification of the team. The issuer method
will return a veriable credential for the roster.

### Future Roles ###

Initially, determination of player and coach eligibility will be
determined by a human being who uses the local association contract
object to create a verifiable credential. Ultimately, the
demonstration would include more specialized roles that allow for
greater automation.

#### Insurance Agency ####

The insurance agency is a signing/issuing authority. When paid
(e.g. in blue marbles) it will issue a credential that describes
coverage (e.g. policy number, maximum liability, deduction) for the
holder.

#### State Agency ####

The state agency is a signing/issuing authority for validating claims
about names, birth dates, addresses, and photographs. This agency
might be a Department of Motor Vehicles issuing a driver's license.

#### Safety Educational Institution ####

The safety educational institution is an signing/issuing authority to
validate claims of educational certification. For example, if a coach
takes a class on concussion protocols, the educational institution
will issue a credential that includes the concussion training
certification.

#### Background Check ####

A background check is one means of determining if an individuals past
behavior is appropriate for the role of coach. A number of
organizations can provide background checks. While this may be an area
where a "trust network" is used by the local association to determine
which organizations provide an appropriate check, we will assume that
a single organization, represented by a signing/issuing authority,
will provide a certification of an appropriate background.

### Steps ###

* Create an issuing authority contract object for the local assocation
* Create an issuing authority contract object for the national assocation
* Add the local association as a trusted provider for the national association

* For each player and coach:
  * Create a key pair for the person
  * Create a digital asset contract object with the person's picture
  * Create an identity contract object for the person
  * Create a player
  * Create a credential each player using their identity contract; the
    credential will contain claims for name, address, birthday, and
    photograph (contract identifier for the digital asset that manages
    their picture); have the credential signed by the state agency

* Create a signing/issuing authority for the local association.  The
  issuing authority will create verifiable credentials for each of the
  players and coaches. These credentials will include information
  about birthday, address, insurance coverage, and certifications.

* Create a policy agent for an association called "national". The
  policy agent will be configured with a trust relationship with the
  local association (e.g. a copy of the verifying key for the local
  assocation issuing authority object). When presented by a coach with
  a bundle of verifiable credentials that constitutes the coaches and
  players for a team, the policy agent will verify the authenticity of
  each credential, ensure that the appropriate criteria are met, and
  issue a verifiable certificate for the roster.
