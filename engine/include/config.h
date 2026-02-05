/* config.h - Multi-platform configuration for PrBoom DOOM Engine
 *
 * This replaces the Android-specific config.h with a portable version.
 * Platform-specific settings are handled via preprocessor detection.
 *
 * Part of the DOOM Multiplatform Mega-Repo
 */

#ifndef PRBOOM_CONFIG_H
#define PRBOOM_CONFIG_H

/* Uncomment this to exhaustively run memory checks while the game is running
   (this is EXTREMELY slow). */
/* #undef CHECKHEAP */

/* Define for support for MBF helper dogs */
/* #define DOGS 1 */

/* Define to be the path where Doom WADs are stored */
#ifndef DOOMWADDIR
  #ifdef _WIN32
    #define DOOMWADDIR "."
  #elif defined(__ANDROID__)
    #define DOOMWADDIR "/sdcard/doom"
  #elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
      #define DOOMWADDIR "."
    #else
      #define DOOMWADDIR "/Library/Application Support/doom"
    #endif
  #elif defined(__EMSCRIPTEN__)
    #define DOOMWADDIR "/"
  #else
    #define DOOMWADDIR "/usr/share/doom"
  #endif
#endif

/* Define if you are building with OpenGL support */
/* #undef GL_DOOM */

/* ============================================================
 * Platform feature detection
 * ============================================================ */

#ifdef _WIN32
  /* Windows (MSVC or MinGW) */
  #define HAVE_ASM_BYTEORDER_H 0
  #define HAVE_INET_NTOP 1
  #define HAVE_INET_PTON 1
  #define HAVE_INTTYPES_H 1
  #define HAVE_LIBM 1
  #define HAVE_MMAP 0
  #define HAVE_SNPRINTF 1
  #define HAVE_STDINT_H 1
  #define HAVE_STDLIB_H 1
  #define HAVE_STRINGS_H 0
  #define HAVE_STRING_H 1
  #define HAVE_UNISTD_H 0
  #define HAVE_USLEEP 0

  #ifdef _MSC_VER
    #define stricmp _stricmp
    #define strnicmp _strnicmp
  #else
    #define stricmp strcasecmp
    #define strnicmp strncasecmp
  #endif

#elif defined(__ANDROID__)
  /* Android NDK */
  #define HAVE_ASM_BYTEORDER_H 0
  #define HAVE_INET_NTOP 0
  #define HAVE_INET_PTON 0
  #define HAVE_INTTYPES_H 0
  #define HAVE_LIBM 1
  #define HAVE_MMAP 0
  #define HAVE_SNPRINTF 0
  #define HAVE_STDINT_H 0
  #define HAVE_STDLIB_H 1
  #define HAVE_STRINGS_H 0
  #define HAVE_STRING_H 1
  #define HAVE_UNISTD_H 1
  #define HAVE_USLEEP 1
  #define stricmp strcasecmp
  #define strnicmp strncasecmp

#elif defined(__EMSCRIPTEN__)
  /* Emscripten / WebAssembly */
  #define HAVE_ASM_BYTEORDER_H 0
  #define HAVE_INET_NTOP 0
  #define HAVE_INET_PTON 0
  #define HAVE_INTTYPES_H 1
  #define HAVE_LIBM 1
  #define HAVE_MMAP 0
  #define HAVE_SNPRINTF 1
  #define HAVE_STDINT_H 1
  #define HAVE_STDLIB_H 1
  #define HAVE_STRINGS_H 1
  #define HAVE_STRING_H 1
  #define HAVE_UNISTD_H 1
  #define HAVE_USLEEP 1
  #define stricmp strcasecmp
  #define strnicmp strncasecmp

#elif defined(__APPLE__)
  /* macOS, iOS, watchOS */
  #define HAVE_ASM_BYTEORDER_H 0
  #define HAVE_INET_NTOP 1
  #define HAVE_INET_PTON 1
  #define HAVE_INTTYPES_H 1
  #define HAVE_LIBM 1
  #define HAVE_MMAP 1
  #define HAVE_SNPRINTF 1
  #define HAVE_STDINT_H 1
  #define HAVE_STDLIB_H 1
  #define HAVE_STRINGS_H 1
  #define HAVE_STRING_H 1
  #define HAVE_UNISTD_H 1
  #define HAVE_USLEEP 1
  #define stricmp strcasecmp
  #define strnicmp strncasecmp

#else
  /* Generic POSIX / Linux */
  #define HAVE_ASM_BYTEORDER_H 1
  #define HAVE_INET_NTOP 1
  #define HAVE_INET_PTON 1
  #define HAVE_INTTYPES_H 1
  #define HAVE_LIBM 1
  #define HAVE_MMAP 1
  #define HAVE_SNPRINTF 1
  #define HAVE_STDINT_H 1
  #define HAVE_STDLIB_H 1
  #define HAVE_STRINGS_H 1
  #define HAVE_STRING_H 1
  #define HAVE_UNISTD_H 1
  #define HAVE_USLEEP 1
  #define stricmp strcasecmp
  #define strnicmp strncasecmp
#endif

/* ============================================================
 * Optional library support
 * ============================================================ */

/* PNG screenshot support */
/* #undef HAVE_LIBPNG */

/* SDL_mixer for music */
/* #undef HAVE_LIBSDL_MIXER */

/* SDL_net for networking */
/* #undef HAVE_LIBSDL_NET */

/* Network game support */
/* #undef HAVE_NET */

/* ============================================================
 * Engine configuration
 * ============================================================ */

/* High resolution support */
#define HIGHRES 1

/* 386 assembly optimizations (x86 only) */
#if defined(__i386__) || defined(_M_IX86)
  #define I386_ASM 1
#else
  /* #undef I386_ASM */
#endif

/* Package information */
#define PACKAGE "PrBoom"
#define PACKAGE_BUGREPORT "https://github.com/prboom/prboom"
#define PACKAGE_NAME "PrBoom"
#define PACKAGE_STRING "PrBoom 2.6.0"
#define PACKAGE_TARNAME "prboom"
#define PACKAGE_VERSION "2.6.0"
#define VERSION "2.6.0"

/* Packed structure attribute */
#ifdef _MSC_VER
  #define PACKEDATTR
  /* Use #pragma pack(push, 1) / #pragma pack(pop) around packed structs */
#else
  #define PACKEDATTR __attribute__((packed))
#endif

/* Enable internal range checking (debug builds) */
/* #undef RANGECHECK */

/* Old SDL_net compatibility */
/* #undef SDL_NET_UDP_PACKET_SRC */

/* Simple runtime checks */
#define SIMPLECHECKS 1

/* ANSI C headers available */
#define STDC_HEADERS 1

/* Timing diagnostics */
/* #undef TIMEDIAG */

/* GLU support */
/* #undef USE_GLU_IMAGESCALE */
/* #undef USE_GLU_MIPMAP */

/* SDL net usage */
/* #undef USE_SDL_NET */

/* Debug malloc */
/* #undef WITH_DMALLOC */

/* Big endian detection */
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  #define WORDS_BIGENDIAN 1
#else
  /* #undef WORDS_BIGENDIAN */
#endif

/* Zone memory ID checks */
#define ZONEIDCHECK 1

/* Type compatibility */
/* #undef const */
/* #undef gid_t */
/* #undef size_t */
/* #undef uid_t */

#ifndef __cplusplus
  /* #undef inline */
#endif

#endif /* PRBOOM_CONFIG_H */
