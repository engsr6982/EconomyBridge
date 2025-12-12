#pragma once
#include <concepts>
#include <memory>

namespace econbridge {

class IEconomy;
class IEconomyProvider;

namespace concepts {

template <typename T>
concept EconomyImpl = std::derived_from<T, IEconomy> && std::is_final_v<T>;

template <typename T>
concept EconomyProvider = std::derived_from<T, IEconomyProvider> && std::is_final_v<T>;

// T: EconomyProvider
// E: EconomyImpl
template <typename T, typename E>
concept EconomyProviderImpl = EconomyProvider<T> && EconomyImpl<E> && requires(T t) {
    { t.create() } -> std::same_as<std::shared_ptr<E>>;
};

} // namespace concepts

} // namespace econbridge