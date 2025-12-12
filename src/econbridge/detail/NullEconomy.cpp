#include "NullEconomy.h"

namespace econbridge::detail {

std::string_view NullEconomy::getName() const { return BackendName; }
int64_t          NullEconomy::getBalance(const mce::UUID& uuid) const { return 0; }
bool             NullEconomy::setBalance(const mce::UUID& uuid, int64_t amount) { return true; }
bool             NullEconomy::addBalance(const mce::UUID& uuid, int64_t amount) { return true; }
bool             NullEconomy::reduceBalance(const mce::UUID& uuid, int64_t amount) { return true; }
bool             NullEconomy::transfer(const mce::UUID& from, const mce::UUID& to, int64_t amount) { return true; }

std::string NullEconomyProvider::getName() const { return NullEconomy::BackendName; }
bool        NullEconomyProvider::isAvailable() const { return true; }

} // namespace econbridge::detail