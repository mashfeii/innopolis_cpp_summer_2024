#include <algorithm>
#include <cctype>
#include <string>

inline std::string& ltrim(std::string& s) {
  s.erase(s.begin(),
          std::find_if_not(s.begin(), s.end(), [](unsigned char c) { return std::isspace(c); }));
  return s;
}

inline std::string& rtrim(std::string& s) {
  s.erase(std::find_if_not(s.rbegin(), s.rend(), [](unsigned char c) { return std::isspace(c); })
              .base(),
          s.end());
  return s;
};

inline void trim(std::string& s) {
  ltrim(s);
  rtrim(s);
}
