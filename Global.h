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
#include <openssl/sha.h>
#include "dirent.h"
#include "zlib-1.3/zlib.h"
#define F_OK 0
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
typedef struct {
    char worktree[30];
    char gitdir[30];
    char config[30];
    /* -------------------------- worktree,gitdir,conf -------------------------- */
}GitRepository;

typedef struct{
    char fmt[20];//类型
    char *data;//数据
}Object;

typedef struct{
    Object object;
}GitBlob;

GitRepository init(char path[],bool force);


//set_repo_default_config:设置默认配置文件
void set_repo_default_config(GitRepository* repo);

void repo_create(GitRepository* repo,char path[]);

void serialize(Object* object,char blobdata[]);

void deserialize(Object* object,char data[]);