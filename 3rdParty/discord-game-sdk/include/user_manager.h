#pragma once

#include <memory>

#include "types.h"

namespace discord {

class UserManager final {
public:
    ~UserManager() = default;

    Result GetCurrentUser(User* currentUser);
    std::tuple<Result, std::shared_ptr<User>> GetCurrentUser();

    void GetUser(UserId userId, std::function<void(Result, User const&)> callback);

    Result GetCurrentUserPremiumType(PremiumType* premiumType);
    std::tuple<Result, std::shared_ptr<PremiumType>> GetCurrentUserPremiumType();

    Result CurrentUserHasFlag(UserFlag flag, bool* hasFlag);
    std::tuple<Result, bool> CurrentUserHasFlag(UserFlag flag);

    Event<> OnCurrentUserUpdate;

private:
    friend class Core;

    UserManager() = default;
    UserManager(UserManager const& rhs) = delete;
    UserManager& operator=(UserManager const& rhs) = delete;
    UserManager(UserManager&& rhs) = delete;
    UserManager& operator=(UserManager&& rhs) = delete;

    IDiscordUserManager* internal_;
    static IDiscordUserEvents events_;
};

} // namespace discord
