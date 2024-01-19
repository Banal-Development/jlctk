using Debugger

c_types_d = Dict{Symbol,String}(:Int32=>"int",:Float64=>"double",:UInt8=>"char")

function generate_c_struct(struct_name, struct_members)
  println("struct $struct_name {")
  for (struct_member_name, struct_member_type) in struct_members
   c_typename = get(c_types_d, struct_member_type, undef)
   if c_typename == undef
     println("error")
   else
     println("  $c_typename $struct_member_name;")
   end
  end
  println("};")
end

function walk_struct_def(sn::Symbol, s_body::Expr)
  #println("struct $sn with body ", typeof(s_body))
  s_members = []
  for arg in s_body.args
    if isa(arg, LineNumberNode)
      continue
    end
    #println(arg, " ", typeof(arg), " ", arg.head, "--", typeof(arg.head), " ", arg.args[1], " ", arg.args[2])
    push!(s_members, (arg.args[1], arg.args[2]))
  end

  generate_c_struct(sn, s_members)
end

function walk_for_structs(ln::LineNumberNode)
end

function walk_for_structs(e::Expr)
  if e.head == :block
    for arg in e.args
      walk_for_structs(arg)
    end
  elseif e.head == :struct
    #println("found struct ", e.args[2])
    walk_struct_def(e.args[2], e.args[3])
  else
    println("// unhandled: ", e, " ", typeof(e))
    #eval(e)
    #@bp
  end
end

function parseall(str)
    pos = start(str)
    exs = []
    while !done(str, pos)
      ex, pos = parse(str, pos) # returns next starting point as well as expr
      ex.head == :toplevel ? append!(exs, ex.args) : push!(exs, ex) #see comments for info
    end

    if length(exs) == 0
      throw(ParseError("end of input"))
    elseif length(exs) == 1
      return exs[1]
    else
      return Expr(:block, exs...) # convert the array of expressions
      # back to a single expression
    end
end

function main()
 println("// ", ARGS)

 jlh_code = read(ARGS[1], String)
 expr = Meta.parse("begin $jlh_code end")
 #expr = Meta.parseall(jlh_code) # see also https://stackoverflow.com/a/46366560/1181482

 walk_for_structs(expr)

 @bp

end

#@run main()
main()