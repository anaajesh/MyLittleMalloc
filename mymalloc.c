#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#define MEMLENGTH 4096 
#define ALIGNMENT 8 //@anagha pls check ts --> i remember it being 8 and not 16 but i could be wrong

typedef struct ChunkHeader {
    size_t size;
    int allocated; //1 for allocated and 0 for not allocated
} ChunkHeader;

static union {
    char bytes[MEMLENGTH];
    double not_used;  
} heap; //instantiate the statiic union to create a heap of 4096 bytes

static int initialized = 0; // 0 if the heap isnt set up and 1 otherwise; idk if we REALLY need this but its going to be a sticklet for debugging if we dont 


//HELEPER METHODS HERE?
//we need a method to round up
static size_t round_up(size_t size){
    return (size + 7) & ~((size_t)7); 
    //^ he did this in class its sm easier 
}


static void leak_checker(void){
    char *ptr = heap.bytes; // refering to the first chunk 
    int leaked_objects = 0; 
    int leaked_bytes = 0;

    while (ptr< heap.bytes + MEMLENGTH){
        ChunkHeader *header = (ChunkHeader *)ptr;//treating the address as a chunkheader pointer
        if (header->allocated){
            leaked_objects++;
            leaked_bytes += header->size-sizeof(ChunkHeader);
        }
        ptr += header->size; //bascly move to the next chunk
    }

    if(leaked_objects > 0){
        printf("LEAKED %d OBJECTS FOR A TOTAL OF %d BYTES\n", leaked_objects, leaked_bytes);
    }
}

static void initialize_heap(void){
    ChunkHeader *start = (ChunkHeader *)heap.bytes; //casting the first byte of the heap to a chunkheader pointer
    start->size = MEMLENGTH; 
    start->allocated = 0; 
    initialized = 1; // @anagha again we can remove ts but idek 
    atexit(leak_checker); 
}


//MALLOC SHIT HERE
void *mymalloc (size_t size, char *file, int line){

    if (size == 0){
        return NULL; 
    }

    if(!initialized){
        initialize_heap(); 
    }

    size_t payload_size = round_up(size); 
    size_t total_needed = payload_size + sizeof(ChunkHeader);

    char *ptr = heap.bytes;
    while (ptr < heap.bytes + MEMLENGTH){ 
        ChunkHeader *header = (ChunkHeader *)ptr;
        if(!header->allocated && header->size >= total_needed){ // hceacking if the chunk is free and big enough
           if ( header -> size >= sizeof(ChunkHeader) + total_needed + ALIGNMENT){
                size_t leftover_size = header->size - total_needed;
                header->size = total_needed;//splitting in two 
                ChunkHeader *next_header = (ChunkHeader *)(ptr + total_needed);//leftobver piece starts right after the chunk I just shrunk 
                next_header->size = leftover_size;
                next_header->allocated = 0;
             }
             header->allocated = 1;
             return (void *)(ptr + sizeof(ChunkHeader)); //returning the pointer to payload right after the header
        }

        ptr += header->size;

    }
    //if no chunck was big enough to fit the request
    fprintf(stderr, "malloc: Unable to allocate %zu bytes (%s:%d)\n", size, file, line);
    return NULL;
    //@anagha pls check if this is the right format of the error message i think it is but idk
}

void myfree(void *ptr, char *file, int line){

    if(ptr == NULL){
        return;
    }

    if(!initialized){
        fprintf(stderr, "free: invalid pointer (%s:%d)\n", file, line);
        exit(2);
    }

    char *p = heap.bytes;
    ChunkHeader *current = NULL;

    while(p < heap.bytes + MEMLENGTH){

        current = (ChunkHeader *)p;

        if((void *)(p + sizeof(ChunkHeader)) == ptr){
            break;
        }

        p = p + current->size;
    }

    //pointer wrong -> error
    if(p >= heap.bytes + MEMLENGTH){
        fprintf(stderr, "free: invalid pointer (%s:%d)\n", file, line);
        exit(2);
    }

    if(current->allocated == 0){
        fprintf(stderr, "free: double free detected (%s:%d)\n", file, line); //alr free -> error
        exit(2);
    }

    current->allocated = 0;

    //chunk after
    char *next = p + current->size;

    if(next < heap.bytes + MEMLENGTH){

        ChunkHeader *next_chunk = (ChunkHeader *)next;

        if(next_chunk->allocated == 0){
            current->size = current->size + next_chunk->size;
        }
    }

    //chunk before
    char *before = heap.bytes;
    ChunkHeader *before_chunk = NULL;

    while(before < p){

        before_chunk = (ChunkHeader *)before;
        before = before + before_chunk->size;
    }


    // combine with prev chunk if its free
    if(before_chunk != NULL && before_chunk->allocated == 0){

        before_chunk->size = before_chunk->size + current->size;
    }
}