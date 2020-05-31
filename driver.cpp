#include<vector>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

using namespace std;

int main (int argc, char **argv) {
	vector<pid_t> kids; // create a vector to store process ID's for all children

	int Rgen_AOne[2];
	int AOne_ATwo[2];

	pipe(Rgen_AOne); // create pipe to link Rgen to A1 (pipe 1)
	pipe(AOne_ATwo); // create pipe to link A1 to Rgen (pipe 2)

	pid_t child_pid; // create a process id variable  
	
	// concurrently run Python script
	child_pid = fork();
	
	// fork successfully produces a child
	if (child_pid == 0) { 
		// troubleshooting: cerr << "Within Python child\n";
		
		dup2(Rgen_AOne[0], STDIN_FILENO); // map input to "read" end of pipe 1
		dup2(AOne_ATwo[1], STDOUT_FILENO); // map output to "write" end of pipe 2
		
		// close all file descriptors to prevent child from sharing parent's resources
		close(Rgen_AOne[0]);
		close(Rgen_AOne[1]);
	    	close(AOne_ATwo[0]);  
		close(AOne_ATwo[1]);
		
		// arguments needed to run the Python script
		char *args[]={(char*)"python3",(char*)"ece650-a1.py",(char*)NULL}; 
		
		// replaces child process with image of specified program
		execvp("python3",args);

		return 1;
	}	
	
	// fork fails to produce a child - still within parent process
	else if (child_pid < 0) {
		cerr << "Error: could not fork\n";
		return 1;
	}
	
	kids.push_back(child_pid);

	// concurrently run shortest path interface program
	child_pid = fork();
	
	// fork successfully produces a child
	if (child_pid == 0){
		// troubleshooting: cerr << "Within Shortest Path C++ child\n";

		dup2(AOne_ATwo[0], STDIN_FILENO); // map input to "read" end of pipe 2

		// close all file descriptors to prevent child from sharing parent's resources
		close(AOne_ATwo[0]);  
		close(AOne_ATwo[1]); 

		// arguments needed to run the C++ script
		char *args[]={(char*)"a2ece650",(char*)NULL};

		// replaces child process with image of specified program
		execvp("./a2ece650",args);

		return 1;
	}

	// fork fails to produce a child - still within parent process
	else if (child_pid < 0) {
		cerr << "Error: could not fork\n";
		return 1;
	}

	kids.push_back(child_pid);

	// concurrently run randomized generator program
	child_pid = fork();
	
	// fork successfully produces a child
	if (child_pid == 0){
		// troubleshooting: cerr << "Within Random Generator C++ child\n";
		dup2(Rgen_AOne[1], STDOUT_FILENO); // map output to "write" end of pipe 1

		// close all file descriptors to prevent child from sharing parent's resources
		close(Rgen_AOne[0]);
		close(Rgen_AOne[1]); 

		// store arguments specified to driver into array to use for rgen
		argv[0] = (char*)"rgen";
		
		// replaces child process with image of specified program
		execvp("./rgen",argv);

		return 1;
	}

	else if (child_pid < 0) {
		cerr << "Error: could not fork\n";
		return 1;
	}

	kids.push_back(child_pid);

	// concurrently accept user input to use shortest path interface
	child_pid = fork();
	
	// fork successfully produces a child
	if (child_pid == 0) {
		dup2(AOne_ATwo[1], STDOUT_FILENO); // map output of keyboard entry to "write" end of pipe 2 
		
		// parses user input
		while (!cin.eof()){
			string input;
			getline(cin,input); // parses user input and stores in variable input

			cout << input << endl;
		}

		return 1;
	} 

	// fork fails to produce a child - still within parent process
	else if (child_pid < 0) {
		cerr << "Error: could not fork\n";
		return 1;
    	}

	kids.push_back(child_pid);

	wait(NULL); // wait for all processes to complete output

	// terminate all child processes
	for (pid_t k : kids){
		kill (k, SIGTERM);
	}



    return 0;

}
