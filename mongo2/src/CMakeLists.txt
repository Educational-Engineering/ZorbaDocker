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

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}/mongo2.xq.src")

DECLARE_ZORBA_MODULE (
  URI "http://www.zorba-xquery.com/modules/mongo2"
  VERSION 1.0
  FILE "mongo2.xq"
  LINK_LIBRARIES "${MONGO_LIBRARIES}" "mongoclient" "boost_thread" "boost_system" "boost_regex" "pthread"
    )
