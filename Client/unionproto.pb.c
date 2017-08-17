/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.8 at Tue Aug 08 15:56:51 2017. */

#include "include\unionproto.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t BulletMessage_fields[2] = {
	PB_FIELD(1, MESSAGE , REQUIRED, STATIC  , FIRST, BulletMessage, Pos, Pos, &SDL_Rect_fields),
	PB_LAST_FIELD
};

const pb_field_t SDL_Rect_fields[5] = {
	PB_FIELD(1, INT32   , REQUIRED, STATIC  , FIRST, SDL_Rect, x, x, 0),
	PB_FIELD(2, INT32   , REQUIRED, STATIC  , OTHER, SDL_Rect, y, x, 0),
	PB_FIELD(3, INT32   , REQUIRED, STATIC  , OTHER, SDL_Rect, h, y, 0),
	PB_FIELD(4, INT32   , REQUIRED, STATIC  , OTHER, SDL_Rect, w, h, 0),
	PB_LAST_FIELD
};

const pb_field_t Player_fields[17] = {
	PB_FIELD(1, INT32   , REQUIRED, STATIC  , FIRST, Player, x, x, 0),
	PB_FIELD(2, INT32   , REQUIRED, STATIC  , OTHER, Player, y, x, 0),
	PB_FIELD(3, STRING  , REQUIRED, STATIC  , OTHER, Player, name, y, 0),
	PB_FIELD(4, INT32   , REQUIRED, STATIC  , OTHER, Player, id, name, 0),
	PB_FIELD(5, MESSAGE , REPEATED, CALLBACK, OTHER, Player, bullets, id, &SDL_Rect_fields),
	PB_FIELD(6, BOOL    , REQUIRED, STATIC  , OTHER, Player, calibred, bullets, 0),
	PB_FIELD(7, INT32   , REQUIRED, STATIC  , OTHER, Player, health, calibred, 0),
	PB_FIELD(8, INT32   , REQUIRED, STATIC  , OTHER, Player, ammo, health, 0),
	PB_FIELD(9, INT32   , REQUIRED, STATIC  , OTHER, Player, step, ammo, 0),
	PB_FIELD(10, BOOL    , REQUIRED, STATIC  , OTHER, Player, fire, step, 0),
	PB_FIELD(11, INT32   , REQUIRED, STATIC  , OTHER, Player, state, fire, 0),
	PB_FIELD(12, BOOL    , REQUIRED, STATIC  , OTHER, Player, walk, state, 0),
	PB_FIELD(13, INT32   , REQUIRED, STATIC  , OTHER, Player, lastAnim, walk, 0),
	PB_FIELD(14, INT32   , REQUIRED, STATIC  , OTHER, Player, fireIdle, lastAnim, 0),
	PB_FIELD(15, MESSAGE , REQUIRED, STATIC  , OTHER, Player, Pos, fireIdle, &SDL_Rect_fields),
	PB_FIELD(16, MESSAGE , REQUIRED, STATIC  , OTHER, Player, sprite, Pos, &SDL_Rect_fields),
	PB_LAST_FIELD
};

const pb_field_t GameDataMessage_fields[3] = {
	PB_FIELD(1, INT32   , REQUIRED, STATIC  , FIRST, GameDataMessage, GameMode, GameMode, 0),
	PB_FIELD(2, MESSAGE , REPEATED, CALLBACK, OTHER, GameDataMessage, players, GameMode, &Player_fields),
	PB_LAST_FIELD
};

const pb_field_t ConnectionMessage_fields[2] = {
	PB_FIELD(1, STRING  , REQUIRED, STATIC  , FIRST, ConnectionMessage, name, name, 0),
	PB_LAST_FIELD
};

const pb_field_t ConnectionCallbackMessage_fields[5] = {
	PB_FIELD(1, BOOL    , REQUIRED, STATIC  , FIRST, ConnectionCallbackMessage, sucess, sucess, 0),
	PB_FIELD(2, INT32   , REQUIRED, STATIC  , OTHER, ConnectionCallbackMessage, clentId, sucess, 0),
	PB_FIELD(3, MESSAGE , REQUIRED, STATIC  , OTHER, ConnectionCallbackMessage, player, clentId, &Player_fields),
	PB_FIELD(4, STRING  , REQUIRED, STATIC  , OTHER, ConnectionCallbackMessage, motd, player, 0),
	PB_LAST_FIELD
};
const pb_field_t UnionMessage_fields[5] = {
	PB_FIELD(1, MESSAGE , OPTIONAL, STATIC  , FIRST, UnionMessage, a, a, &BulletMessage_fields),
	PB_FIELD(2, MESSAGE , OPTIONAL, STATIC  , OTHER, UnionMessage, b, a, &ConnectionCallbackMessage_fields),
	PB_FIELD(3, MESSAGE , OPTIONAL, STATIC  , OTHER, UnionMessage, c, b, &ConnectionMessage_fields),
	PB_FIELD(4, MESSAGE , OPTIONAL, STATIC  , OTHER, UnionMessage, d, c, &GameDataMessage_fields),
	PB_LAST_FIELD
};


/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_32BIT
* compile-time option. You can do that in pb.h or on compiler command line.
*
* The reason you need to do this is that some of your messages contain tag
* numbers or field sizes that are larger than what can fit in 8 or 16 bit
* field descriptors.
*/
PB_STATIC_ASSERT((pb_membersize(BulletMessage, Pos) < 65536 && pb_membersize(Player, Pos) < 65536 && pb_membersize(Player, sprite) < 65536 && pb_membersize(ConnectionCallbackMessage, player) < 65536 && pb_membersize(UnionMessage, a) < 65536 && pb_membersize(UnionMessage, b) < 65536 && pb_membersize(UnionMessage, c) < 65536 && pb_membersize(UnionMessage, d) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_BulletMessage_SDL_Rect_Player_GameDataMessage_ConnectionMessage_ConnectionCallbackMessage_UnionMessage)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
	/* If you get an error here, it means that you need to define PB_FIELD_16BIT
	* compile-time option. You can do that in pb.h or on compiler command line.
	*
	* The reason you need to do this is that some of your messages contain tag
	* numbers or field sizes that are larger than what can fit in the default
	* 8 bit descriptors.
	*/
	PB_STATIC_ASSERT((pb_membersize(BulletMessage, Pos) < 256 && pb_membersize(Player, Pos) < 256 && pb_membersize(Player, sprite) < 256 && pb_membersize(ConnectionCallbackMessage, player) < 256 && pb_membersize(UnionMessage, a) < 256 && pb_membersize(UnionMessage, b) < 256 && pb_membersize(UnionMessage, c) < 256 && pb_membersize(UnionMessage, d) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT_FOR_MESSAGES_BulletMessage_SDL_Rect_Player_GameDataMessage_ConnectionMessage_ConnectionCallbackMessage_UnionMessage)
#endif


	/* @@protoc_insertion_point(eof) */