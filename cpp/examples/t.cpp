#include <unistd.h>
#include <iostream>
#include <cmath>
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

int main()
{
  auto out_seg_descr = create_mmap_segment("out_seg.bin", 10 * 1024 * 1024);
  PlanetPos* out_seg = (PlanetPos*)out_seg_descr.start_addr;
  
  Planet p1(0, 1.0, 2 * M_PI / 3600.0, 1.0);
  unsigned out_i = 0;
  while (true) {
    cout << p1.planet_num << " " << p1.t << " " << p1.x << " " << p1.y << endl;
    new (&out_seg[out_i++]) PlanetPos{p1.planet_num, p1.t, p1.x, p1.y};
    p1.move();
    sleep(1.0);
  }
}
