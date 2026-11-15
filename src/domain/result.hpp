#pragma once

#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

namespace ccsim {

template <typename T>
class Result {
public:
    static Result success(T value) {
        return Result(std::move(value));
    }

    static Result failure(std::string error) {
        return Result(std::move(error));
    }

    bool has_value() const noexcept {
        return std::holds_alternative<T>(value_);
    }

    const T& value() const {
        if (!has_value()) {
            throw std::logic_error("Cannot read the value of a failed result");
        }
        return std::get<T>(value_);
    }

    T& value() {
        if (!has_value()) {
            throw std::logic_error("Cannot read the value of a failed result");
        }
        return std::get<T>(value_);
    }

    const std::string& error() const {
        if (has_value()) {
            throw std::logic_error("Cannot read the error of a successful result");
        }
        return std::get<std::string>(value_);
    }

private:
    explicit Result(T value) : value_(std::move(value)) {}
    explicit Result(std::string error) : value_(std::move(error)) {}

    std::variant<T, std::string> value_;
};

}  // namespace ccsim
