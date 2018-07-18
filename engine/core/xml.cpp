#include "xml.hpp"

#include <iterator>
#include <algorithm>

namespace sushi {

/*
void parse_xml_stream(XmlDocument& doc, std::istream& stream) {
    std::string xml_sources;

    std::copy(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>(), std::back_inserter(xml_sources));
    char* raw_text_buffer = new char[xml_sources.size()];

    doc.parse<0>(xml_sources.c_str());

    delete[]
}
 */

XmlDocument::XmlDocument() noexcept
: text_buffer{nullptr} {

}

XmlDocument::XmlDocument(std::istream& stream)
: text_buffer{nullptr} {
    std::string xml_sources;
    try {
        std::copy(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>(), std::back_inserter(xml_sources));
        text_buffer = new char[xml_sources.size()];
        std::copy(std::begin(xml_sources), std::end(xml_sources), text_buffer);
        document.parse<0>(text_buffer);
    }
    catch(...) {
        delete[] text_buffer;
        text_buffer = nullptr;
        throw;
    }
}

XmlDocument::~XmlDocument() noexcept {
    delete[] text_buffer;
}

XmlNode* XmlDocument::root() noexcept {
    return document.first_node();
}

}