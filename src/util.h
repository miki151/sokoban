#pragma once

#include <vector>
#include <set>
#include <algorithm>
#include <memory>
#include <cassert>

using namespace std;

#define CHECK(a) assert(a)

class Range {
  public:
  Range(int start, int end);
  explicit Range(int end);

  bool isEmpty() const;
  Range reverse();
  Range shorten(int r);

  int getStart() const;
  int getEnd() const;

  class Iter {
    public:
    Iter(int ind, int min, int max, int increment);

    int operator* () const;
    bool operator != (const Iter& other) const;

    const Iter& operator++ ();

    private:
    int ind;
    int min;
    int max;
    int increment;
  };

  Iter begin();
  Iter end();

  private:
  int start = 0;
  int finish = 0;
  int increment = 1;
};

template <class T>
Range All(const T& container) {
  return Range(container.size());
}

class RandomGen {
  public:
  RandomGen() {}
  RandomGen(RandomGen&) = delete;
  void init(int seed);
  int get(int max);
  long long getLL();
  int get(int min, int max);
  int get(Range);
  int get(const vector<double>& weights);
  double getDouble();
  double getDouble(double a, double b);
  bool roll(int chance);
  bool rollD(double chance);
  template <typename T>
  T choose(const vector<T>& v, const vector<double>& p) {
    CHECK(v.size() == p.size());
    return v.at(get(p));
  }

  template <typename T>
  T choose(const vector<T>& v) {
    vector<double> pi(v.size(), 1);
    return choose(v, pi);
  }

  template <typename T>
  T choose(const set<T>& vi) {
    vector<T> v(vi.size());
    std::copy(vi.begin(), vi.end(), v.begin());
    return choose(v);
  }

  template <typename T>
  T choose(initializer_list<T> vi, initializer_list<double> pi) {
    return choose(vector<T>(vi), vector<double>(pi));
  }

  template <typename T, typename... Args>
  const T& choose(T const& first, T const& second, const Args&... rest) {
    return chooseImpl(first, 2, second, rest...);
  }

  template <typename T>
  T choose(vector<pair<T, double>> vi) {
    vector<T> v;
    vector<double> p;
    for (auto elem : vi) {
      v.push_back(elem.first);
      p.push_back(elem.second);
    }
    return choose(v, p);
  }

  template <typename T>
  vector<T> permutation(vector<T> v) {
    random_shuffle(v.begin(), v.end(), [this](int a) { return get(a);});
    return v;
  }

  template <typename T>
  vector<T> permutation(const set<T>& vi) {
    vector<T> v(vi.size());
    std::copy(vi.begin(), vi.end(), v.begin());
    return permutation(v);
  }

  template <typename T>
  vector<T> permutation(initializer_list<T> vi) {
    vector<T> v(vi);
    random_shuffle(v.begin(), v.end(), [this](int a) { return get(a);});
    return v;
  }

  vector<int> permutation(Range r) {
    vector<int> v;
    for (int i : r)
      v.push_back(i);
    random_shuffle(v.begin(), v.end(), [this](int a) { return get(a);});
    return v;
  }

  template <typename T>
  vector<T> chooseN(int n, vector<T> v) {
    CHECK(n <= v.size());
    random_shuffle(v.begin(), v.end(), [this](int a) { return get(a);});
    return getPrefix(v, n);
  }

  template <typename T>
  vector<T> chooseN(int n, initializer_list<T> v) {
    return chooseN(n, vector<T>(v));
  }

  private:
  default_random_engine generator;
  std::uniform_real_distribution<double> defaultDist;

  template <typename T>
  const T& chooseImpl(T const& cur, int total) {
    return cur;
  }

  template <typename T, typename... Args>
  const T& chooseImpl(T const& chosen, int total,  T const& next, const Args&... rest) {
    const T& nextChosen = roll(total) ? next : chosen;
    return chooseImpl(nextChosen, total + 1, rest...);
  }
};

class Rectangle;

class Vec2 {
  public:
  int x;
  int y;
  Vec2() : x(0), y(0) {}
  Vec2(int x, int y);
  bool inRectangle(int px, int py, int kx, int ky) const;
  bool operator == (const Vec2& v) const;
  bool operator != (const Vec2& v) const;
  Vec2 operator + (const Vec2& v) const;
  Vec2 operator * (int) const;
  Vec2 operator * (double) const;
  Vec2 operator / (int) const;
  Vec2& operator += (const Vec2& v);
  Vec2 operator - (const Vec2& v) const;
  Vec2& operator -= (const Vec2& v);
  Vec2 operator - () const;
  bool operator < (Vec2) const;
  Vec2 mult(const Vec2& v) const;
  Vec2 div(const Vec2& v) const;
  static int dotProduct(Vec2 a, Vec2 b);
  int length8() const;
  int length4() const;
  int dist8(Vec2) const;
  double distD(Vec2) const;
  double lengthD() const;
  Vec2 shorten() const;
  pair<Vec2, Vec2> approxL1() const;
  bool isCardinal4() const;
  bool isCardinal8() const;
  static Vec2 getCenterOfWeight(vector<Vec2>);

