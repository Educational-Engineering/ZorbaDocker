# Copyright 2012 The FLWOR Foundation.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (zorba_mongo2_module)
ENABLE_TESTING ()
INCLUDE (CTest)
LIST (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules")

SET (CMAKE_CXX_FLAGS "-pthread -g3 -Wl,--no-undefined ")
#SET (CMAKE_CXX_FLAGS "-I/usr/local/include -L/usr/local/lib -pthread -lmongoclient -lboost_thread-mt -lboost_system -lboost_regex -lrt -lpthread -Wl,--no-undefined")


FIND_PACKAGE (Zorba REQUIRED HINTS "${ZORBA_BUILD_DIR}")
MESSAGE(STATUS "Zorba_USE_FILE ${Zorba_USE_FILE}")
INCLUDE ("${Zorba_USE_FILE}")

IF (ZORBA_SUPRESS_MONGO2)
  MESSAGE (STATUS "ZORBA_SUPRESS_MONGO2 is true - not searching for MongoDB.")
ELSE (ZORBA_SUPRESS_MONGO2)

  MESSAGE (STATUS "Looking for Mongo DB")
  FIND_PACKAGE ("LibMongodb")

  IF (MONGO_FOUND)
    MESSAGE (STATUS "Found MongoDB --" ${MONGO_LIBRARIES})
    INCLUDE_DIRECTORIES (${MONGO_INCLUDE_DIR})

    ADD_SUBDIRECTORY("src")
#   ADD_TEST_DIRECTORY("${PROJECT_SOURCE_DIR}/test")
  ELSE (MONGO_FOUND)
    MESSAGE (STATUS "MongoDB library not found; MongoDB module skipped")
  ENDIF (MONGO_FOUND)
ENDIF (ZORBA_SUPRESS_MONGO2)




DONE_DECLARING_ZORBA_URIS()
