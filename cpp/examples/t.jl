using SharedArrays

struct PlanetPos
  planet_num::Int64
  t::Float64
  x::Float64
  y::Float64
end

struct PlanetPosRing
  ring_size::Int64
  end_idx::UInt64
  #planet_pos_v::Vector{PlanetPos}
end

function main()
 sa = SharedArray{PlanetPosRing}(abspath("./out_seg.bin"), (1,))
 ring_size = sa[1].ring_size
 println("ring_size: ", ring_size)
 println("end_idx: ", sa[1].end_idx) 
 planet_pos_v = SharedArray{PlanetPos}(abspath("./out_seg.bin"), (ring_size,), sizeof(PlanetPosRing))

 start_idx = 1
 while true
  last_idx = sa[1].end_idx-1
  if last_idx >= start_idx
     println(planet_pos_v[mod(start_idx, ring_size)], " --> ", planet_pos_v[mod(last_idx, ring_size)])
  end
  #for i in start_idx:last_idx
  #  println(planet_pos_v[i])
  #end
  start_idx = last_idx+1
  sleep(1.5)
 end
 
end

main()
