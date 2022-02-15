#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

int ifAvi(const char * filePath);
int getSize(const char * filePath);	
int ifSame(const char * firsFilePath, const char * secondFilePath);
void aviCount(const char * dir,  int * aviAmount);
void getAviPathes(const char * dir,  char ** aviPathes, int * num);


//C:/Users/m video/Desktop/videos
int main(){
	char startDir[PATH_MAX+1], finishDir[PATH_MAX+1];
	
	gets(startDir);
	gets(finishDir);

	strcat(finishDir, "/");
    strcat(finishDir, "avis");

    mkdir(finishDir);

    int aviAmount = 0;
	aviCount(startDir, &aviAmount);

	char *aviPathes[aviAmount];
	for(int i = 0; i < aviAmount; i++)
		aviPathes[i] = (char*)malloc(sizeof(char)*(PATH_MAX + 1));
	int num = 0;
	getAviPathes(startDir, aviPathes, &num);

	printf("-------------------------\n");

	//for(int i = 0; i < aviAmount; i++)
		//printf("%s\n", aviPathes[i]);

	int count = 1;
	for(int i = 0; i < aviAmount - 1; i++){
		for(int j = i + 1; j < aviAmount; j++){
			if(ifSame(aviPathes[i], aviPathes[j]) != 1) continue;
			char newPath[PATH_MAX + 1];
			char number[5];
			strcpy(newPath, finishDir);
			strcat(newPath, "/map_");
			if(count > 9 && count < 100) strcat(newPath, "0");
			if(count < 10) strcat(newPath, "00");
			strcat(newPath, itoa(count, number, 10));
			strcat(newPath, ".avi");
			printf("OLDPATH: %s\nNEWPATH: %s\n", aviPathes[i], newPath);
			rename(aviPathes[i], newPath);
    		count++;
		}
	}

	return 0;
}
//C:/Users/m video/Desktop/videos
int getSize(const char * filePath){
	int size;

	FILE * f = fopen(filePath, "r");
	if(!f) return -1;

	fseek(f, 0L, SEEK_END); 
	size = ftell(f);

	fclose(f);
	return size;
}
//C:/Users/m video/Desktop/videos
int ifAvi(const char * filePath){
	FILE * f = fopen(filePath, "rb");
	if(!f) return -1;

	char buf[13];
	size_t result = fread(buf, 1, 12, f);

	if(result < 12) {
		fclose(f);
		return 0;
	}

	if(buf[0] != 'R') {
		fclose(f);
		return 0;
	}
	if(buf[1] != 'I') {
		fclose(f);
		return 0;
	}
	if(buf[2] != 'F') {
		fclose(f);
		return 0;
	}
	if(buf[3] != 'F') {
		fclose(f);
		return 0;
	}
	if(buf[8] != 'A') {
		fclose(f);
		return 0;
	}
	if(buf[9] != 'V') {
		fclose(f);
		return 0;
	}
	if(buf[10] != 'I') {
		fclose(f);
		return 0;
	}
	fclose(f);
	return 1;
}
//C:/Users/m video/Desktop/videos
int ifSame(const char * firsFilePath, const char * secondFilePath){
	FILE * fisrtFile = fopen(firsFilePath, "r");
	if(!fisrtFile) return -1;

	FILE * secondFile = fopen(secondFilePath, "r");
	if(!secondFile) {
		fclose(fisrtFile);
		return -1;
	}

	int firstFileSize = getSize(firsFilePath);
	int secondFileSize = getSize(secondFilePath);
	char first;
	char second;

	if(firstFileSize != secondFileSize) {
		fclose(fisrtFile);
		fclose(secondFile);
		return 0;
	}

	for(int i; i<firstFileSize; i++){
		first = fgetc(fisrtFile);
		second = fgetc(secondFile);
		if(first!=second) {
			fclose(fisrtFile);
			fclose(secondFile);
			return 0;
		}
	}
	fclose(fisrtFile);
	fclose(secondFile);
	return 1;
}
//C:/Users/m video/Desktop/videos
void aviCount(const char * dir, int * aviAmount){
	DIR *dp;
	if((dp = opendir(dir)) == NULL) {
        printf("cannot open directory\n");
        return;
    }

	char currentPath[PATH_MAX+1]; 

    struct dirent *entry;
    struct stat statbuf;

    chdir(dir);
    while((entry = readdir(dp)) != NULL) {

    	strcpy(currentPath, dir);
    	strcat(currentPath, "/");
        strcat(currentPath, entry->d_name);

        stat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0 || strcmp("avis",entry->d_name) == 0)
                continue;
            aviCount(currentPath, aviAmount);
			continue;
        }
        if(ifAvi(currentPath) == 1) *(aviAmount) = *(aviAmount) + 1;
    }
    chdir("..");
    closedir(dp);
}
void getAviPathes(const char * dir, char ** aviPathes, int * num){
	DIR *dp;
	if((dp = opendir(dir)) == NULL) {
        printf("cannot open directory\n");
        return;
    }

	char currentPath[PATH_MAX+1]; 

    struct dirent *entry;
    struct stat statbuf;

    chdir(dir);
    while((entry = readdir(dp)) != NULL) {

    	strcpy(currentPath, dir);
    	strcat(currentPath, "/");
        strcat(currentPath, entry->d_name);

        stat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0 || strcmp("avis",entry->d_name) == 0)
                continue;
            getAviPathes(currentPath, aviPathes, num);
			continue;
        }
        
        if(ifAvi(currentPath) == 1){
        	strcpy(aviPathes[(*num)],currentPath);
        	(*num) = (*num) + 1;
        }
   	}
    chdir("..");
    closedir(dp);
}