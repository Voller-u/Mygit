#include "Global.h"
void serialize(Object* object,char blobdata[]){
    strcpy(object->data,blobdata,sizeof(blobdata));
}

void deserialize(Object* object,char data[]){
    strcpy(object->data,data,sizeof data);
}