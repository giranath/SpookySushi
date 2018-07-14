#include "profiler.hpp"
#include <fstream>
#include <iterator>
#include <iomanip>
#include <unistd.h>

namespace sushi { namespace debug {

std::ostream& operator<<(std::ostream& os, ProfileEvent::Type type) {
    switch(type) {
        case ProfileEvent::Type::start:
            os << "S";
            break;
        case ProfileEvent::Type::end:
            os << "E";
            break;
        case ProfileEvent::Type::instant:
            os << "I";
            break;
    }

    return os;
}

ProfileEvent::ProfileEvent(uint32_t name, Type t, uint64_t frame_index)
: ProfileEvent(name, t, Clock::now(), frame_index){

}

ProfileEvent::ProfileEvent(uint32_t name, Type t, Clock::time_point tp, uint64_t frame_index)
: ProfileEvent(name, t, tp, std::this_thread::get_id(), frame_index){

}

ProfileEvent::ProfileEvent(uint32_t name, Type t, Clock::time_point tp, std::thread::id thread_id, uint64_t frame_index)
: name(name)
, time_point_(tp)
, thread_id(thread_id)
, type_(t)
, frame_index{frame_index}
, padding{} {

}

std::ostream& operator<<(std::ostream& os, const ProfileEvent& ev) {
    return os << std::left << std::setw(15) << std::hex << ev.thread_id << std::dec
              << std::setw(15) << ev.frame_index
              << std::setw(20) << ev.time_point_.time_since_epoch().count()
              << std::setw(5) << ev.type_
              << std::setw(30) << ev.name << std::right
            ;

}

void Profiler::execute_background_task() {
    std::ofstream profile_out("profile.prof", std::ios::binary);

    profile_out << "session infos:\n"
                << "pid:    " << getpid() << "\n"
                << "system: " << SYSTEM_FULL_NAME << "\n"
                << "\n";

    profile_out << "session events:\n"
                << std::left << std::setw(15) << "thread_id"
                << std::setw(15) << "frame_index"
                << std::setw(20) << "time_point"
                << std::setw(5) << "type"
                << std::setw(30) << "name" << std::right << std::endl;

    while(is_running) {
        // Transfert every events to a local array
        std::array<ProfileEvent, 4096> to_write;
        auto end = events_queue.transfert(std::begin(to_write));

        if(std::distance(std::begin(to_write), end) > 0) {
            std::copy(std::begin(to_write), end, std::ostream_iterator<ProfileEvent>(profile_out, "\n"));
        }
        else {
            std::this_thread::yield();
        }
    }
}

Profiler::Profiler() noexcept
: background_thread{}
, events_queue{4096}
, is_running{false} {
}

Profiler& Profiler::get() noexcept {
    static Profiler instance;

    return instance;
}

void Profiler::start() noexcept {
    // Start background thread
    is_running = true;
    background_thread = std::thread(&Profiler::execute_background_task, std::ref(*this));
}

void Profiler::stop() noexcept {
    is_running = false;
    if(background_thread.joinable()) {
        background_thread.join();
    }
}

bool Profiler::push(const ProfileEvent& event) {
    const bool res = events_queue.push(event);

    if(!res) throw std::runtime_error{"queue is full"};

    return res;
}

bool Profiler::push_start(uint32_t name) {
    return push(ProfileEvent{name, ProfileEvent::Type::start, current_frame_index});
}

bool Profiler::push_end(uint32_t name) {
    return push(ProfileEvent{name, ProfileEvent::Type::end, current_frame_index});
}

bool Profiler::push_instant(uint32_t name) {
    return push(ProfileEvent{name, ProfileEvent::Type::instant, current_frame_index});
}

void Profiler::set_frame_index(uint64_t current_frame_index) noexcept {
    this->current_frame_index = current_frame_index;
}

ScopedProfile::ScopedProfile(uint32_t name) noexcept
: name(name) {
    Profiler::get().push_start(name);
}

ScopedProfile::~ScopedProfile() noexcept {
    Profiler::get().push_end(name);
}

}}