#include "LegacyMoneyEconomy.h"

#include "econbridge/EconomyBridge.h"

#include <ll/api/service/PlayerInfo.h>

#include <stdexcept>
#include <windows.h>

namespace econbridge::detail {

static const wchar_t* LegacyMoneyModuleName = L"LegacyMoney.dll";

using LLMoney_Get    = int64_t (*)(std::string);
using LLMoney_Set    = bool (*)(std::string, int64_t);
using LLMoney_Add    = bool (*)(std::string, int64_t);
using LLMoney_Reduce = bool (*)(std::string, int64_t);
using LLMoney_Trans  = bool (*)(std::string from, std::string to, int64_t val, const std::string& note);

bool LegacyMoneyEconomy::isLegacyMoneyLoaded() { return GetModuleHandle(LegacyMoneyModuleName) != nullptr; }
std::optional<std::string> LegacyMoneyEconomy::uuid2xuid(mce::UUID const& uuid) {
    if (auto entry = ll::service::PlayerInfo::getInstance().fromUuid(uuid)) {
        return entry->xuid;
    }
    return std::nullopt;
}

std::string_view LegacyMoneyEconomy::getName() const { return BackendName; }
int64_t          LegacyMoneyEconomy::getBalance(const mce::UUID& uuid) const {
    if (!isLegacyMoneyLoaded()) {
        throw std::runtime_error("LegacyMoney is not loaded.");
    }
    auto func = reinterpret_cast<LLMoney_Get>(GetProcAddress(GetModuleHandle(LegacyMoneyModuleName), "LLMoney_Get"));
    if (!func) {
        throw std::runtime_error("Dynamic call to LLMoney_Get failed.");
    }
    if (auto xuid = uuid2xuid(uuid)) {
        return func(*xuid);
    }
    return 0;
}
bool LegacyMoneyEconomy::setBalance(const mce::UUID& uuid, int64_t amount) {
    if (!isLegacyMoneyLoaded()) {
        throw std::runtime_error("LegacyMoney is not loaded.");
    }
    auto func = reinterpret_cast<LLMoney_Set>(GetProcAddress(GetModuleHandle(LegacyMoneyModuleName), "LLMoney_Set"));
    if (!func) {
        throw std::runtime_error("Dynamic call to LLMoney_Set failed.");
    }
    if (auto xuid = uuid2xuid(uuid)) {
        return func(*xuid, amount);
    }
    return false;
}
bool LegacyMoneyEconomy::addBalance(const mce::UUID& uuid, int64_t amount) {
    if (!isLegacyMoneyLoaded()) {
        throw std::runtime_error("LegacyMoney is not loaded.");
    }
    auto func = reinterpret_cast<LLMoney_Add>(GetProcAddress(GetModuleHandle(LegacyMoneyModuleName), "LLMoney_Add"));
    if (!func) {
        throw std::runtime_error("Dynamic call to LLMoney_Add failed.");
    }
    if (auto xuid = uuid2xuid(uuid)) {
        return func(*xuid, amount);
    }
    return false;
}
bool LegacyMoneyEconomy::reduceBalance(const mce::UUID& uuid, int64_t amount) {
    if (!isLegacyMoneyLoaded()) {
        throw std::runtime_error("LegacyMoney is not loaded.");
    }
    auto func =
        reinterpret_cast<LLMoney_Reduce>(GetProcAddress(GetModuleHandle(LegacyMoneyModuleName), "LLMoney_Reduce"));
    if (!func) {
        throw std::runtime_error("Dynamic call to LLMoney_Reduce failed.");
    }
    if (auto xuid = uuid2xuid(uuid)) {
        return func(*xuid, amount);
    }
    return false;
}
bool LegacyMoneyEconomy::transfer(const mce::UUID& from, const mce::UUID& to, int64_t amount) {
    if (!isLegacyMoneyLoaded()) {
        throw std::runtime_error("LegacyMoney is not loaded.");
    }
    auto func =
        reinterpret_cast<LLMoney_Trans>(GetProcAddress(GetModuleHandle(LegacyMoneyModuleName), "LLMoney_Trans"));
    if (!func) {
        throw std::runtime_error("Dynamic call to LLMoney_Trans failed.");
    }
    auto uf = uuid2xuid(from), ut = uuid2xuid(to);
    if (!uf || !ut) {
        return false;
    }
    return func(*uf, *ut, amount, "");
}


std::string LegacyMoneyEconomyProvider::getName() const { return LegacyMoneyEconomy::BackendName; }
bool        LegacyMoneyEconomyProvider::isAvailable() const { return LegacyMoneyEconomy::isLegacyMoneyLoaded(); }

} // namespace econbridge::detail