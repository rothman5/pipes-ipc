#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

#define MAX_SIZE 1024

int create_and_open_fifo(const char* pipeName, int mode) {
	if (mkfifo(pipeName, 0666) == -1)
		cerr << "Error: failed to create pipe." << endl;

	int fd = open(pipeName, mode);
	if (fd == -1) {
		cerr << "Error: pipe already exists." << endl;
		unlink(pipeName);
		return 1;
	}
	return fd;
}

int main() {
	bool debug = true;

    const char * outpipe = "outpipe";

    int ofd = create_and_open_fifo(outpipe, O_WRONLY);
    cout << "outpipe opened..." << endl; 

    char wBuffer[MAX_SIZE] = {0};

	cout << "writer started..." << endl;
    cout << "Enter start coordinates:" << endl;

    while (true) {
    	cin.getline(wBuffer, MAX_SIZE);

    	int bytesWritten = write(ofd, wBuffer, strlen(wBuffer)+1);

		if (strcmp("q", wBuffer) == 0)
	    		break;

	    if (debug) {
	    	if (bytesWritten == -1)
    			cerr << "[Error] write operation failed!" << endl;

    		cout << "[Debug] # of bytes written into buffer: " << bytesWritten << endl;
	    }
	    cout << "Enter next coordinates:" << endl;
    }
    close(ofd);
    unlink(outpipe);

	return 0;
}
