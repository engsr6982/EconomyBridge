#pragma once
#include "econbridge/Concepts.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace econbridge {


class IEconomyProvider {
public:
    using Ptr = std::shared_ptr<IEconomyProvider>;

    virtual ~IEconomyProvider() = default;

    [[nodiscard]] virtual std::string getName() const = 0;

    [[nodiscard]] virtual bool isAvailable() const = 0;
};


template <typename Self, concepts::EconomyImpl Impl>
class ICreateFactory {
public:
    template <typename... Args>
    auto create(Args&&... args) -> std::shared_ptr<Impl> {
        static_assert(concepts::EconomyProvider<Self>);

        auto thiz = static_cast<Self*>(this);
        if (!thiz->isAvailable()) {
            return nullptr;
        }
        return std::make_shared<Impl>(std::forward<Args>(args)...);
    }
};


} // namespace econbridge