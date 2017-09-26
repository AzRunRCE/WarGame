#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/common.h"
#include "include/pb_encode.h"
#include "include/pb_decode.h"
#include "include/unionproto.pb.h"

const pb_field_t* decode_unionmessage_type(pb_istream_t *stream)
{
	pb_wire_type_t wire_type;
	uint32_t tag;
	bool eof;

	while (pb_decode_tag(stream, &wire_type, &tag, &eof))
	{
		if (wire_type == PB_WT_STRING)
		{
			const pb_field_t *field;
			for (field = UnionMessage_fields; field->tag != 0; field++)
			{
				if (field->tag == tag && (field->type & PB_LTYPE_SUBMESSAGE))
				{
					/* Found our field. */
					return field->ptr;
				}
			}
		}

		/* Wasn't our field.. */
		pb_skip_field(stream, wire_type);
	}

	return NULL;
}
bool encode_unionmessage(pb_ostream_t *stream, const pb_field_t messagetype[], const void *message)
{
	const pb_field_t *field;
	for (field = UnionMessage_fields; field->tag != 0; field++)
	{
		if (field->ptr == messagetype)
		{
			/* This is our field, encode the message using it. */
			if (!pb_encode_tag_for_field(stream, field))
				return false;

			return pb_encode_submessage(stream, messagetype, message);
		}
	}

	/* Didn't find the field for messagetype */
	return false;
}

bool decode_unionmessage_contents(pb_istream_t *stream, const pb_field_t fields[], void *dest_struct)
{
	pb_istream_t substream;
	bool status;
	if (!pb_make_string_substream(stream, &substream))
		return false;

	status = pb_decode(&substream, fields, dest_struct);
	pb_close_string_substream(stream, &substream);
	return status;
}

bool print_string(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	uint8_t buffer[1024] = { 0 };

	/* We could read block-by-block to avoid the large buffer... */
	if (stream->bytes_left > sizeof(buffer) - 1)
		return false;

	if (!pb_read(stream, buffer, stream->bytes_left))
		return false;

	/* Print the string, in format comparable with protoc --decode.
	* Format comes from the arg defined in main().
	*/
	printf((char*)*arg, buffer);
	return true;
}

bool encode_repeatedstring(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
	//char *str[6] = { "Hello world!", "Hi", "Bonjour", "Salut" , "Bonjour", "Salut" };
	char str[7] = "coucou\0";

	/*for (i = 0; i < 6; i++)
	{*/
	if (!pb_encode_tag_for_field(stream, field))
		return false;

	if (!pb_encode_string(stream, (uint8_t*)str, strlen(str)))
		return false;
	//}
	return true;
}


//bool printBullet_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
//{
//	BulletMessage Bullet;
//
//	if (!pb_decode(stream, BulletMessage_fields, &Bullet))
//		return false;
//
//	printf("%d %d\n", Bullet.Pos.x, Bullet.Pos.y);
//
//	return true;
//}
//
//bool printPlayer_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
//{
//	Player Player;
//	Player.bullets.funcs.decode = &printBullet_callback;
//	if (!pb_decode(stream, Player_fields, &Player))
//		return false;
//
//	printf("%s %d %d\n", Player.name, Player.Pos.x, Player.Pos.y);
//
//	return true;
//}
//

