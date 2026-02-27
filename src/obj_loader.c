//
// Created by asgyu on 2026. febr. 25..
//

#include "obj_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_obj(const char* filename, Vertex** vertices){
    printf("[INFO] %s beolvasasa elkezdodott\n", filename);

    FILE* file = fopen(filename, "r");
    if(!file){
        printf("[HIBA] Nem sikerult beolvasni a kovetkezo fajlt: %s\n", filename);
        return;
    }
    char line[1024];
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
    printf("[SIKER] %s sikeresen beolvasva\n", filename);
}

bool load_textured_obj(const char* filename, Model* model){
    printf("[INFO] %s beolvasasa elkezdodott\n", filename);

    FILE* file = fopen(filename, "r");
    if(!file){
        printf("[HIBA] Nem sikerult beolvasni a kovetkezo fajlt: %s\n", filename);
        return false;
    }

    int v_count = 0, vt_count=0, total_indices = 0;
    char line[1024];
    while(fgets(line, sizeof(line), file)){
        if(line[0] == 'v' && line[1] == ' ') v_count++;
        else if(line[0] == 'v' && line[1] == 't') vt_count++;
        else if(line[0] == 'f'){
            int spaces = 0;
            for(int i=0; line[i] != '\0'; i++){
                if(line[i] == ' ') spaces++;
            }

            int vertices_in_face = 0;
            char* ptr = line + 2;
            while(*ptr != '\0' && *ptr != '\n'){
                while(*ptr == ' ') ptr++;
                if(*ptr == '\0' || *ptr == '\n') break;
                vertices_in_face++;
                while(*ptr != ' ' && *ptr != '\0' && *ptr != '\n') ptr++;
            }

            if(vertices_in_face == 3) total_indices += 3;
            else if(vertices_in_face == 4) total_indices += 6;
        }
    }

    rewind(file);

    if(v_count == 0){
        printf("[HIBA] Nincsenek vertexek a fajlban: %s\n", filename);
        fclose(file);
        return false;
    }

    float (*temp_v)[3] = malloc(sizeof(float[3]) * v_count);
    if(!temp_v){
        printf("[HIBA] Memoriafoglalasi hiba (temp_v)\n");
        fclose(file);
        return false;
    }

    float (*temp_vt)[2] = NULL;
    if(vt_count > 0){
        temp_vt = malloc(sizeof(float[2]) * vt_count);
        if(!temp_vt){
            printf("[HIBA] Memoriafoglalasi hiba (temp_vt)\n");
            free(temp_v);
            fclose(file);
            return false;
        }
    }

    int current_v_temp = 0;
    int current_vt_temp = 0;
    while(fgets(line, sizeof(line), file)){
        if(line[0] == 'v' && line[1] == ' '){
            sscanf(line, "v %f %f %f", &temp_v[current_v_temp][0], &temp_v[current_v_temp][1], &temp_v[current_v_temp][2]);
            current_v_temp++;
        }else if(line[0] == 'v' && line[1] == 't'){
            sscanf(line, "vt %f %f", &temp_vt[current_vt_temp][0], &temp_vt[current_vt_temp][1]);
            current_vt_temp++;
        }
    }

    model->vertices = malloc(sizeof(ModelVertex) * total_indices);
    if(!model->vertices){
        printf("[HIBA] Memoriafoglalasi hiba (model->vertices)\n");
        free(temp_v);
        if(temp_vt) free(temp_vt);
        fclose(file);
        return false;
    }
    model->number_of_vertex = total_indices;

    rewind(file);
    int current_v = 0;
    while(fgets(line, sizeof(line), file)){
        if(line[0] == 'f'){
            int v_index[4] ={0}, vt_index[4] ={0}, n_index[4] ={0};
            int matches = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                                 &v_index[0], &vt_index[0], &n_index[0],
                                 &v_index[1], &vt_index[1], &n_index[1],
                                 &v_index[2], &vt_index[2], &n_index[2],
                                 &v_index[3], &vt_index[3], &n_index[3]);

            int vertices_found = 0;
            if(matches == 12) vertices_found = 4;
            else if(matches == 9) vertices_found = 3;
            else{
                 matches = sscanf(line, "f %d//%d %d//%d %d//%d %d//%d",
                                 &v_index[0], &n_index[0],
                                 &v_index[1], &n_index[1],
                                 &v_index[2], &n_index[2],
                                 &v_index[3], &n_index[3]);
                if(matches == 8) vertices_found = 4;
                else if(matches == 6) vertices_found = 3;
                else{
                    matches = sscanf(line, "f %d/%d %d/%d %d/%d %d/%d",
                                      &v_index[0], &vt_index[0],
                                      &v_index[1], &vt_index[1],
                                      &v_index[2], &vt_index[2],
                                      &v_index[3], &vt_index[3]);
                    if(matches == 8) vertices_found = 4;
                    else if(matches == 6) vertices_found = 3;
                    else{
                         matches = sscanf(line, "f %d %d %d %d",&v_index[0], &v_index[1], &v_index[2], &v_index[3]);
                         if(matches == 4) vertices_found = 4;
                         else if(matches == 3) vertices_found = 3;
                    }
                }
            }

            int indices_to_process[6] ={0, 1, 2, -1, -1, -1};
            if(vertices_found == 4){
                indices_to_process[3] = 0;
                indices_to_process[4] = 2;
                indices_to_process[5] = 3;
            }

            for(int k = 0; k < 6; k++){
                int index = indices_to_process[k];
                if(index == -1) break;

                if(v_index[index] > 0 && v_index[index] <= v_count){
                    model->vertices[current_v].x = temp_v[v_index[index]-1][0];
                    model->vertices[current_v].y = temp_v[v_index[index]-1][1];
                    model->vertices[current_v].z = temp_v[v_index[index]-1][2];
                }else{
                    model->vertices[current_v].x = 0;
                    model->vertices[current_v].y = 0;
                    model->vertices[current_v].z = 0;
                }

                if(vt_count > 0 && vt_index[index] > 0 && vt_index[index] <= vt_count){
                    model->vertices[current_v].u = temp_vt[vt_index[index]-1][0];
                    model->vertices[current_v].v = temp_vt[vt_index[index]-1][1];
                }else{
                    model->vertices[current_v].u = 0.0f;
                    model->vertices[current_v].v = 0.0f;
                }

                current_v++;
                if(current_v >= total_indices) break;
            }
        }
    }

    free(temp_v);
    if(temp_vt) free(temp_vt);
    fclose(file);
    printf("[SIKER] %s sikeresen beolvasva (Vertexek: %d)\n", filename, current_v);

    model->number_of_vertex = current_v;

    return true;
}