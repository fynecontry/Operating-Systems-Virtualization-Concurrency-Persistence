#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "map-reduce.h"

void *Map(void *file_name) {
    FILE *fp = fopen(file_name, "r");
    assert(fp != NULL);

    char *line = NULL;
    size_t size = 0;
    while (getline(&line, &size, fp) != -1) {
        char *token, *dummy = line;
        while ((token = strsep(&dummy, " .,\t\n\r")) != NULL) {
            if (*token != '\0'){
                MR_Emit(token, "1");
                printf("%s 1\n", token);
            }
        }
    }
    free(line);
    fclose(fp);
}

void Reduce(char *key, Getter get_next, int partition_number) {
    int count = 0;
    char *value;
    while ((value = get_next(key, partition_number)) != NULL){
        count++;
    }
    printf("%s %d\n", key, count);

}

// Example usage
int main(int argc, char *argv[]) {
    Getter get_next = get_value;
    
    MR_Run(argc, argv, Map, argc - 1, Reduce, PARTITION_SIZE, MR_DefaultHashPartition);

    return 0;
}


