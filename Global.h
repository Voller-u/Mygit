#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <io.h>
#include <assert.h>
#include <direct.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include "dirent.h"
#define F_OK 0
typedef struct GitRepository
{
    char* worktree;
    char* gitdir;
    char* config;
    /* -------------------------- worktree,gitdir,conf -------------------------- */
}GitRepository;

GitRepository init(char path[],bool force);

// char* repo_path(char** repoPath,GitRepository repo,char path[]);

// char* repo_file(GitRepository repo,char path[],bool mkdir);

char* repo_dir(GitRepository repo,char path[],bool mkdir);

//set_repo_default_config:设置默认配置文件
void set_repo_default_config(GitRepository* repo);

GitRepository repo_create(char path[]);