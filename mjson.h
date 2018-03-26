#ifndef MJSON_H
#define MJSON_H



void dofile(char *filename);/* Read a file, parse, render back, etc. */
void readJSON();
void saveJSON();
int getEventNum();
int getCandidateNum(int eventID,char *eventName);
char* getEventName(int eventID);
char* getEventPlace(int eventID, char *eventName);
int getEventID(char *eventName); // if not found, return -1
int getCandidateID(char *eventName, char *candidateName); // if not found, return -1
char* getCandidateName(int eventID, int candidateID);
char* getCandidateCollege(int eventID, int candidateID);
char* getCandidateScore(int eventID, int candidateID);
int addEvent(char *eventName, char *eventPlace);
int addCandidate(QString eventName, QString candidateName, char *candidateCollege, char *candidateScore);
void deletEvent(int eventID, char *eventName);
void deletCandidate(char *eventName, char *candidateName);
int changeEventName(char *eventName, char *newEventName);
void changeEventPlace(char *eventName, char *newEventPlace);
int changeCandidateName(char *eventName, int candidateID, char *newCandidateName);
void changeCandidateCollege(char *eventName, int candidateID, char *newCandidateCollege);
void changeCandidateScore(char *eventName, int candidateID, char *newCandidateScore);


#endif // MJSON_H
