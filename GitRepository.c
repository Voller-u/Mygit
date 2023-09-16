#include "Global.h"
GitRepository init(char path[],bool force){
    GitRepository gitRepository;
    strcpy(gitRepository.worktree,path);

    strcpy(gitRepository.gitdir,path);
    strcat(gitRepository.gitdir,"/.git");

    strcpy(gitRepository.config,gitRepository.gitdir);
    strcat(gitRepository.config,"/config");
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

void to_repo_path(char repoPath[],GitRepository repo,char path[]){
    printf("repo.gitdir: %s\npath:%s\n",repo.gitdir,path);
    memcpy(repoPath,repo.gitdir,sizeof(repo.gitdir));
    strcat(repoPath,"/");
    strcat(repoPath,path);
    mkdirs(repoPath);
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
    FILE* config_fp = NULL;
    printf("config: %s\n",repo->config);
    config_fp = fopen(repo->config,"w+");
    if(config_fp == NULL){
        printf("配置文件打开失败");
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
    //避免把最后的路径也变成文件夹
    /*if( len>0 && access(str,0)!=0 )
    {
        mkdir( str);
    }*/
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

void repo_create(GitRepository* repo,char path[]){
    *repo = init(path,true);
    if(exists((*repo).worktree)){
        if(!isFolder((*repo).worktree)){
            printf("%s is not a directory!" ,path);
            return ;
        }
        if(exists((*repo).gitdir) && isEmptyFolder((*repo).gitdir)){
            printf("%s is not empty!" , path);
            return;
        }
    }
    else{
        mkdirs((*repo).worktree);
    }
    //TODO assert

    //写.git/description文件
    FILE* des_fp = NULL;
    char des_path[30];
    to_repo_path(des_path,(*repo),"description");
    printf("%s\n",des_path);
    des_fp = fopen(des_path,"w+");
    if(des_fp == NULL){
        printf("ERROR:description file open failed.");
        return;
    }
    fprintf(des_fp,"Unnamed repository; edit this file 'description' to name the repository.\n");
    fclose(des_fp);

    //写./git/HEAD文件
    FILE* head_fp = NULL;
    char head_path[30];
    to_repo_path(head_path,(*repo),"HEAD");
    printf("%s\n",head_path);
    head_fp = fopen(head_path,"w+");
    if(head_fp == NULL){
        printf("ERROR:HEAD file open failed.");
        return;
    }
    fprintf(head_fp,"ref: refs/heads/master\n");
    fclose(head_fp);

    //写./git/config文件
    FILE* conf_fp = NULL;
    char conf_path[30];
    to_repo_path(conf_path,(*repo),"config");
    printf("%s\n",conf_path);
    conf_fp = fopen(conf_path,"w+");
    if(conf_fp == NULL){
        printf("ERROR:config file open failed.");
        return;
    }
    printf("repo->config %s\n",repo->config);
    set_repo_default_config(repo);
    fclose(conf_fp);
}

//将路径转为系统绝对路径
void fullPath(char s[MAX_PATH]){
    DWORD len = GetFullPathName(".",MAX_PATH,s,NULL);

    if(len == 0){
        printf("GetFullPathName failed: %d\n",GetLastError());
        return;
    }
}

void repo_find(char real_path[MAX_PATH]){
    fullPath(real_path);
    printf("%s",real_path);
    char real_path_plus_git[MAX_PATH];
    memcpy(real_path_plus_git,real_path,sizeof real_path);
    strcat(real_path_plus_git,"/.git");
    if(isFolder(real_path_plus_git))
        return;
    //如果没有返回，说明.git应该在父级或更高的目录中，继续递归
    char parent[MAX_PATH];
    memcpy(parent,real_path_plus_git,sizeof real_path_plus_git);
    strcat(parent,"../");
    fullPath(parent);
    if(strcmp(parent,real_path)==0){
        //若父目录与当前目录相同，说明当前已经是根目录并且没有.git文件夹
        printf("No git directory.");
        return;
    }

    //否则继续递归

    repo_find(parent);

}