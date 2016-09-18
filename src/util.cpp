#include "util.h"


void RandomGen::init(int seed) {
  generator.seed(seed);
}

int RandomGen::get(int max) {
  return get(0, max);
}

long long RandomGen::getLL() {
  return uniform_int_distribution<long long>(-(1LL << 62), 1LL << 62)(generator);
}

int RandomGen::get(Range r) {
  return get(r.getStart(), r.getEnd());
}

int RandomGen::get(int min, int max) {
  CHECK(max > min);
  return uniform_int_distribution<int>(min, max - 1)(generator);
}

int RandomGen::get(const vector<double>& weights) {
  double sum = 0;
  for (double elem : weights)
    sum += elem;
  CHECK(sum > 0);
  double r = getDouble(0, sum);
  sum = 0;
  for (int i : All(weights)) {
    sum += weights[i];
    if (sum >= r)
      return i;
  }
  return weights.size() - 1;
}

bool RandomGen::roll(int chance) {
  return get(chance) == 0;
}

bool RandomGen::rollD(double chance) {
  return getDouble(0, chance) <= 1;
}

double RandomGen::getDouble() {
  return defaultDist(generator);
}

double RandomGen::getDouble(double a, double b) {
  return uniform_real_distribution<double>(a, b)(generator);
}


Vec2::Vec2(int _x, int _y) : x(_x), y(_y) {
}

Vec2 Vec2::mult(const Vec2& v) const {
  return Vec2(x * v.x, y * v.y);
}

Vec2 Vec2::div(const Vec2& v) const {
  return Vec2(x / v.x, y / v.y);
}

int Vec2::dotProduct(Vec2 a, Vec2 b) {
  return a.x * b.x + a.y * b.y;
}

vector<Vec2> Vec2::directions8() {
  return Vec2(0, 0).neighbors8();
}

vector<Vec2> Vec2::neighbors8() const {
  return {Vec2(x, y + 1), Vec2(x + 1, y), Vec2(x, y - 1), Vec2(x - 1, y), Vec2(x + 1, y + 1), Vec2(x + 1, y - 1),
      Vec2(x - 1, y - 1), Vec2(x - 1, y + 1)};
}

vector<Vec2> Vec2::directions4() {
  return Vec2(0, 0).neighbors4();
}

vector<Vec2> Vec2::neighbors4() const {
  return { Vec2(x, y + 1), Vec2(x + 1, y), Vec2(x, y - 1), Vec2(x - 1, y)};
}

vector<Vec2> Vec2::directions8(RandomGen& random) {
  return random.permutation(directions8());
}

vector<Vec2> Vec2::neighbors8(RandomGen& random) const {
  return random.permutation(neighbors8());
}

vector<Vec2> Vec2::directions4(RandomGen& random) {
  return random.permutation(directions4());
}

vector<Vec2> Vec2::neighbors4(RandomGen& random) const {
  return random.permutation(neighbors4());
}

bool Vec2::isCardinal4() const {
  return abs(x) + abs(y) == 1;
}

bool Vec2::isCardinal8() const {
  return max(abs(x), abs(y)) == 1;
}

vector<Vec2> Vec2::corners() {
  return { Vec2(1, 1), Vec2(1, -1), Vec2(-1, -1), Vec2(-1, 1)};
}

vector<set<Vec2>> Vec2::calculateLayers(set<Vec2> elems) {
  vector<set<Vec2>> ret;
  while (1) {
    ret.emplace_back();
    set<Vec2> curElems(elems);
    for (Vec2 v : curElems)
      for (Vec2 v2 : v.neighbors4())
        if (!curElems.count(v2)) {
          ret.back().insert(v);
          elems.erase(v);
          break;
        }
    if (elems.empty())
      break;
  }
  return ret;
}

Rectangle Rectangle::boundingBox(const vector<Vec2>& verts) {
  CHECK(!verts.empty());
  int infinity = 1000000;
  int minX = infinity, maxX = -infinity, minY = infinity, maxY = -infinity;
  for (Vec2 v : verts) {
    minX = min(minX, v.x);
    maxX = max(maxX, v.x);
    minY = min(minY, v.y);
    maxY = max(maxY, v.y);
  }
  return Rectangle(minX, minY, maxX + 1, maxY + 1);
}

