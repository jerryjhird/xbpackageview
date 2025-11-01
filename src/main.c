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

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            fancymode = true;
        } else if (!filepath) {
            filepath = argv[i];
        }
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

    uint32_t package_type_var = read32be_file(f, OFFSET_PACKAGE_TYPE);
    uint32_t content_type_var = read32be_file(f, OFFSET_CONTENT_TYPE);
    uint32_t title_id = read32be_file(f, OFFSET_TITLE_ID);

    wchar_t package_name[0x900] = {0};
    wchar_t title_name[0x80] = {0};
    wchar_t publisher_name[0x80] = {0};

    read_utf16be_file(f, OFFSET_PACKAGE_NAME, 0x900, package_name);
    read_utf16be_file(f, OFFSET_TITLE_NAME, 0x80, title_name);
    read_utf16be_file(f, OFFSET_PUBLISHER_NAME, 0x80, publisher_name);

    fclose(f);

    wprintf(L"package type: %s\n", package_type(package_type_var));
    wprintf(L"content type: %s (0x%08X)\n", content_type(content_type_var), content_type_var);
    wprintf(L"title ID: %08X\n", title_id);

    if (package_name[0])   wprintf(L"package name: %ls\n", package_name);
    if (title_name[0])     wprintf(L"title name: %ls\n", title_name);
    if (publisher_name[0]) wprintf(L"publisher: %ls\n", publisher_name);

    return 0;
}