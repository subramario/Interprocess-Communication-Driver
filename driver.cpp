#include<vector>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

//USE CERR FOR ERRORS FOR PIPING!
using namespace std;

int main (int argc, char **argv) {
	vector<pid_t> kids;

	int Rgen_AOne[2];
	int AOne_ATwo[2];

	pipe(Rgen_AOne);
	pipe(AOne_ATwo);

	//Mapping A1
	pid_t child_pid;
	child_pid = fork();
	if (child_pid == 0) {
		dup2(Rgen_AOne[0], STDIN_FILENO); //Taking standard input as Rgen 
		dup2(AOne_ATwo[1], STDOUT_FILENO); //Standard output 

		close(Rgen_AOne[1]);
	    close(AOne_ATwo[0]);  
	    close(AOne_ATwo[0]);
		close(AOne_ATwo[1]);

	    char *args[]={(char*)"python3",(char*)"ece650-a1.py",(char*)NULL}; 
	    // cerr << "Hello1\n";
	    execvp("python3",args);
	    // cerr << "Returned from A1" << endl;

	    return 1;
	}

	else if (child_pid < 0) {
        cerr << "Error: could not fork\n";
        return 1;
    }

	kids.push_back(child_pid);

	//Mapping A2
	child_pid = fork();
	if (child_pid == 0){
		dup2(AOne_ATwo[0], STDIN_FILENO); //Taking standard input as Rgen 

	    close(AOne_ATwo[0]);  
	    close(AOne_ATwo[1]); 

	    char *args[]={(char*)"a2ece650",(char*)NULL};
	    // cerr << "Hello2\n"; 
	    execvp("./a2ece650",args);

	    // cerr << "Returned from A2" << endl;
	    return 1;
	}

	else if (child_pid < 0) {
        cerr << "Error: could not fork\n";
        return 1;
    }

	kids.push_back(child_pid);

	child_pid = fork();
	if (child_pid == 0){
		dup2(Rgen_AOne[1], STDOUT_FILENO); //Taking standard input as Rgen 

	    close(Rgen_AOne[0]); 

	    argv[0] = (char*)"rgen";
	    // cerr << "Hello3\n";

	    execvp("./rgen",argv);

	    // cerr << "Returned from Rgen" << endl;
	    return 1;
	}

	else if (child_pid < 0) {
        cerr << "Error: could not fork\n";
        return 1;
    }

	kids.push_back(child_pid);

	dup2(AOne_ATwo[1], STDOUT_FILENO);

	child_pid = fork();
	if (child_pid == 0) {
		while (!cin.eof()){
			string input;
			getline(cin,input);

			cout << input << endl;
	
		}

		return 1;
	} 

	else if (child_pid < 0) {
        cerr << "Error: could not fork\n";
        return 1;
    }

	kids.push_back(child_pid);

	wait(NULL);

	for (pid_t k : kids){
		kill (k, SIGTERM);
	}



    return 0;

}
