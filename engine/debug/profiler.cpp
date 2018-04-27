#include "profiler.hpp"
#include <fstream>
#include <iterator>
#include <iomanip>

namespace sushi { namespace debug {

std::ostream& operator<<(std::ostream& os, profile_event::type type) {
    switch(type) {
        case profile_event::type::start:
            os << "S";
            break;
        case profile_event::type::end:
            os << "E";
            break;
        case profile_event::type::instant:
            os << "I";
            break;
    }

    return os;
}

profile_event::profile_event(uint32_t name, type t)
: profile_event(name, t, clock::now()){

}

profile_event::profile_event(uint32_t name, type t, clock::time_point tp)
: profile_event(name, t, tp, std::this_thread::get_id()){

}

profile_event::profile_event(uint32_t name, type t, clock::time_point tp, std::thread::id thread_id)
: name(name)
, time_point_(tp)
, thread_id(thread_id)
, type_(t)
, padding{} {

}

std::ostream& operator<<(std::ostream& os, const profile_event& ev) {
    return os << std::left << std::setw(15) << std::hex << ev.thread_id << std::dec
              << std::setw(20) << ev.time_point_.time_since_epoch().count()
              << std::setw(5) << ev.type_
              << std::setw(30) << ev.name << std::right
            ;

}

void profiler::execute_background_task() {
    std::ofstream profile_out("profile.prof", std::ios::binary);

    // Write file header

    profile_out << std::left << std::setw(15) << "thread_id"
                << std::setw(20) << "time_point"
                << std::setw(5) << "type"
                << std::setw(30) << "name" << std::right << std::endl;

    while(is_running) {
        // Transfert every events to a local array
        std::array<profile_event, 4096> to_write;
        auto end = events_queue.transfert(std::begin(to_write));

        if(std::distance(std::begin(to_write), end) > 0) {
            std::copy(std::begin(to_write), end, std::ostream_iterator<profile_event>(profile_out, "\n"));
        }
        else {
            std::this_thread::yield();
        }
    }
}

profiler::profiler() noexcept
: background_thread{}
, events_queue{4096}
, is_running{false} {
}

profiler& profiler::get() noexcept {
    static profiler instance;

    return instance;
}

void profiler::start() noexcept {
    // Start background thread
    is_running = true;
    background_thread = std::thread(&profiler::execute_background_task, std::ref(*this));
}

void profiler::stop() noexcept {
    is_running = false;
    if(background_thread.joinable()) {
        background_thread.join();
    }
}

bool profiler::push(const profile_event& event) {
    const bool res = events_queue.push(event);

    if(!res) throw std::runtime_error{"queue is full"};

    return res;
}

}}