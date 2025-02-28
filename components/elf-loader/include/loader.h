#ifndef LOADER_HPP
#define LOADER_HPP

#include <stdio.h>
#include <stdint.h>

#if defined(__linux__)
#define LOADER_FD_T FILE *
#endif

#if !(defined(__linux__))
#define LOADER_FD_T void*
#endif


typedef struct {
    const char *name; /*!< Name of symbol */
    void *ptr; /*!< Pointer of symbol in memory */
} ELFLoaderSymbol_t;

typedef struct {
    const ELFLoaderSymbol_t *exported; /*!< Pointer to exported symbols array */
    unsigned int exported_size; /*!< Elements on exported symbol array */
} ELFLoaderEnv_t;



typedef struct ELFLoaderSection_t {
    void *data;
    int secIdx;
    size_t size;
    off_t relSecIdx;
    struct ELFLoaderSection_t* next;
} ELFLoaderSection_t;



struct ELFLoaderContext_t {
    LOADER_FD_T fd;
    void* exec;
    void* text;
    const ELFLoaderEnv_t *env;

    size_t e_shnum;
    off_t e_shoff;
    off_t shstrtab_offset;

    size_t symtab_count;
    off_t symtab_offset;
    off_t strtab_offset;

    ELFLoaderSection_t* section;
};

typedef struct ELFLoaderContext_t ELFLoaderContext_t;



int elfLoader(LOADER_FD_T fd,const ELFLoaderEnv_t *env,char *funcname,int arg);
intptr_t elfLoaderRun(ELFLoaderContext_t *ctx,intptr_t arg);
int elfLoaderSetFunc(ELFLoaderContext_t *ctx,const char *funcname);
ELFLoaderContext_t *elfLoaderInitLoadAndRelocate(LOADER_FD_T fd,const ELFLoaderEnv_t *env);
void elfLoaderFree(ELFLoaderContext_t *ctx);
void* elfLoaderGetTextAddr(ELFLoaderContext_t *ctx);


#endif //  LOADER_HPP
