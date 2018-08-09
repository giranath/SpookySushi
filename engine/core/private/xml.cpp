#include "xml.hpp"

#include <iterator>
#include <algorithm>

namespace sushi {

XmlDocument::XmlDocument() noexcept
: text_buffer{nullptr} {

}

XmlDocument::XmlDocument(std::istream& stream)
: text_buffer{nullptr} {
    std::string xml_sources;
    try {
        std::copy(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>(), std::back_inserter(xml_sources));

        text_buffer = new char[xml_sources.size() + 1];
        std::fill(text_buffer, text_buffer + xml_sources.size() + 1, 0);

        std::copy(std::begin(xml_sources), std::end(xml_sources), text_buffer);
        document.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(text_buffer);
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