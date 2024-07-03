#pragma once

#include <exception>
#include <sstream>
#include <string>

class StreamOutputException : public std::exception {
 public:
  const char* what() const noexcept {
    return "No viable operator << for such argument";
  }
};

class WrongStringFormat : public std::exception {
 public:
  const char* what() const noexcept {
    return "Unexpected '}' before '{' in string format";
  }
};

class ArgumentsOverflow : public std::exception {
 protected:
  std::string msg_;

 public:
  explicit ArgumentsOverflow(const char* message) : msg_(message) {}
  explicit ArgumentsOverflow(const std::string& message) : msg_(message) {}
  const char* what() const noexcept { return msg_.c_str(); }
};

std::string formatEndPoint(const std::string& string_format,
                           const size_t& count) {
  // void(count);
  return string_format;
}

template <typename Cur, typename... Args>
std::string recursiveFormat(const std::string& string_format,
                            const size_t& count,
                            const Cur& arg,
                            const Args&... args) {
  std::basic_ostringstream<char> result;
  const size_t argument_index = (count - 1) - sizeof...(args);
  size_t inserted_arguments = 0;

  for (size_t i = 0; i != string_format.length(); ++i) {
    if (string_format[i] == '}') {
      throw WrongStringFormat();
    } else if (string_format[i] == '{') {
    } else {
      if (string_format[i] != '\0') {
        result << string_format[i];
      }
    }
  }

  if (!inserted_arguments) {
    throw ArgumentsOverflow("Too many arguments for format string");
  }

  return formatHelper(result.str(), count, args...);
}

template <typename... Args>
std::string format(const std::string& string_format, const Args&... args) {
  recursiveFormat(string_format, sizeof...(args), args...);
}
