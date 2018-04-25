#include "job.hpp"

namespace sushi {

void job::finish() {
    --unfinish_count;

    if(finished()) {
        if(parent) {
            parent->finish();
        }
    }
}

job::job(job_function fn, job* parent)
: function(fn)
, parent(parent)
, unfinish_count(1) {
    if(parent) {
        parent->unfinish_count++;
    }
}

void job::run() {
    if(function) {
        function(*this);
    }
    finish();
}

bool job::finished() {
    return unfinish_count == 0;
}

}