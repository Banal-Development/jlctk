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

end # module jlctk
