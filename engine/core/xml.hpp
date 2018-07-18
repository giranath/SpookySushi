#ifndef SPOOKYSUSHI_XML_HPP
#define SPOOKYSUSHI_XML_HPP

#include "string_parser_helper.hpp"
#include <rapidxml.hpp>

#include <string>
#include <iostream>

namespace sushi {

using XmlNode = rapidxml::xml_node<>;
using XmlAttribute = rapidxml::xml_attribute<>;
//using XmlDocument = rapidxml::xml_document<>;

class XmlDocument {
    char* text_buffer;
    rapidxml::xml_document<> document;
public:
    XmlDocument() noexcept;
    explicit XmlDocument(std::istream& stream);
    ~XmlDocument() noexcept;

    XmlNode* root() noexcept;
};

template<typename Type>
void parse_xml_attribute(XmlAttribute* attribute, Type& value, const Type& default_value = {}) {
    if(attribute) {
        try {
            value = StringParserHelper<Type>::parse(std::string{attribute->value()});
        }
        catch (const std::exception &exception) {
            value = default_value;
        }
    }
    else {
        value = default_value;
    }
}

//void parse_xml_stream(XmlDocument& doc, std::istream& stream);

}

#endif //SPOOKYSUSHI_XML_HPP
