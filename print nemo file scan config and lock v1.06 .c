#include <windows.h>
#include <stdio.h>
#include <direct.h>

int get_locktype(char input_line[]){
			if(strstr(input_line , ",,0") != 0){
				printf("There is no lock.\n\n");
				return 0;
			}
		
			if(strstr(input_line , ",,1,3,1,7") != 0){
				printf("lock to LTE system.\n\n");
				return 0;
			}
		
			if(strstr(input_line , ",,1,3,1,5") != 0){
				printf("lock to UMTS system.\n\n");
				return 0;
			}
		
			if(strstr(input_line , ",,1,3,1,1") != 0){
				printf("lock to GSM system.\n\n");
				return 0;
			}
			
			return -1;
}

void print_scanconfig(char current_nmf_name []){
	char input_line[2048];
	bool not_do_first_print = 1;
	FILE *in;
	in = fopen(current_nmf_name , "r");
	if(in == NULL){
		printf("open %c fail! \n" , current_nmf_name);
		return;
	}
	
	while(!feof(in)){
		fgets(input_line , 2048 , in);
		if( strncmp(input_line , "SCANCONFIG" , 10) == 0 ){
			printf("filename : %s\n" , current_nmf_name);
			//printf("%s\n\n" , input_line);
			
			
			//---only get channel number---
			short DoubleQuote = 1;
			printf("scan      ");
			for(int i = 0 ; i < strlen(input_line) ; i++){
				if(DoubleQuote < 0 && input_line[i] != '\"' && input_line[i] != ','){
					printf("%c" , input_line[i]);
				}
				
				if(DoubleQuote < 0 &&input_line[i] == ','){
					printf(" ");
				}
				
				if(input_line [i] == '\"'){
					DoubleQuote *= -1;
					printf(" ");
				}
				
			}
			printf("\n\n");
			//---only get channel number---
			break;	//end read file while print the first result
		}
	}
	
	fclose(in);
}

void print_lock(char current_nmf_name []){
	char input_line[2048];
	FILE *in;
	in = fopen(current_nmf_name , "r");
	if(in == NULL){
		printf("open %c fail! \n" , current_nmf_name);
		return;
	}
	
	while(!feof(in)){
		fgets(input_line , 2048 , in);
		if( strncmp(input_line , "LOCK" , 4) == 0 ){
			printf("filename : %s\n" , current_nmf_name);
			if(get_locktype(input_line) != 0){
				printf("%s\n" , input_line);
			}
			break;	//end read file while print the first result
		}
	}
	
	fclose(in);
	
	
}

int main()
{
	
    char InputPath[65535] ;
	getcwd(InputPath , 65535);  
    //放要讀取檔案的資料夾路徑到InputPath字串裡
    char szDir[65535];
    char dir[65535];
    WIN32_FIND_DATA FileData;
    HANDLE          hList;
    sprintf(szDir, "%s\\*.nmf", InputPath );
    if ( (hList = FindFirstFile(szDir, &FileData))==INVALID_HANDLE_VALUE )	{
    	printf("No nemofiles be found.\n\n");
	}    
    else {
    		//get first *.nmf
    		//sprintf(dir, "%s\\%s", InputPath, FileData.cFileName);
            //printf("%s\n", dir);
            print_scanconfig(FileData.cFileName);
            print_lock(FileData.cFileName);
    	
        while (1) {
            if (!FindNextFile(hList, &FileData)) {
                if (GetLastError() == ERROR_NO_MORE_FILES)
                    break;
            }
            
            //get next *.nmf
            //sprintf(dir, "%s\\%s", InputPath, FileData.cFileName);
            //printf("%s\n", dir);
            print_scanconfig(FileData.cFileName);
            print_lock(FileData.cFileName);
        }
    }
    FindClose(hList);
    
    system("pause");
    
    return 0;
}
