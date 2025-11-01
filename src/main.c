#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include "magic.h"

bool fancymode = false;

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-f] <package>\n", argv[0]);
        return 1;
    }

    setlocale(LC_ALL, "");

    const char *filepath = NULL;

    if (argc >= 3 && strcmp(argv[1], "-f") == 0) {
        fancymode = true;
        filepath = argv[2];
    } else {
        filepath = argv[1];
    }

    if (!filepath) {
        fprintf(stderr, "Error: No package file specified.\n");
        return 1;
    }

    FILE *f = fopen(filepath, "rb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    uint8_t buf[0x1200]; // shared read buffer for UTF16 reads

    // --- core header fields ---
    uint32_t package_type_var = read32be_file(f, OFFSET_PACKAGE_TYPE);
    uint32_t content_type_var = read32be_file(f, OFFSET_CONTENT_TYPE);
    uint32_t title_id = read32be_file(f, OFFSET_TITLE_ID);

    // --- extended metadata fields ---
    uint32_t content_id = read32be_file(f, OFFSET_CONTENT_ID);

    wchar_t package_name[0x900] = {0};
    wchar_t title_name[0x80] = {0};
    wchar_t publisher_name[0x80] = {0};
    wchar_t package_desc[0x900] = {0};

    read_utf16be_file(f, OFFSET_PACKAGE_NAME, 0x900, package_name, buf);
    read_utf16be_file(f, OFFSET_TITLE_NAME, 0x80, title_name, buf);
    read_utf16be_file(f, OFFSET_PUBLISHER_NAME, 0x80, publisher_name, buf);
    read_utf16be_file(f, OFFSET_PACKAGE_DESC, 0x900, package_desc, buf);

    fclose(f);

    // --- output section ---
    wprintf(L"package type: %s\n", package_type(package_type_var));
    wprintf(L"content type: %s (0x%08X)\n\n", content_type(content_type_var), content_type_var);
    wprintf(L"title ID: %08X\n", title_id);
    wprintf(L"content ID: %08X\n\n", content_id);

    if (package_name[0])   wprintf(L"package name: %ls\n", package_name);
    if (package_desc[0])   wprintf(L"description: %ls\n", package_desc);
    if (title_name[0])     wprintf(L"title name: %ls\n", title_name);
    if (publisher_name[0]) wprintf(L"publisher: %ls\n", publisher_name);

    return 0;
}