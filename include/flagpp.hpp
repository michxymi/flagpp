/**
 * @file flagpp.hpp
 * @brief A modern, header-only C++ feature flag library with zero dependencies
 * @version 0.1.0
 * 
 * FlagPlusPlus (flagpp) is a lightweight, thread-safe feature flag library
 * for C++ applications. It provides a simple API for defining, checking, and
 * updating feature flags at runtime.
 * 
 * Features:
 * - Header-only with no external dependencies
 * - Thread-safe operations
 * - Type-safe flag values
 * - Modern C++17 implementation
 * - Minimal runtime overhead
 */

#ifndef FLAGPP_HPP
#define FLAGPP_HPP

#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

namespace flagpp {

/**
 * @brief Represents the possible types a feature flag can have
 */
using FlagValue = std::variant<bool, int, double, std::string>;

/**
 * @brief Type-safe wrapper for flag values with conversion operators
 * 
 * Provides type-safe access to the underlying flag value with
 * convenient conversion operators for common types.
 */
class Value {
private:
  FlagValue value_;

public:
  /**
   * @brief Construct a new Value object
   * @tparam T The type of the value (must be compatible with FlagValue)
   * @param value The value to store
   */
  template <typename T, 
            typename = std::enable_if_t<
              std::is_constructible_v<FlagValue, T>>>
  explicit Value(T value) : value_(std::move(value)) {}

  /**
   * @brief Get the value as the specified type
   * @tparam T The type to retrieve
   * @return std::optional<T> The value if it matches the requested type, or nullopt
   */
  template <typename T>
  std::optional<T> get() const {
    if (std::holds_alternative<T>(value_)) {
      return std::get<T>(value_);
    }
    return std::nullopt;
  }

  /**
   * @brief Convert to boolean
   * @return bool The value as a boolean, or false if not a boolean
   */
  explicit operator bool() const {
    auto val = get<bool>();
    return val.has_value() ? *val : false;
  }

  /**
   * @brief Convert to integer
   * @return int The value as an integer, or 0 if not an integer
   */
  explicit operator int() const {
    auto val = get<int>();
    return val.has_value() ? *val : 0;
  }

  /**
   * @brief Convert to double
   * @return double The value as a double, or 0.0 if not a double
   */
  explicit operator double() const {
    auto val = get<double>();
    return val.has_value() ? *val : 0.0;
  }

  /**
   * @brief Convert to string
   * @return std::string The value as a string, or empty string if not a string
   */
  explicit operator std::string() const {
    auto val = get<std::string>();
    return val.has_value() ? *val : "";
  }
};

/**
 * @brief Represents a feature flag with thread-safe access
 * 
 * Stores the flag's name, value, and description with mutex
 * protection for thread-safe access.
 */
class Flag {
private:
  std::string name_;
  FlagValue value_;
  std::string description_;
  mutable std::shared_mutex mutex_; // Use shared_mutex for reader-writer lock

public:
  /**
   * @brief Construct a new Flag object
   * @param name The flag's name
   * @param default_value The flag's default value
   * @param description The flag's description (optional)
   */
  Flag(std::string name, FlagValue default_value, std::string description = "")
      : name_(std::move(name)), value_(std::move(default_value)),
        description_(std::move(description)) {}

  /**
   * @brief Get the flag's name
   * @return std::string_view The flag's name
   */
  std::string_view name() const { return name_; }

  /**
   * @brief Get the flag's description
   * @return std::string_view The flag's description
   */
  std::string_view description() const { return description_; }

  /**
   * @brief Get the flag's current value
   * @return Value The flag's value wrapped in a Value object
   */
  Value value() const { 
    std::shared_lock lock(mutex_); // Read lock
    return Value(value_); 
  }

  /**
   * @brief Update the flag's value
   * @tparam T The type of the new value (must be compatible with FlagValue)
   * @param new_value The new value to set
   */
  template <typename T>
  void update(T new_value) {
    std::unique_lock lock(mutex_); // Write lock
    value_ = FlagValue(std::move(new_value));
  }
};

/**
 * @brief Singleton registry for all feature flags
 * 
 * Provides thread-safe storage and access to all feature flags
 * in the application.
 */
class FlagRegistry {
private:
  mutable std::shared_mutex mutex_;
  std::unordered_map<std::string, std::shared_ptr<Flag>> flags_;

  // Private constructor for singleton
  FlagRegistry() = default;

public:
  // Delete copy/move constructors and assignment operators
  FlagRegistry(const FlagRegistry&) = delete;
  FlagRegistry& operator=(const FlagRegistry&) = delete;
  FlagRegistry(FlagRegistry&&) = delete;
  FlagRegistry& operator=(FlagRegistry&&) = delete;

