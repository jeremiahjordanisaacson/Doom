/* =========================================================================
 * wad-info.c - Display information about a DOOM WAD file
 * =========================================================================
 * Reads the WAD header and directory, then prints a human-readable summary
 * of every lump contained in the file.
 *
 * WAD format reference:
 *   Offset  Size  Description
 *   0x00    4     Identification ("IWAD" or "PWAD")
 *   0x04    4     Number of lumps (int32, little-endian)
 *   0x08    4     Offset to the directory table (int32, little-endian)
 *
 * Each 16-byte directory entry:
 *   0x00    4     Offset to lump data (int32)
 *   0x04    4     Lump size in bytes (int32)
 *   0x08    8     Lump name (null-padded ASCII)
 *
 * Usage:
 *   ./wad-info doom1.wad
 *
 * License: GPL v2
 * ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* -------------------------------------------------------------------------
 * WAD structures
 * ------------------------------------------------------------------------- */

#pragma pack(push, 1)

typedef struct {
    char     identification[4]; /* "IWAD" or "PWAD" */
    int32_t  numlumps;         /* number of lumps in the WAD */
    int32_t  infotableofs;     /* offset to the directory table */
} wad_header_t;

typedef struct {
    int32_t  filepos;          /* offset to the lump data */
    int32_t  size;             /* size of the lump in bytes */
    char     name[8];          /* lump name, null-padded */
} wad_dirent_t;

#pragma pack(pop)

/* -------------------------------------------------------------------------
 * Helpers
 * ------------------------------------------------------------------------- */

static void print_usage(const char *progname)
{
    fprintf(stderr, "Usage: %s <file.wad>\n", progname);
    fprintf(stderr, "\nDisplays WAD header information and lump directory.\n");
}

/* Print a lump name safely (it may not be null-terminated if all 8 chars
 * are used). */
static void print_lump_name(const char *name, char *buf, size_t buflen)
{
    size_t i;
    size_t len = 8;

    if (buflen < 9) return;

    for (i = 0; i < len; i++) {
        if (name[i] == '\0') break;
        buf[i] = name[i];
    }
    buf[i] = '\0';
}

/* -------------------------------------------------------------------------
 * Main
 * ------------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
    FILE          *fp   = NULL;
    wad_header_t   header;
    wad_dirent_t  *directory = NULL;
    int32_t        i;
    int            ret = 0;

    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    }

    /* Open the WAD file */
    fp = fopen(argv[1], "rb");
    if (!fp) {
        fprintf(stderr, "Error: cannot open '%s'\n", argv[1]);
        return 1;
    }

    /* Read the 12-byte header */
    if (fread(&header, sizeof(header), 1, fp) != 1) {
        fprintf(stderr, "Error: failed to read WAD header\n");
        ret = 1;
        goto cleanup;
    }

    /* Validate the identification field */
    if (memcmp(header.identification, "IWAD", 4) != 0 &&
        memcmp(header.identification, "PWAD", 4) != 0) {
        fprintf(stderr, "Error: '%s' is not a valid WAD file "
                "(bad identification: %.4s)\n",
                argv[1], header.identification);
        ret = 1;
        goto cleanup;
    }

    /* Print header information */
    printf("File:              %s\n", argv[1]);
    printf("WAD Type:          %.4s\n", header.identification);
    printf("Number of lumps:   %d\n", header.numlumps);
    printf("Directory offset:  %d\n", header.infotableofs);
    printf("\n");

    /* Sanity check */
    if (header.numlumps <= 0) {
        printf("(WAD contains no lumps)\n");
        goto cleanup;
    }

    /* Seek to the directory table */
    if (fseek(fp, header.infotableofs, SEEK_SET) != 0) {
        fprintf(stderr, "Error: cannot seek to directory at offset %d\n",
                header.infotableofs);
        ret = 1;
        goto cleanup;
    }

    /* Read the entire directory */
    directory = (wad_dirent_t *)malloc(sizeof(wad_dirent_t) * header.numlumps);
    if (!directory) {
        fprintf(stderr, "Error: out of memory allocating directory\n");
        ret = 1;
        goto cleanup;
    }

    if (fread(directory, sizeof(wad_dirent_t), header.numlumps, fp)
        != (size_t)header.numlumps) {
        fprintf(stderr, "Error: failed to read lump directory\n");
        ret = 1;
        goto cleanup;
    }

    /* Print the directory listing */
    printf("Lump Directory:\n");
    printf("%-8s%-16s%s\n", "#", "Name", "Size");
    printf("----------------------------------------\n");

    for (i = 0; i < header.numlumps; i++) {
        char name_buf[9];
        print_lump_name(directory[i].name, name_buf, sizeof(name_buf));
        printf("%-8d%-16s%d\n", i, name_buf, directory[i].size);
    }

    printf("\nTotal lumps: %d\n", header.numlumps);

cleanup:
    free(directory);
    if (fp) fclose(fp);
    return ret;
}
