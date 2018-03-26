//
// Created by KelvinLew on 20/03/2018.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <QApplication>

#include "cjson.h"
#include "myhelper.h"


cJSON *JSONDATA;


void dofile(char *filename);/* Read a file, parse, render back, etc. */
void readJSON();
void saveJSON();
int getEventNum();
int getCandidateNum(int eventID,char *eventName);
char* getEventName(int eventID);
char* getEventPlace(int eventID, char *eventName);
int getEventID(char *eventName); // if not found, return -1
int getCandidateID(char *eventName, char *candidateName); // if not found, return -1
char* getCandidateName(char *eventName, int candidateID);
char* getCandidateCollege(char *eventName, int candidateID);
char* getCandidateScore(char *eventName, int candidateID);
int addEvent(char *eventName, char *eventPlace);
int addCandidate(QString eventName, QString candidateName, char *candidateCollege, char *candidateScore);
void deletEvent(int eventID, char *eventName);
void deletCandidate(char *eventName, char *candidateName);
int changeEventName(char *eventName, char *newEventName);
void changeEventPlace(char *eventName, char *newEventPlace);
int changeCandidateName(char *eventName, int candidateID, char *newCandidateName);
void changeCandidateCollege(char *eventName, int candidateID, char *newCandidateCollege);
void changeCandidateScore(char *eventName, int candidateID, char *newCandidateScore);




int getEventNum()
{
    int size;
    cJSON *arrayItem;
    arrayItem=cJSON_GetObjectItem(JSONDATA,"events");
    size=cJSON_GetArraySize(arrayItem);
    return size;
}

int getCandidateNum(int eventID,char *eventName)
{
    int size,i,retn=-1;
    cJSON *arrayItem,*object,*item;
    arrayItem=cJSON_GetObjectItem(JSONDATA,"events");
    if(eventID>=0){
        object=cJSON_GetArrayItem(arrayItem,eventID);
        item=cJSON_GetObjectItem(object,"candidate");
        retn=cJSON_GetArraySize(item);
    }else{
        size=cJSON_GetArraySize(arrayItem);
        for(i=0;i<size;i++){
            object=cJSON_GetArrayItem(arrayItem,i);
            item=cJSON_GetObjectItem(object,"event_name");
            if(strcmp(item->valuestring,eventName) == 0){
                arrayItem=cJSON_GetObjectItem(object,"candidate");
                retn=cJSON_GetArraySize(arrayItem);
                break;
            }
        }
    }

    return retn;
}

char* getEventName(int eventID) // eventID means the order of the events starting from 0
{
    cJSON *arrayItem,*object,*item;
    arrayItem=cJSON_GetObjectItem(JSONDATA,"events");
    object=cJSON_GetArrayItem(arrayItem,eventID);
    item=cJSON_GetObjectItem(object,"event_name");
    return item->valuestring;
}

char* getEventPlace(int eventID, char *eventName) // eventID means the order of the events starting from 0
{
    cJSON *arrayItem,*object,*item;
    arrayItem=cJSON_GetObjectItem(JSONDATA,"events");

    if(eventID >= 0){
        object=cJSON_GetArrayItem(arrayItem,eventID);
        item=cJSON_GetObjectItem(object,"event_place");
    } else {
        int size,i;
        size=cJSON_GetArraySize(arrayItem);
        for(i=0;i<size;i++){
            object=cJSON_GetArrayItem(arrayItem,i);
            item=cJSON_GetObjectItem(object,"event_name");
            if(strcmp(item->valuestring,eventName) == 0){
                item=cJSON_GetObjectItem(object,"event_time");
                break;
            }
        }
    }

    return item->valuestring;
}

int getEventID(char *eventName) // if not found, return -1
{
    cJSON *arrayItem,*object,*item;
    arrayItem=cJSON_GetObjectItem(JSONDATA,"events");


        int size,i;
        size=cJSON_GetArraySize(arrayItem);
        for(i=0;i<size;i++){
            object=cJSON_GetArrayItem(arrayItem,i);
            item=cJSON_GetObjectItem(object,"event_name");
            if(strcmp(item->valuestring,eventName) == 0){
                return i;
            }
        }


    return -1;
}

