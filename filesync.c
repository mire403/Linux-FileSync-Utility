#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void copy_file(const char *src, const char *dst) {
    FILE *fs = fopen(src, "rb");
    if (!fs) return;
    FILE *fd = fopen(dst, "wb");
    if (!fd) {
        fclose(fs);
        return;
    }
    char buf[4096];
    int n;
    while ((n = fread(buf, 1, 4096, fs)) > 0) {
        fwrite(buf, 1, n, fd);
    }
    fclose(fs);
    fclose(fd);
}

int file_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0;
}

time_t get_ctime(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return st.st_ctime;
}

void join_path(char *out, const char *dir, const char *file) {
    strcpy(out, dir);
    int len = strlen(out);
    if (out[len - 1] != '/') {
        out[len] = '/';
        out[len + 1] = '\0';
    }
    strcat(out, file);
}

int main() {
    char DS[256], DD[256];
    printf("Enter source folder: ");
    scanf("%s", DS);
    printf("Enter destination folder: ");
    scanf("%s", DD);

    DIR *dp = opendir(DS);
    if (!dp) {
        printf("Cannot open source directory.\n");
        return 1;
    }

    FILE *log = fopen("added_files.txt", "w");

    struct dirent *ent;
    while ((ent = readdir(dp)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;

        char src_path[512], dst_path[512];
        join_path(src_path, DS, ent->d_name);
        join_path(dst_path, DD, ent->d_name);

        struct stat st_src;
        stat(src_path, &st_src);
        if (S_ISDIR(st_src.st_mode)) continue;

        if (!file_exists(dst_path)) {
            copy_file(src_path, dst_path);
            fprintf(log, "%s\n", src_path);
            continue;
        }

        time_t ctime_src = get_ctime(src_path);
        time_t ctime_dst = get_ctime(dst_path);

        if (ctime_src < ctime_dst) {
            printf("\nFile: %s\n", ent->d_name);
            printf("Source ctime: %ld\n", ctime_src);
            printf("Target ctime: %ld\n", ctime_dst);
            printf("Overwrite? (y/n): ");
            char op;
            scanf(" %c", &op);
            if (op == 'y' || op == 'Y') {
                copy_file(src_path, dst_path);
            }
        } else {
            copy_file(src_path, dst_path);
        }
    }

    fclose(log);
    closedir(dp);
    return 0;
}
