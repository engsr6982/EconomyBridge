#pragma once
#include "econbridge/Concepts.h"
#include "econbridge/IEconomy.h"
#include "econbridge/IEconomyProvider.h"

#include <functional>


struct ScoreboardId;
class Scoreboard;
class Player;
namespace econbridge ::detail {

class ScoreboardEconomy final : public IEconomy {
    std::string scoreBoardName_;

public:
    static constexpr auto      BackendName = "ScoreBoard";
    static Player*             uuid2player(mce::UUID const& uuid);
    static ScoreboardId const& getScoreboardId(Scoreboard& scoreboard, Player& player);

    explicit ScoreboardEconomy(std::string scoreBoardName);

    std::string_view getName() const override;
    int64_t          getBalance(const mce::UUID& uuid) const override;
    bool             setBalance(const mce::UUID& uuid, int64_t amount) override;
    bool             addBalance(const mce::UUID& uuid, int64_t amount) override;
    bool             reduceBalance(const mce::UUID& uuid, int64_t amount) override;
    bool             transfer(const mce::UUID& from, const mce::UUID& to, int64_t amount) override;
};


class ScoreboardEconomyProvider final : public IEconomyProvider,
                                        public ICreateFactory<ScoreboardEconomyProvider, ScoreboardEconomy> {
public:
    const std::string& getName() const override;
    bool               isAvailable() const override;
};
static_assert(concepts::EconomyImpl<ScoreboardEconomy>);
static_assert(concepts::EconomyProvider<ScoreboardEconomyProvider>);
static_assert(concepts::EconomyProviderImpl<ScoreboardEconomyProvider, ScoreboardEconomy>);

} // namespace econbridge::detail
