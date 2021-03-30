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

#include <nlohmann/json-schema.hpp>

#include <QMessageBox>
#include <QFile>

using json = nlohmann::json;
using json_validator = nlohmann::json_schema::json_validator;

static json ParseFile(const char *file) {
    QFile file_schema(file);
    const bool result = file_schema.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(result);
    json json_schema(json::parse(file_schema.readAll().toStdString()));
    file_schema.close();

    return json_schema;
}

class custom_error_handler : public nlohmann::json_schema::basic_error_handler {
   public:
    custom_error_handler(const char *filename) : filename(filename) {}

   private:
    void error(const nlohmann::json_pointer<nlohmann::basic_json<>> &pointer, const json &instance,
               const std::string &message) override {
        nlohmann::json_schema::basic_error_handler::error(pointer, instance, message);

        QMessageBox alert;
        alert.setWindowTitle(format("Failed to validate a layer manifest. The layer will be ignored.", filename.c_str()).c_str());
        alert.setText(format("'%s' is not valid.", filename.c_str()).c_str());
        alert.setInformativeText(message.c_str());
        alert.setIcon(QMessageBox::Critical);
        alert.exec();
    }

    std::string filename;
};

bool IsValid(const char *json_filename) {
    json json_schema = ParseFile(":/layers_schema.json");

    json_validator validator;

    try {
        validator.set_root_schema(json_schema);
    } catch (const std::exception &e) {
        QMessageBox alert;
        alert.setWindowTitle("'layers_schema.json' is not valid.");
        alert.setText(e.what());
        alert.setIcon(QMessageBox::Critical);
        alert.exec();

        return false;
    }

    json json_file = ParseFile(json_filename);
    custom_error_handler json_err(json_filename);

    validator.validate(json_file, json_err);

    return true;
}