  /**
   * @brief Get the singleton instance
   * @return FlagRegistry& Reference to the singleton instance
   */
  static FlagRegistry& instance() {
    static FlagRegistry registry;
    return registry;
  }

  /**
   * @brief Define a new flag or return existing one
   * @tparam T The type of the flag's default value
   * @param name The flag's name
   * @param default_value The flag's default value
   * @param description The flag's description (optional)
   * @return std::shared_ptr<Flag> Pointer to the flag
   */
  template <typename T>
  std::shared_ptr<Flag> define(const std::string& name, T default_value,
                              const std::string& description = "") {
    std::unique_lock lock(mutex_);
    
    auto it = flags_.find(name);
    if (it != flags_.end()) {
      return it->second;
    }
    
    auto flag = std::make_shared<Flag>(name, FlagValue(std::move(default_value)), 
                                      description);
    flags_[name] = flag;
    return flag;
  }

  /**
   * @brief Get a flag by name
   * @param name The flag's name
   * @return std::shared_ptr<Flag> Pointer to the flag, or nullptr if not found
   */
  std::shared_ptr<Flag> get(const std::string& name) const {
    std::shared_lock lock(mutex_);
    
    auto it = flags_.find(name);
    if (it != flags_.end()) {
      return it->second;
    }
    
    return nullptr;
  }

  /**
   * @brief Check if a flag exists
   * @param name The flag's name
   * @return bool True if the flag exists, false otherwise
   */
  bool exists(const std::string& name) const {
    std::shared_lock lock(mutex_);
    return flags_.find(name) != flags_.end();
  }

  /**
   * @brief Update a flag's value
   * @tparam T The type of the new value
   * @param name The flag's name
   * @param value The new value to set
   * @return bool True if the flag was updated, false if not found
   */
  template <typename T>
  bool update(const std::string& name, T value) {
    auto flag = get(name);
    if (!flag) {
      return false;
    }
    
    flag->update(std::move(value));
    return true;
  }

  /**
   * @brief Get all registered flags
   * @return std::vector<std::shared_ptr<Flag>> Vector of all flags
   */
  std::vector<std::shared_ptr<Flag>> get_all() const {
    std::shared_lock lock(mutex_);
    std::vector<std::shared_ptr<Flag>> result;
    result.reserve(flags_.size());
    
    for (const auto& [_, flag] : flags_) {
      result.push_back(flag);
    }
    
    return result;
  }
};

/**
 * @brief Convenience functions for working with flags
 * 
 * Provides a simple API for defining, checking, and updating
 * feature flags without directly interacting with the registry.
 */
namespace flags {

/**
 * @brief Define a new flag or get existing one
 * @tparam T The type of the flag's default value
 * @param name The flag's name
 * @param default_value The flag's default value
 * @param description The flag's description (optional)
 * @return std::shared_ptr<Flag> Pointer to the flag
 */
template <typename T>
std::shared_ptr<Flag> define(const std::string& name, T default_value,
                            const std::string& description = "") {
  return FlagRegistry::instance().define(name, std::move(default_value), 
                                        description);
}

/**
 * @brief Get a flag by name
 * @param name The flag's name
 * @return std::shared_ptr<Flag> Pointer to the flag, or nullptr if not found
 */
inline std::shared_ptr<Flag> get(const std::string& name) {
  return FlagRegistry::instance().get(name);
}

/**
 * @brief Check if a flag exists
 * @param name The flag's name
 * @return bool True if the flag exists, false otherwise
 */
inline bool exists(const std::string& name) {
  return FlagRegistry::instance().exists(name);
}

/**
 * @brief Check if a boolean flag is enabled
 * @param name The flag's name
 * @return bool True if the flag exists and is enabled, false otherwise
 */
inline bool is_enabled(const std::string& name) {
  auto flag = get(name);
  return flag ? static_cast<bool>(flag->value()) : false;
}

/**
 * @brief Get a flag's value with type checking
 * @tparam T The expected type of the flag's value
 * @param name The flag's name
 * @return std::optional<T> The flag's value if it exists and matches the type, or nullopt
 */
template <typename T>
std::optional<T> get_value(const std::string& name) {
  auto flag = get(name);
  if (!flag) {
    return std::nullopt;
  }
  return flag->value().get<T>();
}

/**
 * @brief Update a flag's value
 * @tparam T The type of the new value
 * @param name The flag's name
 * @param value The new value to set
 * @return bool True if the flag was updated, false if not found
 */
template <typename T>
bool update(const std::string& name, T value) {
  return FlagRegistry::instance().update(name, std::move(value));
}

/**
 * @brief Get all registered flags
 * @return std::vector<std::shared_ptr<Flag>> Vector of all flags
 */
inline std::vector<std::shared_ptr<Flag>> get_all() {
  return FlagRegistry::instance().get_all();
}

} // namespace flags

} // namespace flagpp

#endif // FLAGPP_HPP
