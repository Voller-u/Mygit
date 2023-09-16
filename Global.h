#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <io.h>
#include <assert.h>
#include <direct.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <limits.h>
#include <windows.h>
#include "dirent.h"
#define F_OK 0
typedef struct GitRepository
{
    char worktree[30];
    char gitdir[30];
    char config[30];
    /* -------------------------- worktree,gitdir,conf -------------------------- */
}GitRepository;

GitRepository init(char path[],bool force);


//set_repo_default_config:设置默认配置文件
void set_repo_default_config(GitRepository* repo);

void repo_create(GitRepository* repo,char path[]);