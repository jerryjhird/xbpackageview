#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>
#include "magic.h"

uint32_t read32be_file(FILE *f, long offset) {
    uint8_t buf[4];
    if (fseek(f, offset, SEEK_SET) != 0 || fread(buf, 1, 4, f) != 4) return 0;
    return (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
}

void read_utf16be_file(FILE *f, long offset, size_t max_chars, wchar_t *out) {
    for (size_t i = 0; i < max_chars; i++) {
        uint8_t b[2];
        if (fseek(f, offset + i * 2, SEEK_SET) != 0 || fread(b, 1, 2, f) != 2) {
            out[i] = L'\0';
            return;
        }
        uint16_t val = (b[0] << 8) | b[1];
        if (val == 0x0000) {
            out[i] = L'\0';
            return;
        }
        out[i] = val;
    }
    out[max_chars - 1] = L'\0';
}

const char* package_type(uint32_t type) {
    switch(type) {
        case STFS_CON: return "CON";
        case STFS_PIRS: return "PIRS";
        case STFS_LIVE: return "LIVE";
        default: return "Unknown";
    }
}

const char* content_type(uint32_t type) {
    switch(type) {
        case STFS_SAVEGAME:    return fancymode ? "Saved Game (SAVEGAME)" : "SAVEGAME";
        case STFS_DLC:         return fancymode ? "Downloadable Content (DLC)" : "DLC";
        case STFS_PROFILE:     return fancymode ? "User Profile (PROFILE)" : "PROFILE";
        case STFS_ARCADEGAME:  return fancymode ? "Xbox Live Arcade Game (XBLA)" : "XBLA";
        case STFS_TITLEUPDATE: return fancymode ? "Title Update (TITLEUPDATE)" : "TITLEUPDATE";
        default: return "Unknown Content Type";
    }
}