int getCandidateID(char *eventName, char *candidateName) // if not found, return -1
{
    int size,sizeCandidate,i,j;
    cJSON *arrayItem,*object,*item,*itemCandidate;
    arrayItem=cJSON_GetObjectItem(JSONDATA,"events");
    size=cJSON_GetArraySize(arrayItem);
    for(i=0;i<size;i++){
        object=cJSON_GetArrayItem(arrayItem,i);
        item=cJSON_GetObjectItem(object,"event_name");
        if(strcmp(item->valuestring,eventName) == 0){
            arrayItem=cJSON_GetObjectItem(object,"candidate");
            sizeCandidate=cJSON_GetArraySize(arrayItem);

            for(j=0;j<sizeCandidate;j++){
                itemCandidate=cJSON_GetArrayItem(arrayItem,j);
                itemCandidate=cJSON_GetObjectItem(itemCandidate,"name");
                if(strcmp(itemCandidate->valuestring,candidateName) == 0){
                    return j;
                }
            }

            break;
        }
    }

    return -1;
}


char* getCandidateName(int eventID, int candidateID)
{
    cJSON *arrayItem,*object,*item;
    arrayItem=cJSON_GetObjectItem(JSONDATA,"events");
    object=cJSON_GetArrayItem(arrayItem,eventID);
    item=cJSON_GetObjectItem(object,"candidate");
    item=cJSON_GetArrayItem(item,candidateID);
    item=cJSON_GetObjectItem(item,"name");
    return item->valuestring;
}

char* getCandidateCollege(int eventID, int candidateID)
{
    cJSON *arrayItem,*object,*item;
    arrayItem=cJSON_GetObjectItem(JSONDATA,"events");
    object=cJSON_GetArrayItem(arrayItem,eventID);
    item=cJSON_GetObjectItem(object,"candidate");
    item=cJSON_GetArrayItem(item,candidateID);
    item=cJSON_GetObjectItem(item,"college");
    return item->valuestring;
}

char* getCandidateScore(int eventID, int candidateID)
{
    cJSON *arrayItem,*object,*item;
    arrayItem=cJSON_GetObjectItem(JSONDATA,"events");
    object=cJSON_GetArrayItem(arrayItem,eventID);
    item=cJSON_GetObjectItem(object,"candidate");
    item=cJSON_GetArrayItem(item,candidateID);
    item=cJSON_GetObjectItem(item,"score");
    return item->valuestring;
}

//void getCandidate(char *eventName, int candidateID, char *candidateName, char *candidateCollege, char *candidateScore)
//{
//    int size,i, isGet = 0;
//    cJSON *arrayItem,*object,*item,*itemCandidate;
//    char *tmp;
//    tmp=(char *)malloc(100);
//    arrayItem=cJSON_GetObjectItem(JSONDATA,"events");
//    size=cJSON_GetArraySize(arrayItem);
//    for(i=0;i<size;i++){
//        object=cJSON_GetArrayItem(arrayItem,i);
//        item=cJSON_GetObjectItem(object,"event_name");
//        if(strcmp(item->valuestring,eventName) == 0){
//            arrayItem=cJSON_GetObjectItem(object,"candidate");
//            arrayItem=cJSON_GetArrayItem(arrayItem,candidateID);
//            itemCandidate=cJSON_GetObjectItem(arrayItem,"name");
//            tmp=itemCandidate->valuestring;
//            candidateName=tmp;
//            itemCandidate=cJSON_GetObjectItem(arrayItem,"college");
//            candidateCollege=itemCandidate->valuestring;
//            itemCandidate=cJSON_GetObjectItem(arrayItem,"score");
//            candidateScore=itemCandidate->valuestring;
//            isGet=1;
//        }
//        if(isGet==1) break;
//    }
//
//}

int addEvent(char *eventName, char *eventPlace)
{
    if(getEventID(eventName)!=-1){
        return -1;
    }else{
        cJSON *arrayItem,*object;
        arrayItem=cJSON_GetObjectItem(JSONDATA,"events");
        cJSON_AddItemToArray(arrayItem,object=cJSON_CreateObject());
        cJSON_AddStringToObject(object,"event_name",eventName);
        cJSON_AddStringToObject(object,"event_place",eventPlace);
        saveJSON();
    }
    return 0;
}

