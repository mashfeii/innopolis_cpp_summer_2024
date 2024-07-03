// NOTE: Since there is a variadic function, it was better to put all inside
// header file
#pragma once

#include <cctype>
#include <cstdint>
#include <exception>
#include <sstream>
#include <string>

// Custom exceptions
class StreamOutputException : public std::exception {
 public:
  const char* what() const noexcept {
    return "No viable operator<< for such argument";
  }
};
class WrongStringFormat : public std::exception {
 protected:
  std::string msg_;

 public:
  explicit WrongStringFormat(const char* message) : msg_(message) {}
  explicit WrongStringFormat(const std::string& message) : msg_(message) {}
  const char* what() const noexcept { return msg_.c_str(); }
};
class InvalidArguments : public std::exception {
 protected:
  std::string msg_;

 public:
  explicit InvalidArguments(const char* message) : msg_(message) {}
  explicit InvalidArguments(const std::string& message) : msg_(message) {}
  const char* what() const noexcept { return msg_.c_str(); }
};

// Recursion break point
inline std::string formatHelper(const std::string& string_format,
                                [[maybe_unused]] size_t count) {
  return string_format;
}

// Recursion step
template <typename Cur, typename... Args>
std::string formatHelper(const std::string& string_format,
                         size_t count,
                         const Cur& arg,
                         const Args&... args) {
  std::basic_ostringstream<char> result;
  const size_t argument_index = (count - 1) - sizeof...(args);
  size_t inserted_arguments = 0;

  // Go through string char by char
  for (size_t i = 0; i != string_format.length(); ++i) {
    // WARN: Meet closing bracket before opening one
    if (string_format[i] == '}') {
      throw WrongStringFormat("Unexpected '}' before '{' in string format");
      // Meet opening bracket -> validate inside content
    } else if (string_format[i] == '{') {
      ++i;
      std::ostringstream current_entity;
      while (string_format[i] != '}' && i != string_format.length()) {
        if (std::isdigit(string_format[i])) {
          current_entity << string_format[i++];
        } else {
          // WARN: note a digit inside curved brackets
          throw WrongStringFormat("Incorrect format inside curved brackets");
        }
      }
      ++inserted_arguments;

      if (string_format[i] != '}') {
        throw WrongStringFormat("No mathing closing bracket");
      }

      uint32_t entity_index_value;
      try {
        entity_index_value = std::stoull(current_entity.str());
      } catch (...) {
        throw WrongStringFormat("Incorrect format inside curved brackets");
      }

      // Case: index in brackets is the same as current argument index
      if (entity_index_value == argument_index) {
        try {
          result << arg;
        } catch (...) {
          throw StreamOutputException();
        }
        // Case: index in brackets differs from current argument index
      } else {
        // Case: index in brackets overflow
        if (entity_index_value > count - 1) {
          throw InvalidArguments("Not enough argument for substitution");
        }
        // Case: index in brackets is more that argument index
        result << '{' << entity_index_value << '}';
      }
    } else {
      if (string_format[i] != '\0') {
        result << string_format[i];
      }
    }
  }

  if (inserted_arguments == 0) {
    throw InvalidArguments("Too many arguments for format string");
  }

  return formatHelper(result.str(), count, args...);
}

// Starting point for recursive arguments substitution
template <typename... Args>
std::string format(const std::string& string_format, const Args&... args) {
  return formatHelper(string_format, sizeof...(args), args...);
}
