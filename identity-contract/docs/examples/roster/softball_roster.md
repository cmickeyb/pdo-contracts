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
* Coaches must receive a minimum educational certification
* Coaches must prove successful concussion training
* Coaches must prove a successful background check
* Coaches must be insured for a minimum of $500,000 liability

## Demonstration ##

* For each player and coach:
  * Create a key pair for the person
  * Create a digital asset with the person's picture
  * Create an identity for the person with a description (fields for
    name, address, and birthday) and identified-by (contract
    identifier for the digital asset that manages their picture)

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
