#include <nlohmann/json-schema.hpp>

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

using json = nlohmann::json;
using json_validator = nlohmann::json_schema::json_validator;

#ifdef JSON_SCHEMA
static const std::string SCHEMA(JSON_SCHEMA);
#else
#error JSON_SCHEMA must be defined
#endif

#ifdef JSON_MANIFEST
static const std::string MANIFEST(JSON_MANIFEST);
#else
#error JSON_MANIFEST must be defined
#endif

static json Parse(const char* path) {
    std::ifstream t(path);
    std::string data;

    t.seekg(0, std::ios::end);
    data.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    data.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    return json::parse(data);
}

int main(int argc, char* argv[]) {
    printf("Validating Json manifest against Json schema\n");
    printf("Schema %s\n", SCHEMA.c_str());
    printf("Manifest %s\n", MANIFEST.c_str());

    json schema = Parse(JSON_SCHEMA);

    json_validator validator;
    validator.set_root_schema(schema);

    json manifest = Parse(JSON_MANIFEST);

    try {
        validator.validate(manifest);
    } catch (const std::exception& e) {
        std::cerr << "JSON validation error:" << e.what() << std::endl;

        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
