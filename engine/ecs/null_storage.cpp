#include "null_storage.hpp"

namespace sushi { namespace ecs {

void null_storage::iterator::update_pair() {
    if(owner && it != owner->entities.end()) {
        new(raw_pair) std::pair<const entity, null_component>{*it, null_component{}};
    }
}

null_storage::iterator::iterator(const null_storage* owner, std::set<entity>::iterator it) noexcept
: it{it}
, owner{owner} {
    update_pair();
}

null_storage::iterator::iterator() noexcept
: it{}
, owner{} {

}

null_storage::iterator& null_storage::iterator::operator++() {
    ++it;

    update_pair();

    return *this;
}

null_storage::iterator null_storage::iterator::operator++(int) {
    iterator temp = *this;

    ++(*this);

    return temp;
}

bool null_storage::iterator::operator==(const iterator& other) noexcept {
    return owner == other.owner && it == other.it;
}

bool null_storage::iterator::operator!=(const iterator& other) noexcept {
    return owner != other.owner || it != other.it;
}

null_storage::iterator::reference null_storage::iterator::operator*() noexcept {
    return *reinterpret_cast<pair*>(raw_pair);
}

null_storage::iterator::pointer null_storage::iterator::operator->() noexcept {
    return reinterpret_cast<pair*>(raw_pair);
}

void null_storage::clear() {
    entities.clear();
}

void null_storage::insert(entity e) {
    entities.insert(e);
}

null_storage::iterator null_storage::find(entity e) noexcept {
    return iterator{this, entities.find(e)};
}

null_storage::const_iterator null_storage::find(entity e) const noexcept {
    return const_iterator{this, entities.find(e)};
}

null_storage::iterator null_storage::begin() noexcept {
    return iterator{this, entities.begin()};
}

null_storage::const_iterator null_storage::begin() const noexcept {
    return const_iterator{this, entities.begin()};
}

null_storage::const_iterator null_storage::cbegin() const noexcept {
    return const_iterator{this, entities.begin()};
}

null_storage::iterator null_storage::end() noexcept {
    return iterator{this, entities.end()};
}

null_storage::const_iterator null_storage::end() const noexcept {
    return const_iterator{this, entities.end()};
}

null_storage::const_iterator null_storage::cend() const noexcept {
    return const_iterator{this, entities.end()};
}


}}