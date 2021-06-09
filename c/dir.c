#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nvidia_filter(const struct dirent *entry)
{
    return (strncmp(entry->d_name, "s", 1) == 0);
}

int main(void)
{
    struct dirent **namelist;
    struct dirent **namelist2;
    int n,m;
    char dir[PATH_MAX];

    n = scandir("/lib/modules/5.10.0-1029-oem/kernel/", &namelist, nvidia_filter, alphasort);
    if (n == -1) {
        perror("scandir");
        exit(EXIT_FAILURE);
    }

    while (n--) {
        printf("%s\n", namelist[n]->d_name);
	sprintf(dir, "/lib/modules/5.10.0-1029-oem/kernel/%s", namelist[n]->d_name);
        m = scandir(dir, &namelist2, nvidia_filter, alphasort);
        if (n == -1) {
           perror("scandir");
           exit(EXIT_FAILURE);
        }
        while (m--) {
           printf("%s\n", namelist2[m]->d_name);
	   free(namelist2[m]);
        }
	free(namelist2);
        free(namelist[n]);
    }
    free(namelist);

    exit(EXIT_SUCCESS);
}

