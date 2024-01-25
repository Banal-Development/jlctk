function to_cmd(cmd_s::String)::Cmd
 pp = Base.shell_parse(cmd_s)[1]
 pp_cmd = map((x)->String(x.args[1]), pp.args)
 #println(pp_cmd)
 c = Cmd(pp_cmd)
 return c
end

function wait_task(cmd_s::String)
 println("wait_task started")
 cmd = to_cmd(cmd_s)
 rr = run(pipeline(cmd, stdout="/tmp/out.txt", stderr="/tmp/err.txt"), wait = false)
 println("pid: ", getpid(rr))
 wait(rr) # here we block till cmd got killed by signal
 println("rr done ", rr.exitcode)
end

function main()
 println("start")
 cmd_s = "sleep 5000"
 tt = Task(()->wait_task(cmd_s))
 schedule(tt)
end

main()
