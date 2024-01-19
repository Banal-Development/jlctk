using SharedArrays

struct PlanetPos
  planet_num::Int64
  t::Float64
  x::Float64
  y::Float64
end

function main()
 sa = SharedArray{PlanetPos}(abspath("./out_seg.bin"), (1000,))
 for i in 1:10
 println(sa[i])
 end
 
end

main()
