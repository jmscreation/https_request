/*
    Author:     jmscreator
    License:   Always free to use forever (See MIT License)
*/

#include "jsonloader.h"


namespace json {
    /*
        JSON Wrapper Basic Variables
    */

    // public
    bool displayErrors = false;

    // private
    bool _ready = false;
    rapidjson::Document _jsonData;

    /*
        Load External Parameters From JSON Data
    */

   bool loadProperty(const Object& config, const char* name, Object& parameter) {
        if(!_ready){
            if(displayErrors){
                std::cout << "Must parse JSON data before loading property\n";
            }
            return false;
        }

        if(config.IsObject() && config.HasMember(name)){
            if(config[name].IsObject()){
                parameter.SetObject();
                parameter.CopyFrom(config[name], _jsonData.GetAllocator());
                return true;
            } else {
                if(displayErrors) {
                    std::cout << name << " is not a valid object\n";
                }
            }
        }
        return false;
    }

    bool loadPropertyArray(const Object& config, const char* name, Array& parameter) {
        if(!_ready){
            if(displayErrors){
                std::cout << "Must parse JSON data before loading property\n";
            }
            return false;
        }

        if(config.IsObject() && config.HasMember(name)){
            if(config[name].IsArray()){
                parameter.SetArray();
                Object copy;
                copy.CopyFrom(config[name], _jsonData.GetAllocator()); // make copy of array to prevent issues on additional loading

                rapidjson::GenericArray<false, Object> array = copy.GetArray(); // get non-const array of copy
                for(auto it = array.begin(); it != array.end(); ++it){
                    parameter.PushBack(*it, _jsonData.GetAllocator()); // move copied elements
                }

                return true;
            } else {
                if(displayErrors) {
                    std::cout << name << " is not a valid array\n";
                }
            }
        }
        return false;
    }

    bool parseData(const char* data, size_t length) {
        rapidjson::Document& config = _jsonData;
        config.SetNull();
        config.Parse(data, length);
        if(config.GetParseError() != rapidjson::ParseErrorCode::kParseErrorNone){
            if(displayErrors){
                std::cout << "JSON Failed to parse - Error @" << config.GetErrorOffset()
                          << " -> " << rapidjson::GetParseError_En(config.GetParseError()) << "\n";
            }
            return false;
        }
        if(!config.IsObject()){
            if(displayErrors){
                std::cout << "Initial JSON must be an object\n";
            }
            return false;
        }
        _ready = true;
        return true;
    }

    void clearData() {
        _jsonData.Clear();
        _ready = false;
    }
}