#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <string.h>
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
  const char* out_fn = "./cout.txt";
  const char* err_fn = "./cerr.txt";

  bool pass_thru = false;
  if (getenv("__JLCTK_PASS_THRU")) {
    pass_thru = true;
  }

  if (!pass_thru) {
    cout << "argc: " << argc << endl;
    for (int i = 0; i < argc; i++) {
      cout << "argv " << i << " " << argv[i] << endl;
    }
    cout << "screen stdout, pid: " << getpid() << endl;
    cout << "screen stderr, pid: " << getpid() << endl;

    close(0);
    
    int new_stdout = open(out_fn, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    dup2(new_stdout, 1);
    close(new_stdout);
    
    int new_stderr = open(err_fn, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    dup2(new_stderr, 2);
    close(new_stderr);
    
    pid_t parent_pid = getpid();
    pid_t pid = fork();
    if (pid == -1) {
      cerr << "fork failed, errno " << errno << endl;
      exit(2);
    } else if (pid > 0) { // parent
      int status = 0;
      waitpid(pid, &status, 0);
      cerr << "parent exit, status  " << status << endl;
      if (WIFSIGNALED(status)) {
	int child_sig = WTERMSIG(status);
	cerr << "child process killed by signal " << child_sig << " " << strsignal(child_sig) << endl;
      }
      exit(0);
    } else { // child
      //cout << "calling execve" << endl;
      char env_s[256];
      sprintf(env_s, "__JLCTK_PASS_THRU=1");
      char *envp[] = {env_s, NULL};
      auto execve_ret = execvpe(argv[0], argv, envp);
      cerr << "execve_ret: " << execve_ret << " " << errno << endl;
    }
  }
  
  cout << "Hello, world!" << endl;
  char* p = 0;
  //*p = 1;
  
  cout << 1 / 0 << endl;
}