int addCandidate(QString eventName, QString candidateName, char *candidateCollege, char *candidateScore)
{
    int size,i,retn=0;
    cJSON *arrayItem,*object,*item,*array;
    if(getCandidateID(eventName.toLatin1().data(),candidateName.toLatin1().data())==-1){
        arrayItem=cJSON_GetObjectItem(JSONDATA,"events");
        size=cJSON_GetArraySize(arrayItem);
        for(i=0;i<size;i++){
            object=cJSON_GetArrayItem(arrayItem,i);
            item=cJSON_GetObjectItem(object,"event_name");
            if(strcmp(item->valuestring,eventName.toLatin1().data()) == 0){
                if(cJSON_HasObjectItem(object,"candidate")){
                    arrayItem=cJSON_GetObjectItem(object,"candidate");
                    cJSON_AddItemToArray(arrayItem,object=cJSON_CreateObject());
                    cJSON_AddStringToObject(object,"name",candidateName.toLatin1().data());
                    cJSON_AddStringToObject(object,"college",candidateCollege);
                    cJSON_AddStringToObject(object,"score",candidateScore);
                    saveJSON();
                    retn=0;
                    break;
                }else{
                    cJSON_AddItemToObject(object,"candidate",array=cJSON_CreateArray());
                    cJSON_AddItemToArray(array,object=cJSON_CreateObject());
                    cJSON_AddStringToObject(object,"name",candidateName.toLatin1().data());
                    cJSON_AddStringToObject(object,"college",candidateCollege);
                    cJSON_AddStringToObject(object,"score",candidateScore);
                    saveJSON();
                    retn=0;
                    break;
                }

            }
        }
    }else{
        retn=-1;
    }
    return retn;
}


void deletEvent(int eventID, char *eventName) // eventID means the order of the events starting from 0
{
    cJSON *arrayItem,*object,*item;
    arrayItem=cJSON_GetObjectItem(JSONDATA,"events");

    if(eventID >= 0){
        cJSON_DeleteItemFromArray(arrayItem,eventID);
    } else {
        int size,i;
        size=cJSON_GetArraySize(arrayItem);
        for(i=0;i<size;i++){
            object=cJSON_GetArrayItem(arrayItem,i);
            item=cJSON_GetObjectItem(object,"event_name");
            if(strcmp(item->valuestring,eventName) == 0){
                cJSON_DeleteItemFromArray(arrayItem,i);
                saveJSON();
                break;
            }
        }
    }

}


void deletCandidate(char *eventName, char *candidateName)
{
    int size,sizeCandidate,i,j, isDelet = 0;
    cJSON *arrayItem,*object,*item,*array;
    arrayItem=cJSON_GetObjectItem(JSONDATA,"events");
    size=cJSON_GetArraySize(arrayItem);
    for(i=0;i<size;i++){
        object=cJSON_GetArrayItem(arrayItem,i);
        item=cJSON_GetObjectItem(object,"event_name");
        if(strcmp(item->valuestring,eventName) == 0){
            if(cJSON_HasObjectItem(object,"candidate")){
                arrayItem=cJSON_GetObjectItem(object,"candidate");
                sizeCandidate=cJSON_GetArraySize(arrayItem);
                for(j=0;j<sizeCandidate;j++){
                    array=cJSON_GetArrayItem(arrayItem,j);
                    array=cJSON_GetObjectItem(array,"name");
                    if(strcmp(array->valuestring,candidateName) == 0){
                        cJSON_DeleteItemFromArray(arrayItem,j);
                        saveJSON();
                        isDelet=1;
                    }
                    if(isDelet==1) break;
                }



            }else{
                isDelet=1;
            }
        }
        if(isDelet == 1) break;
    }

}

int changeEventName(char *eventName, char *newEventName)
{
    if(getEventID(newEventName)!=-1){
        return -1;
    }else{
        cJSON *arrayItem,*object,*item;
        arrayItem=cJSON_GetObjectItem(JSONDATA,"events");

        int size,i;
        size=cJSON_GetArraySize(arrayItem);
        for(i=0;i<size;i++){
            object=cJSON_GetArrayItem(arrayItem,i);
            item=cJSON_GetObjectItem(object,"event_name");
            if(strcmp(item->valuestring,eventName) == 0){
                cJSON_ReplaceItemInObject(object,"event_name",cJSON_CreateString(newEventName));
                saveJSON();
                break;
            }
        }
    }
    return 0;
}

void changeEventPlace(char *eventName, char *newEventPlace)
{
    cJSON *arrayItem,*object,*item;
    arrayItem=cJSON_GetObjectItem(JSONDATA,"events");

    int size,i;
    size=cJSON_GetArraySize(arrayItem);
    for(i=0;i<size;i++){
        object=cJSON_GetArrayItem(arrayItem,i);
        item=cJSON_GetObjectItem(object,"event_name");
        if(strcmp(item->valuestring,eventName) == 0){
            cJSON_ReplaceItemInObject(object,"event_place",cJSON_CreateString(newEventPlace));
            saveJSON();
            break;
        }
    }

}

