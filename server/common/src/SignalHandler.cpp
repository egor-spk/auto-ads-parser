#include "include/SignalHandler.h"
#include "include/log.h"

#include <iostream>
#include <csignal>
#include <utility>
#include <cstring>
#include <fmt/format.h>

static const char *getSignalName(int sig)
{
    switch (sig)
    {
        case SIGINT:
            return "SIGINT";
        case SIGTERM:
            return "SIGTERM";
        default:
            return "Unknown signal";
    }
};

static void sigHandler(int sig)
{
    std::cout << "Received signal " << getSignalName(sig) << ", but not handled" << std::endl;
}

SignalHandler::SignalHandler()
{
    // перехватываем сигналы SIGINT и SIGTERM
    if (signal(SIGINT, sigHandler) == SIG_ERR)
        throw SignalHandlerError("Unable set SIGINT handler");
    if (signal(SIGTERM, sigHandler) == SIG_ERR)
        throw SignalHandlerError("Unable set SIGTERM handler");

    sigemptyset(&signalSet);
    sigaddset(&signalSet, SIGINT);
    sigaddset(&signalSet, SIGTERM);
}

void SignalHandler::setSigintHandler(std::function<void()> handler) noexcept
{
    sigint_ = std::move(handler);
}

void SignalHandler::setSigtermHandler(std::function<void()> handler) noexcept
{
    sigterm_ = std::move(handler);
}

void SignalHandler::wait()
{
    int sig{};
    bool isWait{true};
    auto callHandler = [&sig, &isWait](const std::function<void()> &handler)
    {
        isWait = false;
        if (handler)
        {
            handler();
        } else
        {
            LOG_WARN("Handler not set for signal {}", getSignalName(sig));
            isWait = true;
        }
    };

    while (isWait)
    {
        int sigwaitRes = sigwait(&signalSet, &sig); // ждем сигнал

        // Получили сигнал или ошибка
        if (sigwaitRes)
        {
            throw SignalHandlerError(fmt::format("Call sigwait return error: {}", strerror(errno)));
        } else
        {
            LOG_INFO("Received signal {}", getSignalName(sig));

            switch (sig)
            {
                case SIGINT:
                    callHandler(sigint_);
                    break;
                case SIGTERM:
                    callHandler(sigterm_);
                    break;
                default:
                    throw SignalHandlerError(fmt::format("Not expected signal received: {}", sig));
            }
        }
    }
}
