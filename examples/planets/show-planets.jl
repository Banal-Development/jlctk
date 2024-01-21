# to run: julia --project=../../jlctk.jl ./show-planets.jl
#
using StaticArrays
using jlctk

struct RingDescriptor
  ring_size::UInt64
  end_idx::UInt64
  #planet_pos_v::Vector{PlanetPos}
end

struct PlanetPos
  planet_num::Int64
  t::Float64
  x::Float64
  y::Float64
  planet::SVector{16, UInt8}
end

function main()
 mmap_v_ptr = jlctk.open_mmap("./out_seg.bin")
 # from https://stackoverflow.com/a/40246607/1181482
 mmap_ptr = reinterpret(Ptr{RingDescriptor}, mmap_v_ptr)
 ring_o = unsafe_wrap(Vector{RingDescriptor}, mmap_ptr, 1)
 planet_pos_v_addr = reinterpret(Ptr{PlanetPos}, mmap_v_ptr + sizeof(RingDescriptor))
 planet_pos_v = unsafe_wrap(Vector{PlanetPos}, planet_pos_v_addr, ring_o[1].ring_size)

 println("ring_size: ", ring_o[1].ring_size)
 println("end_idx: ", ring_o[1].end_idx) 
 println(mmap_v_ptr, " ", mmap_v_ptr + sizeof(RingDescriptor))
 println(mmap_ptr, " ", mmap_ptr + sizeof(RingDescriptor))

 start_idx = 1
 while true
  last_idx = ring_o[1].end_idx-1
  if last_idx >= start_idx
     println(planet_pos_v[mod(start_idx, ring_o[1].ring_size)], " --> ", planet_pos_v[mod(last_idx, ring_o[1].ring_size)])
     println(String(planet_pos_v[mod(start_idx, ring_o[1].ring_size)].planet))
  end
  #for i in start_idx:last_idx
  #  println(planet_pos_v[i])
  #end
  start_idx = last_idx+1
  sleep(1.5)
 end
 
end

main()
