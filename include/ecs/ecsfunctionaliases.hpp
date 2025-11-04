#pragma once

#include <any>
#include <functional>

class ECSContext;

using SystemFn = std::function<void(ECSContext &, float)>;

template <typename CommandType>
using CommandHandler = std::function<void(ECSContext &, const CommandType &)>;

template <typename EventType>
using EventHandler = std::function<void(ECSContext &, const EventType &)>;

using CommandHandlerAny = std::function<void(ECSContext &, const std::any &)>;

using EventListenerAny = std::function<void(ECSContext &, const std::any &)>;

using MemberSetter = std::function<void(std::any &)>;
