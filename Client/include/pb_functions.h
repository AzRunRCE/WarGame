#ifndef PB_FUNCTIONS_H
#define PB_FUNCTIONS_H
#include "common.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "unionproto.pb.h"
const pb_field_t* decode_unionmessage_type(pb_istream_t *stream);
bool encode_unionmessage(pb_ostream_t *stream, const pb_field_t messagetype[], const void *message);
bool decode_unionmessage_contents(pb_istream_t *stream, const pb_field_t fields[], void *dest_struct);
bool print_string(pb_istream_t *stream, const pb_field_t *field, void **arg);
bool encode_repeatedstring(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
bool readPlayers_callback(pb_istream_t *stream, const pb_field_t *field, void **arg);
bool readBullets_callback(pb_istream_t *stream, const pb_field_t *field, void **arg);
#endif /* guard */
