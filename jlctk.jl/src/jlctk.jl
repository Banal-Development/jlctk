module jlctk

# from https://github.com/JuliaLang/julia/blob/master/stdlib/Mmap/src/Mmap.jl

const PROT_READ     = Cint(1)
const PROT_WRITE    = Cint(2)
const MAP_SHARED    = Cint(1)
const MAP_PRIVATE   = Cint(2)

function open_mmap(fn::String)::Ptr{Cvoid}
  shared = true
  io = open(fn, "r")
  mmaplen = filesize(io)
  file_desc = RawFD(fd(io))
  prot = PROT_READ # or PROT_READ | PROT_WRITE
  flags = shared ? MAP_SHARED : MAP_PRIVATE
  offset_page = 0
  ptr = ccall(:jl_mmap, Ptr{Cvoid}, (Ptr{Cvoid}, Csize_t, Cint, Cint, RawFD, Int64),
              C_NULL, mmaplen, prot, flags, file_desc, offset_page)
  return ptr
end

struct RingDescriptor
   ring_size__::UInt64
   end_idx__::UInt64
   #planet_pos_v::Vector{PlanetPos}
end

struct VectorSegment{T}
  ring_o::Vector{RingDescriptor}
  v::Vector{T}
end

function get_size(vs::VectorSegment{T}) where {T}
 return vs.ring_o[1].ring_size__
end

function get_end_idx(vs::VectorSegment{T}) where {T}
 return vs.ring_o[1].end_idx__
end

function open_mmap_segment(_::Ptr{T}, fn::String)::VectorSegment{T} where {T}
 mmap_v_ptr = jlctk.open_mmap(fn)
 # from https://stackoverflow.com/a/40246607/1181482
 mmap_ptr = reinterpret(Ptr{RingDescriptor}, mmap_v_ptr)
 ring_o = unsafe_wrap(Vector{RingDescriptor}, mmap_ptr, 1)
 v_addr = reinterpret(Ptr{T}, mmap_v_ptr + sizeof(RingDescriptor))
 v = unsafe_wrap(Vector{T}, v_addr, ring_o[1].ring_size__)
 ret = VectorSegment{T}(ring_o, v)

 println("ring_size: ", ring_o[1].ring_size__)
 println("end_idx: ", ring_o[1].end_idx__) 
 println(mmap_v_ptr, " ", mmap_v_ptr + sizeof(RingDescriptor))
 println(mmap_ptr, " ", mmap_ptr + sizeof(RingDescriptor))
 return ret
end

end # module jlctk
