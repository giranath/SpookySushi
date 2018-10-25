#include "job_config.hpp"

#include <thread>
#include <algorithm>
#include <iterator>
#include <cctype>

namespace sushi {

JobConfig::JobConfig() noexcept
: max_job_count(2048)
, worker_count(std::thread::hardware_concurrency()){

}

void from_xml(const XmlNode& xml_node, JobConfig& config) noexcept {
    XmlAttribute* worker_count = xml_node.first_attribute("worker_count");
    if(worker_count) {
        std::string worker_count_str{worker_count->value()};
        std::transform(std::begin(worker_count_str), std::end(worker_count_str), std::begin(worker_count_str), [](const char letter) {
            return std::tolower(letter);
        });

        if(worker_count_str == "default") {
            config.worker_count = std::thread::hardware_concurrency();
        }
        else {
            parse_xml_attribute<int>(xml_node.first_attribute("worker_count"), config.worker_count, std::thread::hardware_concurrency());
        }
    }

    parse_xml_attribute<int>(xml_node.first_attribute("max_job_count"), config.max_job_count, 2048);
}

void to_xml(XmlNode& xml_node, const JobConfig& config) noexcept {

}

}