# Copyright 2022 Intel Corporation
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

# ---------------------------------------------
# Set up the include list
# ---------------------------------------------
SET (EXAMPLE_INCLUDES ${WASM_INCLUDES})
LIST(APPEND EXAMPLE_INCLUDES ${CMAKE_CURRENT_LIST_DIR})

# ---------------------------------------------
# Set up the default source list
# ---------------------------------------------
FILE(GLOB EXAMPLE_COMMON_SOURCE ${CMAKE_CURRENT_LIST_DIR}/example/common/*.cpp)
FILE(GLOB EXAMPLE_CONTRACT_SOURCE ${CMAKE_CURRENT_LIST_DIR}/example/contracts/*.cpp)

SET (EXAMPLE_SOURCES)
LIST(APPEND EXAMPLE_SOURCES ${EXAMPLE_COMMON_SOURCE})
LIST(APPEND EXAMPLE_SOURCES ${EXAMPLE_CONTRACT_SOURCE})

# ---------------------------------------------
# Build the wawaka contract common library
# ---------------------------------------------
SET(EXAMPLE_LIB ww_example)
