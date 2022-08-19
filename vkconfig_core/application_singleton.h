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
 * startup delay (500ms by default) to check if a local server is already
 * running. This is 100% robust from an application crash on all
 * platforms.
 *
 * Authors:
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

#include <string>

class ApplicationSingleton {
   public:
    ApplicationSingleton(const std::string &application_name, int timeout = 5000);
    ~ApplicationSingleton();

    bool IsFirstInstance();

   private:
    QLocalServer _local_server;
    std::string _application_name;
    int _timeout;

    ApplicationSingleton(const ApplicationSingleton &) = delete;
    ApplicationSingleton &operator=(const ApplicationSingleton &) = delete;
};
