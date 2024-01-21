// g++ -g t.cpp mmap-utils.cpp file-utils.cpp -o t.tsk
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <atomic>
#include <thread>

using namespace std;

#include <mmap-utils.h>
#include <segment-utils.h>

struct Planet
{
  const int planet_num;
  const double R;
  const double omega, dt; // angular velocity and time delta

  double t = 0.0, fi = 0.0;
  double x = NAN, y = NAN;

  Planet(int planet_num, double R, double omega, double dt) :
    planet_num(planet_num), R(R), omega(omega), dt(dt)
  {
    x = R * cos(fi); y = R * sin(fi);
  }
  
  void move() {
    t  += dt;
    fi += omega * dt;
    x = R * cos(fi); y = R * sin(fi);
  }
};

struct PlanetPos
{
  int planet_num;
  double t, x, y;
  array<unsigned char, 16> planet;

  PlanetPos(int planet_num, double t, double x, double y, const string& planet)
  {
    this->planet_num = planet_num;
    this->t = t;
    this->x = x;
    this->y = y;
    if (planet.size() >= this->planet.size()) {
      throw runtime_error("too long string");
    }
    copy(planet.begin(), planet.end(), this->planet.begin());
  }
};

int main()
{
  size_t req_ring_size = 10000;
  auto out_seg = VectorSegment<PlanetPos>::create_mmap_segment("out_seg.bin", req_ring_size);
  
  Planet p1(0, 1.0, 2 * M_PI / 3600.0, 1.0);
  int c = 0;
  while (true) {
    if (c++ % 100 == 0) {
      cout << p1.planet_num << " " << p1.t << " " << p1.x << " " << p1.y << endl;
    }    

    auto w_idx = out_seg->end_idx++;
    auto aa = to_array("hh");
    new (&out_seg->v[w_idx % req_ring_size]) PlanetPos(p1.planet_num, p1.t, p1.x, p1.y, "myplanet");
    
    p1.move();
    this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}
