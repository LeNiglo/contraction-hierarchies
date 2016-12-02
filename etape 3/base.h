#ifndef BASE_H_
#define BASE_H_

#include <cmath>
#include <ctime>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using std::map;
using std::pair;
using std::string;
using std::vector;

class OstreamVoidifier {
 public:
  // This has to be an operator with a precedence lower than << but
  // higher than ?:
  void operator&(::std::ostream&) {}
};

class Logger {
 public:
  Logger(const char* file, int line) {
    stream() << file << ":" << line << ": ";
  }
  ~Logger() {
    stream() << std::endl;
  }
  std::ostream& stream() { return std::cerr; }
};

class FatalLogger : public Logger {
 public:
  FatalLogger(const char* file, int line, const char* message)
      : Logger(file, line) {
    stream() << "Assertion failed: " << message << " ";
  }
  ~FatalLogger() {
    stream() << std::endl;
    exit(1);
  }
  std::ostream& stream() { return std::cerr; }
};

#define CHECK(x) (x) ? (void)0 \
    : OstreamVoidifier() & FatalLogger(__FILE__, __LINE__, #x).stream()

#define CHECK_BINARY(test, x, y) \
    CHECK((x) test (y)) << std::endl << "Left value: " << (x) << std::endl\
                        << "Right value: " << (y) << std::endl

#define CHECK_EQ(x, y) CHECK_BINARY(==, x, y)
#define CHECK_EQ(x, y) CHECK_BINARY(==, x, y)
#define CHECK_NE(x, y) CHECK_BINARY(!=, x, y)
#define CHECK_LT(x, y) CHECK_BINARY(<, x, y)
#define CHECK_GT(x, y) CHECK_BINARY(>, x, y)
#define CHECK_LE(x, y) CHECK_BINARY(<=, x, y)
#define CHECK_GE(x, y) CHECK_BINARY(>=, x, y)

#define CHECK_NEAR(x, y, eps) \
    CHECK(std::abs(double(x) - double(y)) <= double(eps))\
        << "Left value: " << (x) << std::endl\
        << "Right value: " << (y) << std::endl\
        << "Diff: " << std::endl

#define LOG Logger(__FILE__, __LINE__).stream()

#ifdef DEBUG
#define DLOG LOG
#else
#define DLOG true ? (void)0 : OstreamVoidifier() & std::cerr
#endif

inline bool IsValidLatitude(double lat) { return lat >= -90 && lat <= 90; }
inline bool IsValidLongitude(double lng) { return lng >= -180 && lng < 180; }

template<class T>
string PrintHistogram(const T& v) {
  typedef typename T::value_type Value;
  map<Value, int> count;
  for (typename T::const_iterator it = v.begin(); it != v.end(); ++it) ++count[*it];
  vector<pair<int, Value> > pairs;
  for (typename map<Value, int>::const_iterator it = count.begin(); it != count.end(); ++it) {
    pairs.push_back(std::make_pair(it->second, it->first));
  }
  std::sort(pairs.begin(), pairs.end());
  std::reverse(pairs.begin(), pairs.end());
  std::ostringstream os;
  for (const auto& p : pairs) {
    os << "  #" << p.first << " of '" << p.second << "'\n";
  }
  return os.str();
}

template<class T>
string PrintList(const T& v) {
  std::ostringstream os;
  os << "[ ";
  for (typename T::const_iterator it = v.begin(); it != v.end(); ++it) os << *it << " ";
  os << "]";
  return os.str();
}

template<class T>
vector<T> Sorted(const vector<T>& v) {
  vector<T> s = v;
  std::sort(s.begin(), s.end());
  return s;
}

template<class T>
void SortAndRemoveDuplicates(vector<T>* v) {
  std::sort(v->begin(), v->end());
  v->erase(std::unique(v->begin(), v->end()), v->end());
}

#define kint32max 0x7ffffff

#endif  // BASE_H_
