#include "EconomyBridge.h"

#include "detail/LegacyMoneyEconomy.h"
#include "detail/NullEconomy.h"
#include "detail/ScoreboardEconomy.h"

namespace econbridge {

EconomyBridge::EconomyBridge() {
    registerProvider<detail::NullEconomyProvider>();
    registerProvider<detail::LegacyMoneyEconomyProvider>();
    registerProvider<detail::ScoreboardEconomyProvider>();
}
bool EconomyBridge::registerProviderImpl(IEconomyProvider::Ptr provider) {
    if (!provider || providers_.contains(provider->getName())) {
        return false;
    }
    providers_.emplace(provider->getName(), std::move(provider));
    return true;
}
EconomyBridge& EconomyBridge::getInstance() {
    static EconomyBridge instance;
    return instance;
}
bool                     EconomyBridge::hasProvider(std::string const& name) const { return providers_.contains(name); }
std::vector<std::string> EconomyBridge::getProviders() const {
    std::vector<std::string> names;
    for (auto const& [name, _] : providers_) {
        names.push_back(name);
    }
    return names;
}
bool EconomyBridge::unregisterProvider(std::string const& name) {
    if (!hasProvider(name)) {
        return false;
    }
    providers_.erase(name);
    if (default_->getName() == name) {
        default_.reset();
        default_ = nullptr;
    }
    return true;
}
IEconomyProvider::Ptr EconomyBridge::getProvider(std::string const& name) const {
    if (!hasProvider(name)) {
        return nullptr;
    }
    return providers_.at(name);
}
IEconomy::Ptr EconomyBridge::getDefault() const { return default_; }
void          EconomyBridge::setDefault(IEconomy::Ptr economy) {
    default_.reset();
    default_ = std::move(economy);
}

} // namespace econbridge
