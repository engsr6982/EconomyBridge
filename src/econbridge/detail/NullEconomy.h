#pragma once
#include "econbridge/IEconomy.h"
#include "econbridge/IEconomyProvider.h"

namespace econbridge::detail {

/**
 * @note This economy does nothing.
 */
class NullEconomy final : public IEconomy {
public:
    static constexpr auto BackendName = "NullEconomy";

    std::string_view getName() const override;
    int64_t          getBalance(const mce::UUID& uuid) const override;
    bool             setBalance(const mce::UUID& uuid, int64_t amount) override;
    bool             addBalance(const mce::UUID& uuid, int64_t amount) override;
    bool             reduceBalance(const mce::UUID& uuid, int64_t amount) override;
    bool             transfer(const mce::UUID& from, const mce::UUID& to, int64_t amount) override;
};

class NullEconomyProvider final : public IEconomyProvider, public ICreateFactory<NullEconomyProvider, NullEconomy> {
public:
    [[nodiscard]] std::string getName() const override;
    [[nodiscard]] bool        isAvailable() const override;
};

} // namespace econbridge::detail