  vector<Vec2> box(int radius, bool shuffle = false);
  static vector<Vec2> directions8();
  vector<Vec2> neighbors8() const;
  static vector<Vec2> directions4();
  vector<Vec2> neighbors4() const;
  static vector<Vec2> directions8(RandomGen&);
  vector<Vec2> neighbors8(RandomGen&) const;
  static vector<Vec2> directions4(RandomGen&);
  vector<Vec2> neighbors4(RandomGen&) const;
  static vector<Vec2> corners();
  static vector<set<Vec2>> calculateLayers(set<Vec2>);
  bool inRectangle(const Rectangle& r) const;
};

class Rectangle {
  public:
  friend class Vec2;
  template<typename T>
  friend class Table;
  Rectangle(int width, int height);
  Rectangle(Vec2 dim);
  Rectangle(int px, int py, int kx, int ky);
  Rectangle(Vec2 p, Vec2 k);
  Rectangle(Range xRange, Range yRange);
  static Rectangle boundingBox(const vector<Vec2>& v);
  static Rectangle centered(Vec2 center, int radius);

  int left() const;
  int top() const;
  int right() const;
  int bottom() const;
  int width() const;
  int height() const;
  Vec2 getSize() const;
  Range getYRange() const;
  Range getXRange() const;
  int area() const;

  Vec2 topLeft() const;
  Vec2 bottomRight() const;
  Vec2 topRight() const;
  Vec2 bottomLeft() const;

  bool intersects(const Rectangle& other) const;
  bool contains(const Rectangle& other) const;
  Rectangle intersection(const Rectangle& other) const;

  Rectangle minusMargin(int margin) const;
  Rectangle translate(Vec2 v) const;

  Vec2 middle() const;

  vector<Vec2> getAllSquares() const;

  bool operator == (const Rectangle&) const;
  bool operator != (const Rectangle&) const;

  class Iter {
    public:
    Iter(int x, int y, int px, int py, int kx, int ky);

    Vec2 operator* () const;
    bool operator != (const Iter& other) const;

    const Iter& operator++ ();

    private:
    Vec2 pos;
    int px, py, kx, ky;
  };

  Iter begin() const;
  Iter end() const;

  private:
  int px = 0, py = 0, kx = 0, ky = 0, w = 0, h = 0;
};

template <class T>
class Table {
  public:
  Table(Table&& t) = default;

  Table(const Table& t) : Table(t.bounds) {
    for (int i : Range(bounds.w * bounds.h))
      mem[i] = t.mem[i];
  }

  Table(int x, int y, int w, int h) : bounds(x, y, x + w, y + h), mem(new T[w * h]) {
  }

  Table(const Rectangle& rect) : bounds(rect), mem(new T[rect.w * rect.h]){
  }

  Table(const Rectangle& rect, const T& value) : Table(rect) {
    for (int i : Range(bounds.w * bounds.h))
      mem[i] = value;
  }

  Table(int w, int h) : Table(0, 0, w, h) {
  }

  Table(int x, int y, int width, int height, const T& value) : Table(Rectangle(x, y, x + width, y + height), value) {
  }

  Table(int width, int height, const T& value) : Table(0, 0, width, height, value) {
  }

  const Rectangle& getBounds() const {
    return bounds;
  }

  Table& operator = (Table&& other) = default;
  Table& operator = (const Table& other) {
    bounds = other.bounds;
    mem.reset(new T[bounds.w * bounds.h]);
    for (int i : Range(bounds.w * bounds.h))
      mem[i] = other.mem[i];
    return *this;
  }

  int getWidth() const {
    return bounds.w;
  }
  int getHeight() const {
    return bounds.h;
  }
 
  T& operator[](const Vec2& vAbs) {
    CHECK(vAbs.inRectangle(bounds));
    return mem[(vAbs.x - bounds.px) * bounds.h + vAbs.y - bounds.py];
  }

  const T& operator[](const Vec2& vAbs) const {
    CHECK(vAbs.inRectangle(bounds));
    return mem[(vAbs.x - bounds.px) * bounds.h + vAbs.y - bounds.py];
  }

  private:
  Rectangle bounds;
  unique_ptr<T[]> mem;
};

template <typename T, typename V>
bool contains(const T& v, const V& elem) {
  return std::find(v.begin(), v.end(), elem) != v.end();
}

template<class T>
int findElement(const vector<T>& v, const T& element) {
  for (int i : All(v))
    if (v[i] == element)
      return i;
  return -1;
}
