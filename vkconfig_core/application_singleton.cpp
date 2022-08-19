/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This class can be used to ensure only a single instance of an
 * application is running. The most obvious and slightly "faster"
 * approach would be to used QSharedMemory, however shared memory
 * is not freed if the app crashes. The trade off here is a slight
 * startup delay (500ms) to check if a local server is already
 * running. This is 100% robust from an application crash on all
 * platforms.
 *
 * Authors:
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "application_singleton.h"

ApplicationSingleton::ApplicationSingleton(const std::string& application_name, int timeout)
    : _application_name(application_name)
    , _timeout(timeout) {
}

ApplicationSingleton::~ApplicationSingleton() { _local_server.close(); }

bool ApplicationSingleton::IsFirstInstance() {
    // If we can connect to the server, it means there is another copy running
    QLocalSocket localSocket;
    localSocket.connectToServer(_application_name.c_str());

    // The default timeout is 5 seconds, which should be enough under
    // the most extreme circumstances. Note, that it will actually
    // only time out if the server exists and for some reason it can't
    // connect. Too small a timeout on the other hand can give false
    // assurance that another copy is not running.
    if (localSocket.waitForConnected(_timeout)) {
        localSocket.close();
        return false;
    }

    // Not connected, OR timed out
    // We are the first, start a server
    _local_server.listen(_application_name.c_str());

    return true;
}
