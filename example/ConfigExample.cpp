#include "ConfigExample.hpp"
#include "Config.hpp"
#include <iostream>
#include <string>

void ConfigExample::run() {

    Config testConfig("test.bin", "TESTCFG", "1.0");

    auto section = testConfig.getSection("Test");

    section->set("str", "Hello, World!");
    section->set("int", 42);
    section->set("float", 3.14f);
    section->set("bool", true);
    section->set("double", 3.14159);

    testConfig.save();

    Config loadedConfig("test.bin", "TESTCFG", "1.0");
    loadedConfig.load();
    
    std::string str = loadedConfig.getSection("Test")->get<std::string>("str");
    int intValue = loadedConfig.getSection("Test")->get<int>("int");
    float floatValue = loadedConfig.getSection("Test")->get<float>("float");
    bool boolValue = loadedConfig.getSection("Test")->get<bool>("bool");
    double doubleValue = loadedConfig.getSection("Test")->get<double>("double");

    std::cout << "String: " << str << std::endl;
    std::cout << "Integer: " << intValue << std::endl;
    std::cout << "Float: " << floatValue << std::endl;
    std::cout << "Boolean: " << (boolValue ? "true" : "false") << std::endl;
    std::cout << "Double: " << doubleValue << std::endl;
}