
//{{BLOCK(blocks)

//======================================================================
//
//	blocks, 256x256@8, 
//	Transparent color : FF,00,FF
//	+ palette 256 entries, not compressed
//	+ 9 tiles (t reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 576 + 2048 = 3136
//
//	Time-stamp: 2025-10-25, 17:17:45
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.9
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BLOCKS_H
#define GRIT_BLOCKS_H

#define blocksTilesLen 576
extern const unsigned char blocksTiles[576];

#define blocksMapLen 2048
extern const unsigned short blocksMap[1024];

#define blocksPalLen 512
extern const unsigned short blocksPal[256];

#endif // GRIT_BLOCKS_H

//}}BLOCK(blocks)

//{{BLOCK(Shared)

//======================================================================
//
//	Shared, 16x16@8, 
//	Transparent color : FF,00,FF
//	+ palette 256 entries, not compressed
//	Total size: 512 = 512
//
//	Time-stamp: 2025-10-25, 17:11:14
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.9
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SHARED_H
#define GRIT_SHARED_H

#define SharedPalLen 512
extern const unsigned short SharedPal[256];

#endif // GRIT_SHARED_H

//}}BLOCK(Shared)
