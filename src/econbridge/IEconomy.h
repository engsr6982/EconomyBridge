#pragma once
#include <cstdint>
#include <memory>
#include <string>

namespace mce {
class UUID;
}

namespace econbridge {

class IEconomy {
public:
    using Ptr = std::shared_ptr<IEconomy>;

    virtual ~IEconomy() = default;

    // 唯一标识，返回经济系统名称
    virtual std::string_view getName() const = 0;

    // 获取玩家余额
    virtual int64_t getBalance(mce::UUID const& uuid) const = 0;

    // 设置玩家余额
    virtual bool setBalance(mce::UUID const& uuid, int64_t amount) = 0;

    // 增加玩家余额
    virtual bool addBalance(mce::UUID const& uuid, int64_t amount) = 0;

    // 扣除玩家余额
    virtual bool reduceBalance(mce::UUID const& uuid, int64_t amount) = 0;

    // 转账
    virtual bool transfer(mce::UUID const& from, mce::UUID const& to, int64_t amount) = 0;
};

} // namespace econbridge