#include "../include/duck_db.h"

using namespace std;

void initialSystem(){
    // print help message
    printHelpMess();
    duck_db_ptr = new bplus_tree(dbFileName, (!is_file_exist(dbFileName)));
    // scan user input
    readCommand();
}
// print help message
void printHelpMess(){
    cout << "*********************************************************************************************"<<endl<<endl
		<<" 				Welcome to the duck_db\n 				db file locates in \"./data/db.bin\" \n  "<<endl<<endl
		<<"*********************************************************************************************"<<endl
		<<"  .help 							print help message;"<<endl
		<<"  .exit 							exit program;"<<endl
		<<"  .reset 							delete db file;"<<endl
		<<"  insert db {index} {name} {age} {email}; 			insert record;"<<endl
		<<"  delete from db where id={index}; 				delete record;"<<endl
		<<"  update db {name} {age} {email} where id={index}; 		update a record;"<<endl
		<<"  select * from db where id={index}; 				search a record by index;"<<endl
		<<"*********************************************************************************************"<<endl
		<<endl << nextLineHeader;
}
void readCommand(){
    char *userCommand = new char[256];
    while(true){
        cin.getline(userCommand, 256);
        if(strcmp(userCommand, ".exit") == 0){
            exitCommand();
        }
        if(strcmp(userCommand, ".help") == 0){
            helpCommand();
            continue;
        }
        if(strcmp(userCommand, ".reset") == 0){
            resetCommand();
            continue;
        }
        if(strcmp(userCommand, ".reset") == 0){
            resetCommand();
            continue;
        }
        if(strncmp(userCommand,"insert",6) == 0){
            insertCommand(userCommand);
            continue;
        }
        if(strncmp(userCommand,"update",6) == 0){
            updateCommand(userCommand);
            continue;
        }
        if(strncmp(userCommand,"delete",6) == 0){
            deleteCommand(userCommand);
            continue;
        }
        if(strncmp(userCommand,"select",6) == 0){
            selectCommand(userCommand);
            continue;
        }
        cout << errorMessage<< nextLineHeader;

    }
}
void exitCommand(){
    cout << exitMessage;
    exit(0);
}
void helpCommand(){
    printHelpMess();
}
void resetCommand(){
    if(remove(dbFileName) == 0){
        cout << "can't delete file" << nextLineHeader;
    }else{
        cout << "DB file has been deleted!" << endl << endl;
    }
    initialSystem();
}
void insertCommand(const char *userCommand){
    int *keyIndex = new int;
    value_t *row = new value_t;
    int validInputs = sscanf(userCommand, "insert db %d %s %d %s;", keyIndex, row->name, &(row->age), row->email);
    if(validInputs < 3){
        cout << errorMessage << nextLineHeader;
    }else{
        startTime = clock();
        int return_code = insertRecord(duck_db_ptr, keyIndex, row);
        finishTime = clock();
        switch (return_code){
        case 0:
        cout << "> executed insert index:"<<   *keyIndex << ", time : "<< durationTime(&finishTime,&startTime) <<" seconds\n"<< nextLineHeader;
            break;
        case 1:
        cout << "> failed: already exist index:"<<   *keyIndex << "\n"<< nextLineHeader;
            break;
        default:
        cout << "> failed!\n"<< nextLineHeader;
            break;
        }
    }
}
void updateCommand(const char *userCommand){
    int *keyIndex = new int;
    value_t *row = new value_t;
    int validInputs = sscanf(userCommand,"update db %s %d %s where id=%d;", row->name,&(row->age),row->email,keyIndex);
    if(validInputs < 3){
        cout << errorMessage << nextLineHeader << endl;
    }else{
        startTime = clock();
        int return_code = updateRecord(duck_db_ptr, keyIndex, row);
        finishTime = clock();
        if (return_code == 0){
            cout << "> executed update index:"<<   *keyIndex << ", time : "<< durationTime(&finishTime,&startTime) <<" seconds\n"<< nextLineHeader;
            }else{
                cout << "> failed! no index:"<<   *keyIndex << ", time : "<< durationTime(&finishTime,&startTime) <<" seconds\n"<< nextLineHeader;
        }
    }
}
void deleteCommand(const char *userCommand){
    int *keyIndex = new int;
    int okNum = sscanf(userCommand,"delete from db where id=%d;", keyIndex);
    if(okNum < 1){
        cout << errorMessage << nextLineHeader;
        return;
    }
    startTime = clock();
    int return_code = deleteRecord(duck_db_ptr, keyIndex);
    finishTime = clock();

    if (return_code == 0){
        cout << "> executed delete index:"<<   *keyIndex << ", time : "<< durationTime(&finishTime,&startTime) <<" seconds\n"<< nextLineHeader;
    }else if(return_code == -1){
            cout << "> failed ! no index:"<<   *keyIndex << "\n"<< nextLineHeader;
        }else{
            cout << "> failed!\n"<< nextLineHeader;
    }

}
void selectCommand(const char *userCommand){
    int *keyIndex = new int;
    int okNum = sscanf(userCommand,"select * from db where id=%d;", keyIndex);
    if(okNum < 1){
        cout << errorMessage<< nextLineHeader;
        }else{
            value_t *return_val = new value_t;
            startTime = clock(); 
            int return_code = searchRecord(duck_db_ptr,keyIndex,return_val);
            finishTime = clock(); 
	    	if (return_code != 0){
                cout << "> index:"<< *keyIndex << " doesn't exist, time : "<< durationTime(&finishTime,&startTime) <<" seconds\n"<< nextLineHeader;
                }else{
                    printTable( keyIndex , return_val);
                    cout << "> executed search, time : "<< durationTime(&finishTime,&startTime) <<" seconds\n"<< nextLineHeader;	
                }
        }
}
bool is_file_exist(const char *fileName){
    ifstream ifile(fileName);
    return ifile.good();
}
int insertRecord(bplus_tree *treePtr, int *index, value_t *values){
    bpt::key_t key;
    convertIntToKeyT(&key, index);
    return (*treePtr).insert(key, *values);

}
void convertIntToKeyT(bpt::key_t *keyPtr, int *number){
    char key[16] = { 0 };
    sprintf(key, "%d", *number);
    *keyPtr = key;
}
double durationTime(clock_t *f, clock_t *s){
    return (double)(*f - *s) / CLOCKS_PER_SEC;
}
int updateRecord(bplus_tree *treePtr,int *index, value_t *values){
	bpt::key_t key;
    convertIntToKeyT(&key, index);
	return (*treePtr).update(key, *values);
}
int deleteRecord(bplus_tree *treePtr, int *index){
    bpt::key_t key;
    convertIntToKeyT(&key, index);
	return (*treePtr).remove(key);
}
// search by index
int searchRecord(bplus_tree *treePtr,int *index, value_t *return_val){
	bpt::key_t key;
    convertIntToKeyT(&key, index);
	return (*treePtr).search(key, return_val); 
}
// print table
void printTable(int *index, value_t *values){


	TextTable t( '-', '|', '+' );

    t.add( " id " );
    t.add( " name " );
    t.add( " age " );
    t.add( " email " );
    t.endOfRow();

    t.add( to_string(*index) );
    t.add( values ->name );
    t.add( to_string(values ->age));
    t.add( values ->email );
    t.endOfRow();

    cout << t << endl;
}
int main(int arg, char *argv[]){
	initialSystem();
    return 0;
}
