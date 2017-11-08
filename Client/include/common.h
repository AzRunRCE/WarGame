#ifndef COMMON_H
#define COMMON_H

#include "pb.h"

pb_ostream_t pb_ostream_from_socket(int fd);
pb_istream_t pb_istream_from_socket(int fd);

#endif /* COMMON_H */