#include "mmap-utils.h"
#include "file-utils.h"

#include <sys/unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

#include <stdexcept>
#include <sstream>

using namespace std;

mmap_descr_t create_mmap_segment(const char* fn, size_t segment_size)
{
  int fd = create_file(fn);

  if (ftruncate(fd, segment_size) == -1) {
    throw runtime_error("create_segment: ftruncate failed");
  }

  /* Map shared memory object */
  void* rptr = mmap(NULL, segment_size,
		    PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (rptr == MAP_FAILED) {
    ostringstream m;
    m << "create_segment: mmap failed, errno: " << errno
      << ": " << strerror(errno);
    throw runtime_error(m.str());
  }

  mmap_descr_t seg = {.fd = fd, .start_addr = rptr, .segment_size = segment_size};
  return seg;
}
