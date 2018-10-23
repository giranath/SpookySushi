#ifndef SPOOKYSUSHI_JOB_CONFIG_HPP
#define SPOOKYSUSHI_JOB_CONFIG_HPP

#include <xml.hpp>

namespace sushi {

struct JobConfig {
    int max_job_count;
    int worker_count;

    JobConfig() noexcept;
};

void from_xml(const XmlNode& xml_node, JobConfig& config) noexcept;
void to_xml(XmlNode& xml_node, const JobConfig& config) noexcept;

}


#endif //SPOOKYSUSHI_JOB_CONFIG_HPP
