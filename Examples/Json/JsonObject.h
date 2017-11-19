#ifndef JsonObjectH
#define JsonObjectH

#include <map>
#include <vector>
#include <string>

#include "JsonObjectType.h"

class JsonObject
{
    JsonObjectType type = JsonObjectType::null;
    std::map<std::string, JsonObject> childs;
    std::vector<JsonObject> elements;
    std::string value;

    public:

        JsonObject() = default;

        JsonObject(std::string newValue) :
            type(JsonObjectType::string), value(newValue)
        {
        }

        JsonObject(long newValue) :
            type(JsonObjectType::number), value(std::to_string(newValue))
        {
        }

        JsonObject(bool newValue) :
            type(JsonObjectType::boolean), value(newValue ? "true" : "false")
        {
        }

        void clear()
        {
            type = JsonObjectType::null;
            childs.clear();
            elements.clear();
            value.clear();
        }

        JsonObject &operator =(std::string newValue)
        {
            type = JsonObjectType::string;
            childs.clear();
            elements.clear();
            value = newValue;
            return *this;
        }

        JsonObject &operator =(long newValue)
        {
            type = JsonObjectType::number;
            childs.clear();
            elements.clear();
            value = std::to_string(newValue);
            return *this;
        }

        JsonObject &operator =(bool newValue)
        {
            type = JsonObjectType::boolean;
            childs.clear();
            elements.clear();
            value = newValue ? "true" : "false";
            return *this;
        }

        JsonObjectType getType() const
        {
            return type;
        }

        std::string toString() const
        {
            return value;
        }

        long toNumber() const
        {
            return strtol(value.c_str(), nullptr, 10);
        }

        bool toBoolean() const
        {
            return value == "true";
        }

        void setChild(const char *name)
        {
            type = JsonObjectType::object;
            elements.clear();
            value.clear();
            childs.insert(std::pair<std::string, JsonObject>(name, JsonObject()));
        }

        template <typename Type>
        void setChild(const char *name, Type value)
        {
            type = JsonObjectType::object;
            elements.clear();
            value.clear();
            childs.insert(std::pair<std::string, JsonObject>(name, JsonObject(value)));
        }

        bool hasChild(const char *name) const
        {
            return childs.find(name) != childs.end();
        }

        bool removeChild(const char *name)
        {
            childs.erase(name);
        }

        JsonObject &getChild(const char *name)
        {
            return childs.at(name);
        }

        JsonObject &getFirstChild() const;
        JsonObject &getNextChild() const;

        JsonObject &getArrayElement(unsigned elementIndex);
        unsigned getNumberOfArrayElements() const;
        void setNumberOfArrayElements(unsigned numberOfArrayElements);
};

#endif
