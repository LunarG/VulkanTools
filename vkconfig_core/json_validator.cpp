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
 * Authors:
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "json_validator.h"
#include "util.h"

#include <QFile>

#ifndef JSON_VALIDATION_OFF

#include <nlohmann/json-schema.hpp>

using json = nlohmann::json;
using json_validator = nlohmann::json_schema::json_validator;

static bool validator_initialized = false;
static json_validator validator_instance;

static json ParseFile(const char *file) {
    QFile file_schema(file);
    const bool result = file_schema.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(result);
    json json_schema(json::parse(file_schema.readAll().toStdString()));
    file_schema.close();

    return json_schema;
}

struct custom_error_handler : public nlohmann::json_schema::basic_error_handler {
    custom_error_handler() : error_count(0) {}

    void error(const nlohmann::json_pointer<nlohmann::basic_json<>> &pointer, const json &instance,
               const std::string &message) override {
        nlohmann::json_schema::basic_error_handler::error(pointer, instance, message);

        this->message += message;
        ++this->error_count;
    }

    int error_count;
    std::string message;
};

Validator::Validator() : error(0) {
    if (!::validator_initialized) {
        ::validator_instance = ParseFile(":/layers_schema.json");
        ::validator_initialized = true;
    }
}

bool Validator::Check(const std::string &json_data) {
    assert(!json_data.empty());

    json json_file = json::parse(json_data);

    custom_error_handler json_err;

    ::validator_instance.validate(json_file, json_err);

    if (json_err.error_count > 0) {
        this->message += json_err.message;
        this->error += json_err.error_count;
    }

    return !json_err;
}

void Validator::Reset() {
    this->message.clear();
    this->error = 0;
}
#else

Validator::Validator() : error(0) {}

bool Validator::Check(const std::string &json_data) {
    (void)json_data;

    return true;
}

void Validator::Reset() {
    this->message.clear();
    this->error = 0;
}

#endif  // JSON_VALIDATION_OFF
