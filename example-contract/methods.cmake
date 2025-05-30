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

INCLUDE(family.cmake)

# ---------------------------------------------
# Set up the include list
# ---------------------------------------------
SET (${CF_HANDLE}_INCLUDES ${WASM_INCLUDES})
LIST(APPEND ${CF_HANDLE}_INCLUDES ${CMAKE_CURRENT_LIST_DIR}/src)

# ---------------------------------------------
# Set up the default source list
# ---------------------------------------------
FILE(GLOB ${CF_HANDLE}_COMMON_SOURCE ${CMAKE_CURRENT_LIST_DIR}/src/common/*.cpp)
FILE(GLOB ${CF_HANDLE}_CONTRACT_SOURCE ${CMAKE_CURRENT_LIST_DIR}/src/methods/*.cpp)

SET (${CF_HANDLE}_SOURCES PARENT_SCOPE)
LIST(APPEND ${CF_HANDLE}_SOURCES ${${CF_HANDLE}_COMMON_SOURCE})
LIST(APPEND ${CF_HANDLE}_SOURCES ${${CF_HANDLE}_CONTRACT_SOURCE})

# ---------------------------------------------
# Build the wawaka contract common library
# ---------------------------------------------
SET(${CF_HANDLE}_LIB ww_${CF_NAME})
