# to run: julia --project=../../jlctk.jl ./show-planets.jl
#
using StaticArrays
using jlctk

struct PlanetPos
 planet_num::Int64
 t::Float64
 x::Float64
 y::Float64
 planet::SVector{16, UInt8}
end

function main()
 planet_seg = jlctk.open_mmap_segment(Ptr{PlanetPos}(0), "out_seg.bin")
 println(typeof(planet_seg))

 start_idx = 1
 while true
  size = jlctk.get_size(planet_seg)
  last_idx = jlctk.get_end_idx(planet_seg)-1
  if last_idx >= start_idx
   println(planet_seg.v[mod(start_idx, size)], " --> ", planet_seg.v[mod(last_idx, size)])
   println(String(planet_seg.v[mod(start_idx, size)].planet))
  end
  #for i in start_idx:last_idx
  #  println(planet_pos_v[i])
  #end
  start_idx = last_idx+1
  sleep(1.5)
 end
 
end

main()
