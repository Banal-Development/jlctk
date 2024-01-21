#include "file-utils.h"
#include <cstddef>
#include <string>
#include <sstream>
#include <string.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <libgen.h>

using namespace std;

static void _mkdir(const char *dir)
{
  char tmp[256];
  char *p = nullptr;
  size_t len;

  //snprintf(tmp, sizeof(tmp),"%s",dir);
  memcpy(tmp, dir, strlen(dir));
  len = strlen(tmp);
  if(tmp[len - 1] == '/')
    tmp[len - 1] = 0;
  for(p = tmp + 1; *p; p++)
    if(*p == '/') {
      *p = 0;
      mkdir(tmp, S_IRWXU);
      *p = '/';
    }
  mkdir(tmp, S_IRWXU);
}

int create_file(const char* fn_)
{
  string fn(fn_);
  auto dn = string(dirname((char*)fn_));

  if (access(dn.c_str(), F_OK) != 0) {
    _mkdir(dn.c_str());
    if (access(dn.c_str(), F_OK) != 0) {
      ostringstream m;
      m << "vrgs::create_file: can't create destination directory " << dn;
      throw runtime_error(m.str());
    }
  }

  int fd = -1;
  if (dn.find("/dev/shm") == 0) {
    //string shm_fn = fn.replace(0, strlen("/dev/shm"), "");
    //cout << "shm_open: " << shm_fn << endl;
    //fd = shm_open(shm_fn.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // NB: looks like shm_open and open is actually the same.
    // but open allow to use slash in the name
    fd = open(fn.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
      ostringstream m;
      m << "vrgs::create_file: can't create shm file " << fn
	<< ", errno " << errno;
      throw runtime_error(m.str());
    }
  } else {
    fd = open(fn.c_str(), O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if (fd == -1) {
      ostringstream m;
      m << "vrgs::create_file: can't create file " << fn
	<< ", errno " << errno;
      throw runtime_error(m.str());
    }
  }

  return fd;
}
