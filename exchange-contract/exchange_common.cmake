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
SET (EXCHANGE_INCLUDES ${WASM_INCLUDES})
LIST(APPEND EXCHANGE_INCLUDES ${CMAKE_CURRENT_LIST_DIR})

# ---------------------------------------------
# Set up the default source list
# ---------------------------------------------
FILE(GLOB EXCHANGE_COMMON_SOURCE ${CMAKE_CURRENT_LIST_DIR}/exchange/common/*.cpp)
FILE(GLOB EXCHANGE_CONTRACT_SOURCE ${CMAKE_CURRENT_LIST_DIR}/exchange/contracts/*.cpp)

SET (EXCHANGE_SOURCES)
LIST(APPEND EXCHANGE_SOURCES ${EXCHANGE_COMMON_SOURCE})
LIST(APPEND EXCHANGE_SOURCES ${EXCHANGE_CONTRACT_SOURCE})

# ---------------------------------------------
# Build the wawaka contract common library
# ---------------------------------------------
SET(EXCHANGE_LIB ww_exchange)

ADD_LIBRARY(${EXCHANGE_LIB} STATIC ${EXCHANGE_SOURCES})
TARGET_INCLUDE_DIRECTORIES(${EXCHANGE_LIB} PUBLIC ${EXCHANGE_INCLUDES})

SET_PROPERTY(TARGET ${EXCHANGE_LIB} APPEND_STRING PROPERTY COMPILE_OPTIONS "${WASM_BUILD_OPTIONS}")
SET_PROPERTY(TARGET ${EXCHANGE_LIB} APPEND_STRING PROPERTY LINK_OPTIONS "${WASM_LINK_OPTIONS}")
SET_TARGET_PROPERTIES(${EXCHANGE_LIB} PROPERTIES EXCLUDE_FROM_ALL TRUE)
