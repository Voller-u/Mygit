#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <io.h>
#include <assert.h>
#include <direct.h>
#include <stdlib.h>
#include <sys/types.h>
#include "dirent.h"
#define F_OK 0
#define TRUE 0
struct GitRepository
{
    char* worktree;
    char* gitdir;
    char* config;
    /* -------------------------- worktree,gitdir,conf -------------------------- */
};

GitRepository init(char path[],bool force = false);

char* repo_path(GitRepository repo,char path[]);

char* repo_file(GitRepository repo,char path[],bool mkdir = false);

char* repo_dir(GitRepository repo,char path[],bool mkdir = false);