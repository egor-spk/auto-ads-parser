#ifndef AUTO_ADS_PARSER_SIGNALHANDLER_H
#define AUTO_ADS_PARSER_SIGNALHANDLER_H

#include <functional>

/**
 * @brief Реализация приостановки потока до получения системного сигнала
 */
class SignalHandler
{
public:
    explicit SignalHandler();

    void setSigintHandler(std::function<void()> handler) noexcept;

    void setSigtermHandler(std::function<void()> handler) noexcept;

    void wait();

private:
    sigset_t signalSet{};
    std::function<void()> sigint_;
    std::function<void()> sigterm_;
};

/**
 * @brief Ошибка обработчика сигналов
 */
class SignalHandlerError : public std::exception
{
public:
    explicit SignalHandlerError(const char *msg) : msg_{msg} {}

    explicit SignalHandlerError(std::string msg) : msg_{std::move(msg)} {}

    const char *what() const noexcept override { return msg_.c_str(); }

    const std::string &getMsg() const noexcept { return msg_; }

private:
    std::string msg_;
};

#endif //AUTO_ADS_PARSER_SIGNALHANDLER_H
