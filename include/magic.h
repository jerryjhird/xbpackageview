#ifndef MAGIC_H
#define MAGIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>

// package types
#define STFS_CON 0x434F4E20
#define STFS_LIVE 0x4C495645
#define STFS_PIRS 0x50495253

// content types
#define STFS_SAVEGAME     0x1
#define STFS_DLC          0x2
#define STFS_AVATARITEM   0x9000
#define STFS_PROFILE      0x10000
#define STFS_GAMERPIC     0x20000
#define STFS_THEME        0x30000
#define STFS_TITLEUPDATE  0xB0000
#define STFS_ARCADEGAME   0xD0000

// header offsets
#define OFFSET_PACKAGE_TYPE      0x0
#define OFFSET_CONTENT_TYPE      0x344
#define OFFSET_TITLE_ID          0x360
#define OFFSET_CONTENT_ID        0x32C
#define OFFSET_PACKAGE_NAME      0x411
#define OFFSET_PACKAGE_DESC      0xD11
#define OFFSET_PUBLISHER_NAME    0x1611
#define OFFSET_TITLE_NAME        0x1691
#define OFFSET_PACKAGE_THUMB     0x1712
#define OFFSET_TITLE_THUMB       0x1716

// file table offsets
#define OFFSET_FILETABLE_ENTRY_ID    0x340
#define OFFSET_FILETABLE_STARTBLOCK  0x379 + 0x4
#define OFFSET_FILETABLE_BLOCKCOUNT  0x379 + 0x2

#define BLOCK_SIZE 0x1000
#define FILEENTRY_SIZE 0x40
#define ASCII_NAME_OFFSET 0x28
#define PARENT_DIR_OFFSET 0x32
#define FILE_SIZE_OFFSET 0x34
#define STARTING_BLOCK_OFFSET 0x2F
#define NUM_BLOCKS_OFFSET 0x29

// false = return actual content type / true = return formatted content type
extern bool fancymode;

// helpers

void read_utf16be_file(FILE *f, long offset, size_t max_chars, wchar_t *out); /// read UTF-16BE files at offset
uint32_t read32be_file(FILE *f, long offset); // read 32-bit big-endian interger from file at offset

/// get specific package values
const char* package_type(uint32_t type);
const char* content_type(uint32_t type);

#ifdef __cplusplus
}
#endif

#endif // MAGIC_H