# Copyright 2019 Intel Corporation
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

INCLUDE(methods.cmake)

# -----------------------------------------------------------------
ADD_LIBRARY(${EXCHANGE_LIB} STATIC ${EXCHANGE_SOURCES})
TARGET_INCLUDE_DIRECTORIES(${EXCHANGE_LIB} PUBLIC ${EXCHANGE_INCLUDES})
TARGET_INCLUDE_DIRECTORIES(${EXCHANGE_LIB} PUBLIC ${WASM_INCLUDES})

SET_PROPERTY(TARGET ${EXCHANGE_LIB} APPEND_STRING PROPERTY COMPILE_OPTIONS "${WASM_BUILD_OPTIONS}")
SET_PROPERTY(TARGET ${EXCHANGE_LIB} APPEND_STRING PROPERTY LINK_OPTIONS "${WASM_LINK_OPTIONS}")
SET_TARGET_PROPERTIES(${EXCHANGE_LIB} PROPERTIES EXCLUDE_FROM_ALL TRUE)

# -----------------------------------------------------------------
BUILD_CONTRACT(asset_type contracts/asset_type.cpp
  HEADERS ${EXCHANGE_INCLUDES}
  LIBRARIES ${EXCHANGE_LIB})

BUILD_CONTRACT(vetting_organization contracts/vetting_organization.cpp
  HEADERS ${EXCHANGE_INCLUDES}
  LIBRARIES ${EXCHANGE_LIB})

BUILD_CONTRACT(issuer contracts/issuer.cpp
  HEADERS ${EXCHANGE_INCLUDES}
  LIBRARIES ${EXCHANGE_LIB})

BUILD_CONTRACT(exchange contracts/exchange.cpp
  HEADERS ${EXCHANGE_INCLUDES}
  LIBRARIES ${EXCHANGE_LIB})

BUILD_CONTRACT(data_guardian contracts/data_guardian.cpp
  HEADERS ${EXCHANGE_INCLUDES}
  LIBRARIES ${EXCHANGE_LIB})

BUILD_CONTRACT(token_issuer contracts/token_issuer.cpp
  HEADERS ${EXCHANGE_INCLUDES}
  LIBRARIES ${EXCHANGE_LIB})

BUILD_CONTRACT(token_object contracts/token_object.cpp
  HEADERS ${EXCHANGE_INCLUDES}
  LIBRARIES ${EXCHANGE_LIB})

# -----------------------------------------------------------------
INCLUDE(Python)
BUILD_WHEEL(exchange asset_type vetting_organization issuer exchange data_guardian token_issuer token_object)

# -----------------------------------------------------------------
INCLUDE(Test)
ADD_SHELL_TEST(exchange script SCRIPT test/script_test.sh)
ADD_SHELL_TEST(exchange functional SCRIPT test/run_tests.sh)

# -----------------------------------------------------------------
# install the jupyter notebooks, note that the trailing slash here
# is significant and should not be removed; it prevents the notebooks
# directory being prepended to the copied name
# -----------------------------------------------------------------
INCLUDE(Jupyter)
FILE(GLOB_RECURSE NOTEBOOK_SOURCES docs/notebooks/*.py docs/notebooks/*.md)

CONVERT_JUPYTEXT(EX_NOTEBOOKS ${NOTEBOOK_SOURCES})

ADD_CUSTOM_TARGET(exchange-notebooks ALL DEPENDS ${EX_NOTEBOOKS})

INSTALL(DIRECTORY docs/notebooks/
  DESTINATION "${PDO_JUPYTER_ROOT}/exchange"
  FILES_MATCHING PATTERN "*.ipynb" PATTERN "*.png")
