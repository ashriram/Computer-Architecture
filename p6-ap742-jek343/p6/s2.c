int test19() {
    int n_tries = 10000;
    int block_size;

    // 1024 bytes of padding
    // --------------------
    // 1024 bytes of "heap"
    // --------------------  <-- heap_start
    // 1024 bytes of "padding"
    char memarea[HEAP_SIZE*3];
    //fill padding with an arbitrary value
    for (int i = 0; i < 1024; i++)
    {
      memarea[i] = 'a';
    }
    for(int i = 2048; i < 3072; i++)
    {
      memarea[i] = 'a';
    }

    char *heap_start = &memarea[1024]; // heap will start 1024 bytes in
    char *pointers[NPOINTERS];
    int block_options[] = {1,5,8,14,23,32,64,71,88,101,223};
    int num_blocks = 11;
    int block_sizes[NPOINTERS];
    char characters[26];
    for (int i = 0; i < 26;i++)
    {
      characters[i] = i+'a';
    }
    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));

    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++)
    {
        int index = random() % NPOINTERS;
        block_size = block_options[random() % num_blocks];
        if (pointers[index] == NULL)
        {
            pointers[index] = hl_alloc(heap_start, block_size);
            if(pointers[index] != NULL)
            {
              char* array = (char*) pointers[index];
              for(int j = 0; j < block_size; j++)
              {
                array[j] = characters[index%26];
              }
              block_sizes[index] = block_size;
            }
        }
        else
        {
            hl_release(heap_start, pointers[index]);
            pointers[index] = NULL;
        }
        for(int j = 0; j < NPOINTERS; j++)
        {
          for(int b = 0; b < block_sizes[j]; b++)
          {
            char* array = (char*) pointers[j];
            if(array != NULL)
            {
              if(array[b] != (characters[j%26]))
              {
                return FAILURE;
              }
            }
          }
        }
    }

    for (int i = 0; i < 1024; i++){
      if (memarea[i] != 'a'){
        return FAILURE;
      }
    }

    for(int i = 2048; i < 3072; i++){
      if (memarea[i] != 'a'){
        return FAILURE;
      }
    }

    return SUCCESS;
}
