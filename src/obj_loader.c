//
// Created by asgyu on 2026. febr. 25..
//

#include "obj_loader.h"
#include <stdio.h>
#include <stdlib.h>

void load_obj(const char* filename, Vertex** vertices, int* number_of_vertices) {
    printf("[INFO] %s beolvasasa elkezdodott\n", filename);

    FILE* file = fopen(filename, "r");
    if(!file){
        printf("[HIBA] Nem sikerult beolvasni a kovetkezo fajlt: %s\n", filename);
        return;
    }
    char line[22000];
    int count = 0;

    while(fgets(line, sizeof(line), file)){
        if(line[0] == 'v' && line[1] == ' ') count++;
    }
    if(count == 0) {
        printf("[HIBA] Nem talalhato pont a kovetkezo fajlban: %s\n", filename);
        fclose(file);
        return;
    }

    *number_of_vertices = count;
    *vertices = (Vertex*)malloc((*number_of_vertices)*sizeof(Vertex));

    if(*vertices == NULL) {
        printf("[HIBA] Memoriafoglalasi hiba\n");
        fclose(file);
        return;
    }

    rewind(file);
    int i = 0;
    while(fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            sscanf(line, "v %f %f %f", &(*vertices)[i].x, &(*vertices)[i].y, &(*vertices)[i].z);
            i++;
        }
    }
    fclose(file);
    printf("[SIKER] %s sikeresen beolvasva\n", filename);
}