Rectangle Rectangle::centered(Vec2 center, int radius) {
  return Rectangle(center - Vec2(radius, radius), center + Vec2(radius + 1, radius + 1));
}

vector<Vec2> Rectangle::getAllSquares() const {
  vector<Vec2> ret;
  for (Vec2 v : (*this))
    ret.push_back(v);
  return ret;
}

bool Rectangle::operator == (const Rectangle& r) const {
  return px == r.px && py == r.py && kx == r.kx && ky == r.ky;
}

bool Rectangle::operator != (const Rectangle& r) const {
  return !(*this == r);
}

bool Vec2::inRectangle(int px, int py, int kx, int ky) const {
  return x >= px && x < kx && y >= py && y < ky;
}

bool Vec2::inRectangle(const Rectangle& r) const {
  return x >= r.px && x < r.kx && y >= r.py && y < r.ky;
}

bool Vec2::operator== (const Vec2& v) const {
  return v.x == x && v.y == y;
}

bool Vec2::operator!= (const Vec2& v) const {
  return v.x != x || v.y != y;
}

Vec2& Vec2::operator +=(const Vec2& v) {
  x += v.x;
  y += v.y;
  return *this;
}

Vec2 Vec2::operator + (const Vec2& v) const {
  return Vec2(x + v.x, y + v.y);
}

Vec2 Vec2::operator * (int a) const {
  return Vec2(x * a, y * a);
}

Vec2 Vec2::operator * (double a) const {
  return Vec2(x * a, y * a);
}

Vec2 Vec2::operator / (int a) const {
  return Vec2(x / a, y / a);
}

Vec2& Vec2::operator -=(const Vec2& v) {
  x -= v.x;
  y -= v.y;
  return *this;
}

Vec2 Vec2::operator - (const Vec2& v) const {
  return Vec2(x - v.x, y - v.y);
}

Vec2 Vec2::operator - () const {
  return Vec2(-x, -y);
}

bool Vec2::operator < (Vec2 v) const {
  return x < v.x || (x == v.x && y < v.y);
}

int Vec2::length8() const {
  return max(abs(x), abs(y));
}

int Vec2::dist8(Vec2 v) const {
  return (v - *this).length8();
}

double Vec2::distD(Vec2 v) const {
  return (v - *this).lengthD();
}

int Vec2::length4() const {
  return abs(x) + abs(y);
}

double Vec2::lengthD() const {
  return sqrt(x * x + y * y);
}

Vec2 Vec2::shorten() const {
  CHECK(x == 0 || y == 0 || abs(x) == abs(y));
  int d = length8();
  return Vec2(x / d, y / d);
}

static int sgn(int a) {
  if (a == 0)
    return 0;
  if (a < 0)
    return -1;
  else
    return 1;
}

static int sgn(int a, int b) {
  if (abs(a) >= abs(b))
    return sgn(a);
  else
    return 0;
}

pair<Vec2, Vec2> Vec2::approxL1() const {
  return make_pair(Vec2(sgn(x, x), sgn(y,y)), Vec2(sgn(x, y), sgn(y, x)));
}

Vec2 Vec2::getCenterOfWeight(vector<Vec2> vs) {
  Vec2 ret;
  for (Vec2 v : vs)
    ret += v;
  return ret / vs.size();
}

Rectangle::Rectangle(int _w, int _h) : px(0), py(0), kx(_w), ky(_h), w(_w), h(_h) {
  CHECK(w > 0 && h > 0);
}

Rectangle::Rectangle(Vec2 d) : px(0), py(0), kx(d.x), ky(d.y), w(d.x), h(d.y) {
  CHECK(d.x > 0 && d.y > 0);
}

Rectangle::Rectangle(int px1, int py1, int kx1, int ky1) : px(px1), py(py1), kx(kx1), ky(ky1), w(kx1 - px1),
    h(ky1 - py1) {
  CHECK(kx > px && ky > py);
}

Rectangle::Rectangle(Vec2 p, Vec2 k) : px(p.x), py(p.y), kx(k.x), ky(k.y), w(k.x - p.x), h(k.y - p.y) {
  CHECK(k.x > p.x);
  CHECK(k.y > p.y);
}

