#include "ScoreboardEconomy.h"

#include <ll/api/service/Bedrock.h>

#include "mc/world/level/Level.h"
#include "mc/world/scores/PlayerScoreSetFunction.h"
#include "mc/world/scores/ScoreInfo.h"
#include "mc/world/scores/ScoreboardOperationResult.h"
#include <mc/world/actor/player/Player.h>
#include <mc/world/scores/Objective.h>
#include <mc/world/scores/Scoreboard.h>
#include <mc/world/scores/ScoreboardId.h>

namespace econbridge ::detail {


Player* ScoreboardEconomy::uuid2player(mce::UUID const& uuid) {
    return ll::service::getLevel().transform([&](Level& level) { return level.getPlayer(uuid); });
}
ScoreboardId const& ScoreboardEconomy::getScoreboardId(Scoreboard& scoreboard, Player& player) {
    ScoreboardId const& id = scoreboard.getScoreboardId(player);
    if (id.mRawID == ScoreboardId::INVALID().mRawID) {
        return scoreboard.createScoreboardId(player);
    }
    return id;
}

ScoreboardEconomy::ScoreboardEconomy(std::string scoreBoardName) { this->scoreBoardName_ = std::move(scoreBoardName); }

std::string_view ScoreboardEconomy::getName() const { return BackendName; }
int64_t          ScoreboardEconomy::getBalance(const mce::UUID& uuid) const {
    Scoreboard& scoreboard = ll::service::getLevel()->getScoreboard();
    Objective*  objective  = scoreboard.getObjective(scoreBoardName_);
    if (!objective) {
        return 0;
    }
    auto player = uuid2player(uuid);
    if (!player) {
        return 0;
    }
    return objective->getPlayerScore(getScoreboardId(scoreboard, *player)).mValue;
}
bool ScoreboardEconomy::setBalance(const mce::UUID& uuid, int64_t amount) {
    Scoreboard& scoreboard = ll::service::getLevel()->getScoreboard();
    Objective*  objective  = scoreboard.getObjective(scoreBoardName_);
    if (!objective) {
        return false;
    }
    auto player = uuid2player(uuid);
    if (!player) {
        return false;
    }
    ScoreboardOperationResult result;
    scoreboard.modifyPlayerScore(
        result,
        getScoreboardId(scoreboard, *player),
        *objective,
        static_cast<int>(amount),
        PlayerScoreSetFunction::Set
    );
    return result == ScoreboardOperationResult::Success;
}
bool ScoreboardEconomy::addBalance(const mce::UUID& uuid, int64_t amount) {
    Scoreboard& scoreboard = ll::service::getLevel()->getScoreboard();
    Objective*  objective  = scoreboard.getObjective(scoreBoardName_);
    if (!objective) {
        return false;
    }
    auto player = uuid2player(uuid);
    if (!player) {
        return false;
    }
    ScoreboardOperationResult result;
    scoreboard.modifyPlayerScore(
        result,
        getScoreboardId(scoreboard, *player),
        *objective,
        static_cast<int>(amount),
        PlayerScoreSetFunction::Add
    );
    return result == ScoreboardOperationResult::Success;
}
bool ScoreboardEconomy::reduceBalance(const mce::UUID& uuid, int64_t amount) {
    Scoreboard& scoreboard = ll::service::getLevel()->getScoreboard();
    Objective*  objective  = scoreboard.getObjective(scoreBoardName_);
    if (!objective) {
        return false;
    }
    auto player = uuid2player(uuid);
    if (!player) {
        return false;
    }
    ScoreboardOperationResult result;
    scoreboard.modifyPlayerScore(
        result,
        getScoreboardId(scoreboard, *player),
        *objective,
        static_cast<int>(amount),
        PlayerScoreSetFunction::Subtract
    );
    return result == ScoreboardOperationResult::Success;
}
bool ScoreboardEconomy::transfer(const mce::UUID& from, const mce::UUID& to, int64_t amount) {
    if (reduceBalance(from, amount)) {
        if (addBalance(to, amount)) {
            return true;
        }
        if (!addBalance(from, amount)) {
            throw std::runtime_error("Failed to restore balance after transfer.");
        }
    }
    return false;
}

std::string ScoreboardEconomyProvider::getName() const { return ScoreboardEconomy::BackendName; }
bool        ScoreboardEconomyProvider::isAvailable() const { return true; }


} // namespace econbridge::detail