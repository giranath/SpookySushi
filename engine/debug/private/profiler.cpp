#include "profiler.hpp"
#include "../../driver/process/process.hpp"

#include <fstream>
#include <iterator>
#include <iomanip>

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

#ifdef SUSHI_ENABLE_PROFILING
ProfileEvent::ProfileEvent(uint32_t name, Type t, uint64_t frame_index)
: ProfileEvent(name, t, Clock::now(), frame_index){

}
#else
ProfileEvent::ProfileEvent(uint32_t name, Type t, uint64_t frame_index) {}
#endif

#ifdef SUSHI_ENABLE_PROFILING
ProfileEvent::ProfileEvent(uint32_t name, Type t, Clock::time_point tp, uint64_t frame_index)
: ProfileEvent(name, t, tp, std::this_thread::get_id(), frame_index){

}
#else
ProfileEvent::ProfileEvent(uint32_t name, Type t, Clock::time_point tp, uint64_t frame_index) {}
#endif

#ifdef SUSHI_ENABLE_PROFILING
ProfileEvent::ProfileEvent(uint32_t name, Type t, Clock::time_point tp, std::thread::id thread_id, uint64_t frame_index)
: name(name)
, time_point_(tp)
, thread_id(thread_id)
, type_(t)
, frame_index{frame_index}
, padding{} {

}
#else
ProfileEvent::ProfileEvent(uint32_t name, Type t, Clock::time_point tp, std::thread::id thread_id, uint64_t frame_index) {}
#endif

std::ostream& operator<<(std::ostream& os, const ProfileEvent& ev) {
#ifdef SUSHI_ENABLE_PROFILING
    return os << std::left << std::setw(15) << std::hex << ev.thread_id << std::dec
              << std::setw(15) << ev.frame_index
              << std::setw(20) << ev.time_point_.time_since_epoch().count()
              << std::setw(5) << ev.type_
              << std::setw(30) << ev.name << std::right
            ;
#else
    return os;
#endif

}

void Profiler::execute_background_task() {
#ifdef SUSHI_ENABLE_PROFILING
    std::ofstream profile_out("profile.prof", std::ios::binary);

    profile_out << "session infos:\n"
                << "pid:    " << driver::process::current_process_id() << "\n"
                << "system: " << SYSTEM_FULL_NAME << "\n"
                << "\n";

    profile_out << "thread identities:\n";
    for(const auto& pair : thread_names) {
        profile_out << std::left << std::setw(15) << pair.first
                    << " : "
                    << "\"" << pair.second << "\"\n";
    }
    profile_out << "\n";

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
#endif
}

#ifdef SUSHI_ENABLE_PROFILING
Profiler::Profiler() noexcept
: background_thread{}
, events_queue{4096}
, is_running{false} {
}
#else
Profiler::Profiler() noexcept {}
#endif

Profiler& Profiler::get() noexcept {
    static Profiler instance;

    return instance;
}

void Profiler::start() noexcept {
#ifdef SUSHI_ENABLE_PROFILING
    // Start background thread
    is_running = true;
    background_thread = std::thread(&Profiler::execute_background_task, std::ref(*this));
#endif
}

void Profiler::stop() noexcept {
#ifdef SUSHI_ENABLE_PROFILING
    is_running = false;
    if(background_thread.joinable()) {
        background_thread.join();
    }
#endif
}

void Profiler::identify(std::thread::id thread, const std::string& name) {
#ifdef SUSHI_ENABLE_PROFILING
    thread_names[thread] = name;
#endif
}

void Profiler::identify(std::thread::id thread, std::string&& name) {
#ifdef SUSHI_ENABLE_PROFILING
    thread_names[thread] = std::move(name);
#endif
}

bool Profiler::push(const ProfileEvent& event) {
#ifdef SUSHI_ENABLE_PROFILING
    const bool res = events_queue.push(event);

    if(!res) throw std::runtime_error{"queue is full"};

    return res;
#else
    return true;
#endif
}

bool Profiler::push_start(uint32_t name) {
#ifdef SUSHI_ENABLE_PROFILING
    return push(ProfileEvent{name, ProfileEvent::Type::start, current_frame_index});
#else
    return true;
#endif
}

bool Profiler::push_end(uint32_t name) {
#ifdef SUSHI_ENABLE_PROFILING
    return push(ProfileEvent{name, ProfileEvent::Type::end, current_frame_index});
#else
    return true;
#endif
}

bool Profiler::push_instant(uint32_t name) {
#ifdef SUSHI_ENABLE_PROFILING
    return push(ProfileEvent{name, ProfileEvent::Type::instant, current_frame_index});
#else
    return true;
#endif
}

void Profiler::set_frame_index(uint64_t current_frame_index) noexcept {
#ifdef SUSHI_ENABLE_PROFILING
    this->current_frame_index = current_frame_index;
#endif
}

#ifdef SUSHI_ENABLE_PROFILING
ScopedProfile::ScopedProfile(uint32_t name) noexcept
: name(name) {
    Profiler::get().push_start(name);
}
#else
ScopedProfile::ScopedProfile(uint32_t name) noexcept {}
#endif

#ifdef SUSHI_ENABLE_PROFILING
ScopedProfile::~ScopedProfile() noexcept {
    Profiler::get().push_end(name);
}
#else
ScopedProfile::~ScopedProfile() noexcept {}
#endif

}}