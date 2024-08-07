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

INCLUDE(example_common.cmake)
LIST(APPEND WASM_LIBRARIES ${EXAMPLE_LIB})
LIST(APPEND WASM_INCLUDES ${EXAMPLE_INCLUDES})

# -----------------------------------------------------------------
ADD_LIBRARY(${EXAMPLE_LIB} STATIC ${EXAMPLE_SOURCES})
TARGET_INCLUDE_DIRECTORIES(${EXAMPLE_LIB} PUBLIC ${EXAMPLE_INCLUDES})

SET_PROPERTY(TARGET ${EXAMPLE_LIB} APPEND_STRING PROPERTY COMPILE_OPTIONS "${WASM_BUILD_OPTIONS}")
SET_PROPERTY(TARGET ${EXAMPLE_LIB} APPEND_STRING PROPERTY LINK_OPTIONS "${WASM_LINK_OPTIONS}")
SET_TARGET_PROPERTIES(${EXAMPLE_LIB} PROPERTIES EXCLUDE_FROM_ALL TRUE)

# -----------------------------------------------------------------
BUILD_CONTRACT(example_counter contracts/counter.cpp)

# -----------------------------------------------------------------
INCLUDE(Python)
BUILD_WHEEL(example example_counter)

# -----------------------------------------------------------------
INCLUDE(Test)
ADD_SHELL_TEST(example script SCRIPT test/script_test.sh)
ADD_SHELL_TEST(example functional SCRIPT test/run_tests.sh)

# -----------------------------------------------------------------
# install the jupyter notebooks, note that the trailing slash here
# is significant and should not be removed; it prevents the notebooks
# directory being prepended to the copied name
# -----------------------------------------------------------------
INCLUDE(Jupyter)
FILE(GLOB_RECURSE NOTEBOOK_SOURCES docs/notebooks/*.py docs/notebooks/*.md)

CONVERT_JUPYTEXT(EX_NOTEBOOKS ${NOTEBOOK_SOURCES})

ADD_CUSTOM_TARGET(example-notebooks ALL DEPENDS ${EX_NOTEBOOKS})

INSTALL(DIRECTORY docs/notebooks/
  DESTINATION "${PDO_JUPYTER_ROOT}/example"
  FILES_MATCHING PATTERN "*.ipynb" PATTERN "*.png")