Rectangle::Rectangle(Range xRange, Range yRange)
    : Rectangle(xRange.getStart(), yRange.getStart(), xRange.getEnd(), yRange.getEnd()) {
}

Rectangle::Iter::Iter(int x1, int y1, int px1, int py1, int kx1, int ky1) : pos(x1, y1), px(px1), py(py1), kx(kx1), ky(ky1) {}

Vec2 Rectangle::middle() const {
  return Vec2((px + kx) / 2, (py + ky) / 2);
}

int Rectangle::left() const {
  return px;
}

int Rectangle::top() const {
  return py;
}

Range Rectangle::getXRange() const {
  return Range(px, kx);
}

Range Rectangle::getYRange() const {
  return Range(py, ky);
}

int Rectangle::right() const {
  return kx;
}

int Rectangle::bottom() const {
  return ky;
}

int Rectangle::width() const {
  return w;
}

int Rectangle::height() const {
  return h;
}

int Rectangle::area() const {
  return w * h;
}

Vec2 Rectangle::getSize() const {
  return Vec2(w, h);
}

Vec2 Rectangle::topLeft() const {
  return Vec2(px, py);
}

Vec2 Rectangle::bottomRight() const {
  return Vec2(kx, ky);
}

Vec2 Rectangle::topRight() const {
  return Vec2(kx, py);
}

Vec2 Rectangle::bottomLeft() const {
  return Vec2(px, ky);
}

bool Rectangle::intersects(const Rectangle& other) const {
  return max(px, other.px) < min(kx, other.kx) && max(py, other.py) < min(ky, other.ky);
}

bool Rectangle::contains(const Rectangle& other) const {
  return px <= other.px && py <= other.py && kx >= other.kx && ky >= other.ky;
}

Rectangle Rectangle::intersection(const Rectangle& other) const {
  return Rectangle(max(px, other.px), max(py, other.py), min(kx, other.kx), min(ky, other.ky));
}

Rectangle Rectangle::translate(Vec2 v) const {
  return Rectangle(topLeft() + v, bottomRight() + v);
}

Rectangle Rectangle::minusMargin(int margin) const {
  CHECK(px + margin < kx - margin && py + margin < ky - margin);
  return Rectangle(px + margin, py + margin, kx - margin, ky - margin);
}

Vec2 Rectangle::Iter::operator* () const {
  return pos;
}
bool Rectangle::Iter::operator != (const Iter& other) const {
  return pos != other.pos;
}

const Rectangle::Iter& Rectangle::Iter::operator++ () {
  ++pos.y;
  if (pos.y >= ky) {
    pos.y = py;
    ++pos.x;
  }
  return *this;
}

Rectangle::Iter Rectangle::begin() const {
  return Iter(px, py, px, py, kx, ky);
}

Rectangle::Iter Rectangle::end() const {
  return Iter(kx, py, px, py, kx, ky);
}

Range::Range(int a, int b) : start(a), finish(b) {
}
Range::Range(int a) : Range(0, a) {}

Range Range::reverse() {
  Range r(finish - 1, start - 1);
  r.increment = -1;
  return r;
}

bool Range::isEmpty() const {
  return (increment == 1 && start >= finish) || (increment == -1 && start <= finish);
}

Range Range::shorten(int r) {
  if (start < finish) {
    if (finish - start >= 2 * r)
      return Range(start + r, finish - r);
    else
      return Range(0, 0);
  } else {
    if (start - finish >= 2 * r)
      return Range(start - r, finish + r);
    else
      return Range(0, 0);
  }
}

int Range::getStart() const {
  return start;
}

int Range::getEnd() const {
  return finish;
}

Range::Iter Range::begin() {
  if ((increment > 0 && start < finish) || (increment < 0 && start > finish))
    return Iter(start, start, finish, increment);
  else
    return end();
}

Range::Iter Range::end() {
  return Iter(finish, start, finish, increment);
}

Range::Iter::Iter(int i, int a, int b, int inc) : ind(i), min(a), max(b), increment(inc) {}

int Range::Iter::operator* () const {
  return ind;
}

bool Range::Iter::operator != (const Iter& other) const {
  return other.ind != ind;
}

const Range::Iter& Range::Iter::operator++ () {
  ind += increment;
  //CHECK(ind <= max && ind >= min) << ind << " " << min << " " << max;
  return *this;
}
