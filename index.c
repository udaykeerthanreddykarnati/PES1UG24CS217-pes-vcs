// index.c (commit 2)

#include "index.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

int object_write(ObjectType type, const void *data, size_t len, ObjectID *id_out);
uint32_t get_file_mode(const char *path);

IndexEntry* index_find(Index *index, const char *path) {
    for (int i = 0; i < index->count; i++) {
        if (strcmp(index->entries[i].path, path) == 0)
            return &index->entries[i];
    }
    return NULL;
}

int index_load(Index *index) {
    index->count = 0;

    FILE *f = fopen(INDEX_FILE, "r");
    if (!f) return 0;

    char hex[HASH_HEX_SIZE + 1];
    while (index->count < MAX_INDEX_ENTRIES) {
        IndexEntry *e = &index->entries[index->count];

        int n = fscanf(f, "%o %64s %llu %llu %255s",
                       &e->mode,
                       hex,
                       (unsigned long long *)&e->mtime_sec,
                       (unsigned long long *)&e->size,
                       e->path);

        if (n == EOF) break;
        if (n != 5) { fclose(f); return -1; }

        if (hex_to_hash(hex, &e->hash) < 0) { fclose(f); return -1; }
        index->count++;
    }

    fclose(f);
    return 0;
}

int index_save(const Index *index) {
    return 0;
}

int index_add(Index *index, const char *path) {
    return 0;
}