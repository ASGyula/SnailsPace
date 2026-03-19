//
// Created by asgyu on 2026. febr. 25..
//

#include "obj_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ASSETS_PREFIX "../assets/"

void load_obj(const char* name, Vertex** vertices){
    char filename[256];
    snprintf(filename, sizeof(filename), "%s%s", ASSETS_PREFIX, name);
    printf("[INFO] Texurazott obj betoltese: %s \n", name);

    printf("[INFO] %s beolvasasa elkezdodott\n", filename);

    FILE* file = fopen(filename, "r");
    if(!file){
        printf("[HIBA] Nem sikerult beolvasni a kovetkezo fajlt: %s\n", filename);
        return;
    }
    char line[2200];
    int count = 0;

    while(fgets(line, sizeof(line), file)){
        if(line[0] == 'v' && line[1] == ' ') count++;
    }
    if(count == 0){
        printf("[HIBA] Nem talalhato pont a kovetkezo fajlban: %s\n", filename);
        fclose(file);
        return;
    }

    *vertices = (Vertex*)malloc((count)*sizeof(Vertex));
    (*vertices)->number_of_vertex= count;

    if(*vertices == NULL){
        printf("[HIBA] Memoriafoglalasi hiba\n");
        fclose(file);
        return;
    }

    rewind(file);
    int i = 0;
    while(fgets(line, sizeof(line), file)){
        if(line[0] == 'v' && line[1] == ' '){
            sscanf(line, "v %f %f %f", &(*vertices)[i].x, &(*vertices)[i].y, &(*vertices)[i].z);
            i++;
        }
    }
    fclose(file);
    printf("[SIKER] %s sikeresen beolvasva (v=%f)\n", filename, (*vertices)->number_of_vertex);
}

bool load_textured_obj(const char* name, Model* model){
    char filename[256];
    snprintf(filename, sizeof(filename), "%s%s", ASSETS_PREFIX, name);
    printf("[INFO] Texurazott obj betoltese: %s \n", name);

    FILE* file = fopen(filename, "r");
    if(!file){
        printf("[HIBA] Nem sikerult megnyitni: %s\n", filename);
        return false;
    }

    int v_count = 0, vt_count = 0, total_indices = 0, vn_count=0;
    char line[1024];
    while(fgets(line, sizeof(line), file)){
        if(line[0] == 'v' && line[1] == ' ') v_count++;
        else if(line[0] == 'v' && line[1] == 't') vt_count++;
        else if(line[0] == 'v' && line[1] == 'n') vn_count++;
        else if(line[0] == 'f'){
            int spaces = 0;
            for(int i=0; line[i] != '\0'; i++) if(line[i] == ' ') spaces++;

            if(spaces == 3) total_indices += 3;
            else if(spaces >= 4) total_indices += 6;
        }
    }

    float (*temp_v)[3] = malloc(sizeof(float[3]) * (v_count + 1));
    float (*temp_vt)[2] = (vt_count > 0) ? malloc(sizeof(float[2]) * (vt_count + 1)) : NULL;
    float (*temp_vn)[3] = malloc(sizeof(float[3]) * (vn_count + 1));

    if(!temp_v || (vt_count > 0 && !temp_vt)){
        printf("[HIBA] Memoriafoglalasi hiba az ideiglenes tomboknel!\n");
        if(temp_v) free(temp_v);
        fclose(file);
        return false;
    }

    rewind(file);
    int cv = 1, cvt = 1, cvn=1;
    while(fgets(line, sizeof(line), file)){
        if(line[0] == 'v' && line[1] == ' ') {
            sscanf(line, "v %f %f %f", &temp_v[cv][0], &temp_v[cv][1], &temp_v[cv][2]);
            cv++;
        }else if(line[0] == 'v' && line[1] == 't'){
            sscanf(line, "vt %f %f", &temp_vt[cvt][0], &temp_vt[cvt][1]);
            cvt++;
        }else if(line[0] == 'v' && line[1] == 'n') {
            sscanf(line, "vn %f %f %f", &temp_vn[cvn][0], &temp_vn[cvn][1], &temp_vn[cvn][2]);
            cvn++;
        }
    }

    model->vertices = malloc(sizeof(ModelVertex) * total_indices);
    model->number_of_vertex = total_indices;

    rewind(file);
    int current_v = 0;
    while(fgets(line, sizeof(line), file)){
        if(line[0] == 'f'){
            int v[4] = {0}, vt[4] = {0}, n[4] = {0};
            int matches = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                &v[0], &vt[0], &n[0], &v[1], &vt[1], &n[1], &v[2], &vt[2], &n[2], &v[3], &vt[3], &n[3]);

            int v_found = (matches >= 9) ? (matches / 3) : 0;
            if(v_found == 0){
                matches = sscanf(line, "f %d/%d %d/%d %d/%d %d/%d", &v[0], &vt[0], &v[1], &vt[1], &v[2], &vt[2], &v[3], &vt[3]);
                v_found = (matches >= 6) ? (matches / 2) : 0;
            }

            int loop_limit = (v_found == 4) ? 6 : 3;
            int order[] = {0, 1, 2, 0, 2, 3};

            for(int i = 0; i < loop_limit; i++){
                int idx = order[i];
                if(v[idx] > 0 && v[idx] <= v_count){
                    model->vertices[current_v].x = temp_v[v[idx]][0];
                    model->vertices[current_v].y = temp_v[v[idx]][1];
                    model->vertices[current_v].z = temp_v[v[idx]][2];
                }
                if(temp_vt && vt[idx] > 0 && vt[idx] <= vt_count){
                    model->vertices[current_v].u = temp_vt[vt[idx]][0];
                    model->vertices[current_v].v = 1.0f - temp_vt[vt[idx]][1];
                }

                if(n[idx] > 0 && n[idx] <= vn_count){
                    model->vertices[current_v].nx = temp_vn[n[idx]][0];
                    model->vertices[current_v].ny = temp_vn[n[idx]][1];
                    model->vertices[current_v].nz = temp_vn[n[idx]][2];
                }
                current_v++;
            }
        }
    }

    free(temp_v);
    if(temp_vt) free(temp_vt);
    fclose(file);
    return true;
}