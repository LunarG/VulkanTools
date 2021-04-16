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
#include <QJsonDocument>

#include <iostream>

#ifndef JSON_VALIDATION_OFF

#include <valijson/adapters/qtjson_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validation_results.hpp>
#include <valijson/validator.hpp>

using valijson::Schema;
using valijson::SchemaParser;
using valijson::ValidationResults;
using valijson::Validator;
using valijson::adapters::QtJsonAdapter;

static QJsonDocument ParseJsonFile(const char *file) {
    QFile file_schema(file);
    const bool result = file_schema.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(result);
    const QString &data = file_schema.readAll();
    file_schema.close();

    QJsonParseError json_parse_error;
    const QJsonDocument &json_document = QJsonDocument::fromJson(data.toUtf8(), &json_parse_error);

    return json_document;
}

JsonValidator::JsonValidator() {}

bool JsonValidator::Check(const QString &json_data) {
    assert(!json_data.isEmpty());

    const QJsonDocument schema_document = ParseJsonFile(":/layers/schema.json");

    QJsonParseError json_parse_error;
    const QJsonDocument json_document = QJsonDocument::fromJson(json_data.toUtf8(), &json_parse_error);
    if (json_parse_error.error != QJsonParseError::NoError) {
        return false;
    }

    Schema schema;
    SchemaParser parser;
    QtJsonAdapter schema_adapter(schema_document.object());
    parser.populateSchema(schema_adapter, schema);

    Validator validator(Validator::kWeakTypes);
    QtJsonAdapter document_adapter(json_document.object());

    ValidationResults results;
    if (!validator.validate(schema, document_adapter, &results)) {
        // std::cerr << "Validation failed." << endl;
        ValidationResults::Error error;
        unsigned int error_num = 1;
        while (results.popError(error)) {
            std::string context;
            std::vector<std::string>::iterator itr = error.context.begin();
            for (; itr != error.context.end(); itr++) {
                context += *itr;
            }

            if (error_num <= 3) {
                std::string log = format("Error #%d\n", error_num);
                log += "\t context: " + context + "\n";
                log += "\t desc:    " + error.description + "\n\n";

                message += log.c_str();
            }

            ++error_num;
        }

        message += format("Total Error Count: %d\n", error_num).c_str();

        return false;
    }

    return true;
}

#else

JsonValidator::JsonValidator() : error(0) {}

bool JsonValidator::Check(const std::string &json_data) {
    (void)json_data;

    return true;
}

#endif  // JSON_VALIDATION_OFF
