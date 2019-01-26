#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
void tree(char *basePath, const int root)
{
    int i;
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir)
        return;
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            for (i=0; i<root; i++)
            {
                if (i%2 == 0 || i == 0)
                    printf("%c", '|');
                else
                    printf("  ");

            }
            printf("%c%c%c%s\n",'|', '-','-', dp->d_name);
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            tree(path, root + 2);
        }
    }
    closedir(dir);
}
int main()
{
    char path[100];
    printf("Enter path to list files: ");
    scanf("%s", path);
    tree(path, 0);
    return 0;
}



