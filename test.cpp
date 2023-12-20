#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
void ListFilesRecursively(const char *basePath);
void ListFilesRecursively1(const char *basePath) {
    char searchPath[MAX_PATH];
    sprintf(searchPath, "%s\\*.*", basePath);

    _finddata_t data;
    long handle = _findfirst(searchPath, &data);

    if (handle == -1) {
        return;
    }

    do {
        if (strcmp(data.name,".git") != 0 && strcmp(data.name, "..") != 0) {
            printf("%s\\%s\n", basePath, data.name);

            if (data.attrib & _A_SUBDIR) {
                char subPath[MAX_PATH];
                sprintf(subPath, "%s\\%s", basePath, data.name);
                ListFilesRecursively(subPath);
            }
        }
    } while (_findnext(handle, &data) != -1);

    _findclose(handle);
}

void ListFilesRecursively(const char *basePath) {
    char searchPath[MAX_PATH];
    sprintf(searchPath, "%s\\*.*", basePath);

    _finddata_t data;
    long handle = _findfirst(searchPath, &data);

    if (handle == -1) {
        return;
    }

    do {
        if (strcmp(data.name,".git") != 0 && strcmp(data.name, ".") != 0 && strcmp(data.name, "..") != 0) {
            printf("%s\\%s\n", basePath, data.name);

            if (data.attrib & _A_SUBDIR) {
                char subPath[MAX_PATH];
                sprintf(subPath, "%s\\%s", basePath, data.name);
                ListFilesRecursively(subPath);
            }
        }
    } while (_findnext(handle, &data) != -1);

    _findclose(handle);
}

int main() {
    char currentDir[MAX_PATH];
    if (!_getcwd(currentDir, sizeof(currentDir))) {
        perror("Error getting current directory");
        return 1;
    }

    printf("Listing files in the current directory:\n");
    ListFilesRecursively1(currentDir);

    return 0;
}
