#ifndef SPOOKYSUSHI_ENTITY_ITERATOR_HPP
#define SPOOKYSUSHI_ENTITY_ITERATOR_HPP

#include "storage_traits.hpp"
#include <tuple>
#include <iterator>

namespace sushi {

template<typename T, typename... Ts>
auto car(std::tuple<T, Ts...> t) {
    return std::get<0>(t);
}

template<std::size_t... Ns, typename... Ts>
auto cdr_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t) {
    return std::make_tuple(std::get<Ns+1u>(t)...);
}

template<typename... Ts>
auto cdr(std::tuple<Ts...> t) {
    return cdr_impl(std::make_index_sequence<sizeof...(Ts) - 1u>(), t);
}

template<typename T>
struct as_ref {
    using type = T&;
};

namespace ecs {

template<typename... StorageTypes>
class entity_iterator : public std::iterator<std::forward_iterator_tag,
                                             std::pair<const entity,
                                             std::tuple<typename storage_traits<StorageTypes>::component_type&...>>> {
public:
    using components_type = std::tuple<typename storage_traits<StorageTypes>::component_type &...>;
    using pair_type = std::pair<const entity, components_type>;
private:
    std::tuple<StorageTypes*...> storages_;
    std::tuple<typename storage_traits<StorageTypes>::iterator...> iterators;
    alignas(pair_type) char raw_components[sizeof(pair_type)];
    bool is_ended;

    template<typename... Ts>
    bool are_identicals(std::tuple<Ts...> tuple, entity e) noexcept {
        return std::get<0>(tuple)->first == e && are_identicals(cdr(tuple), e);
    }

    template<typename T>
    bool are_identicals(std::tuple<T> tuple, entity e) noexcept {
        return std::get<0>(tuple)->first == e;
    }

    bool are_identicals(std::tuple<> tuple, entity e) noexcept {
        return true;
    }

    bool are_identicals() noexcept {
        return are_identicals(cdr(iterators), std::get<0>(iterators)->first);
    }

    template<typename... Ts, typename... Ss>
    bool are_at_end(std::tuple<Ts...> iterator, std::tuple<Ss...> storage) noexcept {
        auto it = std::get<0>(iterator);
        auto stora = std::get<0>(storage);

        return it == stora->end() || are_at_end(cdr(iterator), cdr(storage));
    }

    template<typename Ts, typename Ss>
    bool are_at_end(std::tuple<Ts> iterator, std::tuple<Ss> storage) noexcept {
        return std::get<0>(iterator) == std::get<0>(storage)->end();
    }

    bool are_at_end() noexcept {
        return are_at_end(iterators, storages_);
    }

    template<typename... Ts>
    entity get_max_entity(std::tuple<Ts...> tuple) noexcept {
        return std::max(std::get<0>(tuple)->first, get_max_entity(cdr(tuple)));
    }

    template<typename Ts>
    entity get_max_entity(std::tuple<Ts> tuple) noexcept {
        return std::get<0>(tuple)->first;
    }

    entity get_max_entity() noexcept {
        return get_max_entity(iterators);
    }

    template<typename... Ts>
    std::tuple<Ts...> increment_lower(std::tuple<Ts...> tuple, entity e) noexcept {
        if(std::get<0>(tuple)->first < e) {
            (std::get<0>(tuple))++;
        }

        return std::tuple_cat(std::make_tuple(std::get<0>(tuple)), increment_lower(cdr(tuple), e));
    }

    template<typename Ts>
    std::tuple<Ts> increment_lower(std::tuple<Ts> tuple, entity e) noexcept {
        if(std::get<0>(tuple)->first < e) {
            (std::get<0>(tuple))++;
        }

        return tuple;
    }

    template<typename... Ts>
    std::tuple<typename std::remove_reference<typename std::iterator_traits<Ts>::value_type::second_type>::type&...> get_components_refs(std::tuple<Ts...> tuple) noexcept {
        return std::tuple_cat(std::make_tuple(std::ref(std::get<0>(tuple)->second)), get_components_refs(cdr(tuple)));
    }

    template<typename Ts>
    std::tuple<typename std::remove_reference<typename std::iterator_traits<Ts>::value_type::second_type>::type&> get_components_refs(std::tuple<Ts> tuple) noexcept {
        return std::make_tuple(std::ref(std::get<0>(tuple)->second));
    }

    void increment_lower(entity e) {
        iterators = increment_lower(iterators, e);
    }

    void next() {
        if(are_identicals()) {
            ++std::get<0>(iterators);
        }

        while(!are_at_end() && !are_identicals()) {
            const entity max_entity = get_max_entity();
            increment_lower(max_entity);
        }

        if(are_at_end()) {
            is_ended = true;
        }
        else {
            update_components();
        }
    }

    void update_components() {
        new(raw_components) std::pair<const entity, components_type>(std::get<0>(iterators)->first,
                                                                     get_components_refs(iterators));
    }

public:
    using components = std::tuple<typename storage_traits<StorageTypes>::component_type&...>;

    explicit entity_iterator(StorageTypes&... storages)
    : storages_{std::make_tuple(&storages...)}
    , iterators{std::make_tuple(storages.begin()...)}
    , is_ended{false} {
        if(!are_identicals()) {
            next();
        }
        else {
            update_components();
        }
    }

    entity_iterator& operator++() {
        next();
        return *this;
    }

    entity_iterator operator++(int) {
        entity_iterator old = *this;

        ++(*this);

        return old;
    }

    bool operator==(const entity_iterator& other) const noexcept {
        return is_ended == other.is_ended;
    }

    bool operator!=(const entity_iterator& other) const noexcept {
        return is_ended != other.is_ended;
    }

    pair_type& operator*() {
        return *reinterpret_cast<pair_type*>(raw_components);
    }

    pair_type* operator->() {
        return reinterpret_cast<pair_type*>(raw_components);
    }

    friend entity_iterator begin(entity_iterator it) noexcept {
        return it;
    }

    friend entity_iterator end(const entity_iterator& it) noexcept {
        entity_iterator e = it;
        e.is_ended = true;

        return e;
    }
};

}}

#endif //SPOOKYSUSHI_ENTITY_ITERATOR_HPP
