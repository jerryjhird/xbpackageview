#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>
#include "magic.h"
#include <string.h>

uint32_t read32be_file(FILE *f, long offset) {
    uint8_t buf[4];
    if (fseek(f, offset, SEEK_SET) != 0 || fread(buf, 1, 4, f) != 4) return 0;
    return (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
}

void read_utf16be_file(FILE *f, long offset, size_t max_chars, wchar_t *out, uint8_t *buf) {
    if (max_chars == 0) return;

    size_t nbytes = max_chars * 2;

    if (fseek(f, offset, SEEK_SET) != 0) {
        out[0] = L'\0';
        return;
    }

    size_t nread = fread(buf, 1, nbytes, f);
    if (nread == 0) {
        out[0] = L'\0';
        return;
    }

    size_t num_chars = nread / 2;
    size_t num_to_convert = (num_chars < max_chars - 1) ? num_chars : max_chars - 1;
    size_t i = 0;

    for (; i < num_to_convert; ++i) {
        wchar_t c = (buf[i * 2] << 8) | buf[i * 2 + 1];
        out[i] = c;
        if (c == 0) break;
    }
    out[i] = L'\0';
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
