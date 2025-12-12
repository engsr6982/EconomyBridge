#pragma once
#include "IEconomy.h"
#include "IEconomyProvider.h"
#include "econbridge/Concepts.h"

#include <memory>
#include <unordered_map>

namespace econbridge {

class EconomyBridge {
    std::unordered_map<std::string, IEconomyProvider::Ptr> providers_{};
    IEconomy::Ptr                                          default_{nullptr};

    EconomyBridge();
    bool registerProviderImpl(IEconomyProvider::Ptr provider);

public:
    EconomyBridge(EconomyBridge const&)            = delete;
    EconomyBridge& operator=(EconomyBridge const&) = delete;
    EconomyBridge(EconomyBridge&&)                 = delete;
    EconomyBridge& operator=(EconomyBridge&&)      = delete;

    [[nodiscard]] static EconomyBridge& getInstance();

    [[nodiscard]] bool hasProvider(std::string const& name) const;

    [[nodiscard]] std::vector<std::string> getProviders() const;

    template <concepts::EconomyProvider T>
    bool registerProvider(std::shared_ptr<T> provider) {
        return registerProviderImpl(provider);
    }

    template <concepts::EconomyProvider T, typename... Args>
    bool registerProvider(Args&&... args) {
        auto provider = std::make_shared<T>(std::forward<Args>(args)...);
        return registerProvider(provider);
    }

    bool unregisterProvider(std::string const& name);

    [[nodiscard]] IEconomyProvider::Ptr getProvider(std::string const& name) const;

    template <concepts::EconomyProvider T>
    [[nodiscard]] std::shared_ptr<T> getProvider(std::string const& name) const {
        return std::dynamic_pointer_cast<T>(getProvider(name));
    }

    [[nodiscard]] IEconomy::Ptr getDefault() const;

    void setDefault(IEconomy::Ptr economy);
};

} // namespace econbridge
