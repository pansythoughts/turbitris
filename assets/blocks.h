
//{{BLOCK(blocks)

//======================================================================
//
//	blocks, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 10 tiles (t reduced) not compressed
//	+ affine map, not compressed, 32x32 
//	Total size: 512 + 640 + 1024 = 2176
//
//	Time-stamp: 2025-10-25, 17:01:19
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BLOCKS_H
#define GRIT_BLOCKS_H

#define blocksTilesLen 640
extern const unsigned int blocksTiles[160];

#define blocksMapLen 1024
extern const unsigned int blocksMap[256];

#define blocksPalLen 512
extern const unsigned int blocksPal[128];

#endif // GRIT_BLOCKS_H

//}}BLOCK(blocks)
