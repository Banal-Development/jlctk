// g++ -g t.cpp mmap-utils.cpp file-utils.cpp -o t.tsk
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <atomic>
#include <thread>

using namespace std;

#include "mmap-utils.h"

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
};

struct PlanetPosRing
{
  size_t ring_size;
  atomic<size_t> end_idx;
  PlanetPos planet_pos_v[0];
};

int main()
{
  size_t req_ring_size = 10000;
  size_t req_seg_size = sizeof(PlanetPosRing) + sizeof(PlanetPos) * req_ring_size;
  auto out_seg_descr = create_mmap_segment("out_seg.bin", req_seg_size);
  PlanetPosRing* out_seg = (PlanetPosRing*)out_seg_descr.start_addr;
  out_seg->ring_size = req_ring_size;
  out_seg->end_idx = 0;
  
  Planet p1(0, 1.0, 2 * M_PI / 3600.0, 1.0);
  int c = 0;
  while (true) {
    if (c++ % 100 == 0) {
      cout << p1.planet_num << " " << p1.t << " " << p1.x << " " << p1.y << endl;
    }    

    auto w_idx = out_seg->end_idx++;
    new (&out_seg->planet_pos_v[w_idx % req_ring_size]) PlanetPos{p1.planet_num, p1.t, p1.x, p1.y};
    
    p1.move();
    this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}