int changeCandidateName(char *eventName, int candidateID, char *newCandidateName)
{
    if(getCandidateID(eventName,newCandidateName)!=-1){
        return -1;
    }else{
        int size,i;
        cJSON *arrayItem,*object,*item;
        arrayItem=cJSON_GetObjectItem(JSONDATA,"events");
        size=cJSON_GetArraySize(arrayItem);
        for(i=0;i<size;i++){
            object=cJSON_GetArrayItem(arrayItem,i);
            item=cJSON_GetObjectItem(object,"event_name");
            if(strcmp(item->valuestring,eventName) == 0){
                arrayItem=cJSON_GetObjectItem(object,"candidate");
                arrayItem=cJSON_GetArrayItem(arrayItem,candidateID);
                cJSON_ReplaceItemInObject(arrayItem,"name",cJSON_CreateString(newCandidateName));
                saveJSON();
                break;
            }
        }
    }

    return 0;
}

void changeCandidateCollege(char *eventName, int candidateID, char *newCandidateCollege)
{
    int size,i;
    cJSON *arrayItem,*object,*item;
    arrayItem=cJSON_GetObjectItem(JSONDATA,"events");
    size=cJSON_GetArraySize(arrayItem);
    for(i=0;i<size;i++){
        object=cJSON_GetArrayItem(arrayItem,i);
        item=cJSON_GetObjectItem(object,"event_name");
        if(strcmp(item->valuestring,eventName) == 0){
            arrayItem=cJSON_GetObjectItem(object,"candidate");
            arrayItem=cJSON_GetArrayItem(arrayItem,candidateID);
            cJSON_ReplaceItemInObject(arrayItem,"college",cJSON_CreateString(newCandidateCollege));
            saveJSON();
            break;
        }
    }
}

void changeCandidateScore(char *eventName, int candidateID, char *newCandidateScore)
{
    int size,i;
    cJSON *arrayItem,*object,*item;
    arrayItem=cJSON_GetObjectItem(JSONDATA,"events");
    size=cJSON_GetArraySize(arrayItem);
    for(i=0;i<size;i++){
        object=cJSON_GetArrayItem(arrayItem,i);
        item=cJSON_GetObjectItem(object,"event_name");
        if(strcmp(item->valuestring,eventName) == 0){
            arrayItem=cJSON_GetObjectItem(object,"candidate");
            arrayItem=cJSON_GetArrayItem(arrayItem,candidateID);
            cJSON_ReplaceItemInObject(arrayItem,"score",cJSON_CreateString(newCandidateScore));
            saveJSON();
            break;
        }

    }

}


// Read a file, parse, render back, etc.
void dofile(char* filename)
{
    FILE *f;
    long len;
    char *data;


    f=fopen(filename,"r");
    fseek(f,0,SEEK_END);
    len=ftell(f);
    fseek(f,0,SEEK_SET);
    data=(char*)malloc(len+1);
    fread(data,1,len,f);
    fclose(f);

    printf("read file %s complete, len=%ld\n",filename,len);

    JSONDATA=cJSON_Parse(data);


    if (!JSONDATA)
    {
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());
    }
    free(data);
}


void readJSON()
{

    QFile file(QString(qApp->applicationDirPath()+"/data.json"));
    if(file.exists()){
        dofile((QString(qApp->applicationDirPath()+"/data.json")).toLatin1().data());
    }else{
        QFile inputFile(QString(":/data.json"));
        inputFile.open(QIODevice::ReadOnly);
        QTextStream in(&inputFile);
        QString line = in.readAll();
        qDebug() <<line;

        QFile outputFile(QString(qApp->applicationDirPath()+"/data.json"));
        outputFile.open(QIODevice::ReadWrite|QIODevice::Text);
        QTextStream out(&outputFile);
        out<<line;
        inputFile.close();
        outputFile.close();

        dofile((QString(qApp->applicationDirPath()+"/data.json")).toLatin1().data());
    }

}

void saveJSON()
{
    //将json结构格式化到缓冲区
    char *buf = cJSON_Print(JSONDATA);
    printf("%s",buf);
    //打开文件写入json内容
    FILE *fp = fopen((qApp->applicationDirPath()+"/data.json").toLatin1().data(),"w");
    fwrite(buf,strlen(buf),1,fp);
    free(buf);
    fclose(fp);
    //释放json结构所占用的内存
    //cJSON_Delete(JSONDATA);
}
