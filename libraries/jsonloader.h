/*
    Author:     jmscreator
    License:   Always free to use forever (See MIT License)
*/

#ifndef __JSON_LOADER__
#define __JSON_LOADER__

//include rapidjson Parse Library
#include <string>
#include <iostream>
#include <type_traits>

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"


namespace json {

    // public
    typedef rapidjson::Value Object;
    typedef rapidjson::Value Array;
    extern bool displayErrors;

    // private
    extern bool _ready;
    extern rapidjson::Document _jsonData;

    bool loadProperty(const Object& config, const char* name, Object& parameter);
    bool loadPropertyArray(const Object& config, const char* name, Array& parameter);

    template<class T>
    bool loadProperty(const Object& config, const char* name, T& parameter) {
        if(std::is_pointer<T>::value){
            if(displayErrors){
                std::cout << "Pointer cannot be loaded from JSON\n";
            }
            return false;
        }

        if(!_ready){
            if(displayErrors){
                std::cout << "Must parse JSON data before loading property\n";
            }
            return false;
        }

        if(config.IsObject() && config.HasMember(name)){
            if(std::is_arithmetic<T>::value && config[name].IsNumber()) {
                if(config[name].IsUint()) { parameter = config[name].GetUint(); return true; }
                if(config[name].IsInt()) { parameter = config[name].GetInt(); return true; }

                if(config[name].IsInt64()) { parameter = config[name].GetInt64(); return true; }
                if(config[name].IsUint64()) { parameter = config[name].GetUint64(); return true; }

                if(config[name].IsDouble()) { parameter = config[name].GetDouble(); return true; }
                if(config[name].IsFloat()) { parameter = config[name].GetFloat(); return true; }
            }

            if(config[name].Is<T>()){
                parameter = config[name].Get<T>();
                return true;
            } else {
                if(displayErrors) {
                    std::cout << name << " is not a valid type\n";
                }
            }
        }
        return false;
    }

    template<class T>
    bool loadValue(const Object& value, T& parameter) {
        if(std::is_pointer<T>::value){
            if(displayErrors){
                std::cout << "Pointer cannot be loaded from JSON\n";
            }
            return false;
        }

        if(!_ready){
            if(displayErrors){
                std::cout << "Must parse JSON data before loading property\n";
            }
            return false;
        }

        
        if(std::is_arithmetic<T>::value && value.IsNumber()) {
            if(value.IsUint()) { parameter = value.GetUint(); return true; }
            if(value.IsInt()) { parameter = value.GetInt(); return true; }

            if(value.IsInt64()) { parameter = value.GetInt64(); return true; }
            if(value.IsUint64()) { parameter = value.GetUint64(); return true; }

            if(value.IsDouble()) { parameter = value.GetDouble(); return true; }
            if(value.IsFloat()) { parameter = value.GetFloat(); return true; }
        }

        if(value.Is<T>()){
            parameter = value.Get<T>();
            return true;
        } else {
            if(displayErrors) {
                std::cout << "given value is not a valid type\n";
            }
        }
        return false;
    }

    template<class T>
    bool loadProperty(const char* name, T& parameter) {
        return loadProperty(_jsonData, name, parameter);
    }

    template<class T>
    bool loadPropertyArray(const char* name, T& parameter) {
        return loadPropertyArray(_jsonData, name, parameter);
    }

    bool parseData(const char* data, size_t length);
    void clearData();

}

#endif // __JSON_LOADER__