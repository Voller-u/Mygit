#include "Global.h"
GitRepository init(char path[],bool force){
    force = false;
    GitRepository gitRepository;
    gitRepository.worktree = path;
    gitRepository.gitdir = path;
    strcat(gitRepository.gitdir,"/.git");

    // if (!(force || isFolder(gitRepository.gitdir))){
    //     printf("ERROR:Not a Git repository %s",path);
    //     return;
    // }

    /* ---------------------------- 读取.git/config配置文件 --------------------------- */
    // char* cf = repo_file( gitRepository,"config",false);
    // if(cf && access(cf,F_OK)==0){

    // }
    // else if (!force){
    //     printf("Configuration file missing");
    //     return;
    // }

    // if(!force){
    //     //TODO 读取版本号
        
    // }
    return gitRepository;
}

void to_repo_path(char** repoPath,GitRepository repo,char path[]){
    memcpy(*repoPath,repo.gitdir,sizeof repoPath);
    strcat(*repoPath,path);
}
bool exists(char* path){
    struct stat buffer;
    int exist = stat(path,&buffer);
    if(exist == 0)
        return true;
    else  
        return false;
}

void set_repo_default_config(GitRepository* repo){
    char* config_path = (*repo).config;
    FILE* config_fp = NULL;
    config_fp = fopen(config_path,"w+");
    if(config_fp == NULL){
        assert("配置文件打开失败");
    }
    else{
        fprintf(config_fp,"[core]\n");
        fprintf(config_fp,"\trepositoryformatversion = 0\n");
        fprintf(config_fp,"\tfilemode = false\n");
        fprintf(config_fp,"\tbare = false\n");
    }
}

bool isFolder(char* path){
    struct stat buf;
    int res = stat(path,&buf);
    if(S_IFDIR & buf.st_mode) return true;
    return false; 
}

//创建多级目录
void mkdirs(char *muldir) 
{
    int i,len;
    char str[512];    
    strncpy(str, muldir, 512);
    len=strlen(str);
    for( i=0; i<len; i++ )
    {
        if( str[i]=='/' )
        {
            str[i] = '\0';
            if( access(str,0)!=0 )
            {
                mkdir( str);
            }
            str[i]='/';
        }
    }
    if( len>0 && access(str,0)!=0 )
    {
        mkdir( str);
    }
    return;
}

bool isEmptyFolder(char* path){
    DIR * dir;
    struct dirent * ptr;
    int i;
    dir = opendir (path);
    if ((ptr = readdir (dir)) != NULL) {
        return false;
    }
    closedir (dir);
    return true;
}

GitRepository repo_create(char path[]){
    GitRepository repo = init(path,true);
    if(exists(repo.worktree)){
        if(!isFolder(repo.worktree)){
            printf("%s is not a directory!" ,path);
            return ;
        }
        if(exists(repo.gitdir) && isEmptyFolder(repo.gitdir)){
            printf("%s is not empty!" , path);
            return;
        }
    }
    else{
        mkdirs(repo.worktree);
    }

    //TODO assert

    //写.git/description文件
    FILE* des_fp = NULL;
    char des_path[30];
    to_repo_path(&des_path,repo,"description");
    printf("%s",des_path);
    des_fp = fopen(des_path,"w+");
    if(des_fp == NULL){
        //printf("%s",repo_file(repo,"description",false));
        printf("ERROR:description file open failed.");
        return;
    }
    printf("description文件即将书写");
    fprintf(des_fp,"Unnamed repository; edit this file 'description' to name the repository.\n");

    //写./git/HEAD文件
    FILE* head_fp = NULL;
    char head_path[30];
    to_repo_path(&head_path,repo,"HEAD");
    head_fp = fopen(head_path,"w+");
    if(head_fp == NULL){
        printf("ERROR:HEAD file open failed.");
        return;
    }
    fprintf(head_fp,"ref: refs/heads/master\n");
    //写./git/config文件
    FILE* conf_fp = NULL;
    char conf_path[30];
    to_repo_path(&conf_path,repo,"config");
    conf_fp = fopen(conf_path,"w+");
    if(conf_fp == NULL){
        printf("ERROR:config file open failed.");
        return;
    }
    set_repo_default_config(repo.config);

    return repo;
}