#pragma once
#include "econbridge/Concepts.h"
#include "econbridge/IEconomy.h"
#include "econbridge/IEconomyProvider.h"

#include <optional>

namespace econbridge::detail {

class LegacyMoneyEconomy final : public IEconomy {
public:
    static constexpr auto             BackendName = "LegacyMoney";
    static bool                       isLegacyMoneyLoaded();
    static std::optional<std::string> uuid2xuid(mce::UUID const& uuid);

    std::string_view getName() const override;
    int64_t          getBalance(const mce::UUID& uuid) const override;
    bool             setBalance(const mce::UUID& uuid, int64_t amount) override;
    bool             addBalance(const mce::UUID& uuid, int64_t amount) override;
    bool             reduceBalance(const mce::UUID& uuid, int64_t amount) override;
    bool             transfer(const mce::UUID& from, const mce::UUID& to, int64_t amount) override;
};

class LegacyMoneyEconomyProvider final : public IEconomyProvider,
                                         public ICreateFactory<LegacyMoneyEconomyProvider, LegacyMoneyEconomy> {
public:
    const std::string& getName() const override;
    bool               isAvailable() const override;
};
static_assert(concepts::EconomyProviderImpl<LegacyMoneyEconomyProvider, LegacyMoneyEconomy>);

} // namespace econbridge::detail
