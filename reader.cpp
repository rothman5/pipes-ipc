#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

using namespace std;

#define MAX_SIZE 1024

struct Point {
    long long lat, lon;
};

void parse(string* line_info, string line, string delimiter = ",") {
  int start = 0;
  int end = line.find(delimiter);
  int i = 0;
  
  while (end != -1) {
      line_info[i] = line.substr(start, end - start);
      start = end + delimiter.size();
      end = line.find(delimiter, start);
      i++;
  }
  line_info[3] = line.substr(start, end - start);
}

int open_fifo(const char* pipeName, int mode) {
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

    const char * inpipe = "outpipe";

    int ifd = open_fifo(inpipe, O_RDONLY);
    cout << "inpipe opened..." << endl;

    char buffer[MAX_SIZE];
    cout << "Reader started..." << endl;
    string toParse;

    Point sPoint, ePoint;

    while (true) {
        int bytesRead = read(ifd, buffer, MAX_SIZE);        

        if (strcmp("q", buffer) == 0)
            break;
    	
        for (int i = 0; i < bytesRead; i++) {
        	toParse += buffer[i];
        }
        string pointInfo[4];
        parse(pointInfo, toParse);

        if (debug) {
        	// Test parse: 53.530870 -113.532972\n53.530870 -113.514991\n

        	if (bytesRead == -1)
            	cerr << "[Error] Read operation failed!" << endl;

        	cout << "[Debug] # of bytes read into buffer: " << bytesRead << endl;
        	cout << "[Debug] Buffer contents:" << endl << "==> " << buffer << endl;

        	cout << "[Debug] Parsed content from buffer:" << endl << "==> " << 
        	pointInfo[0] << " " << pointInfo[1] << endl << "==> " << 
        	pointInfo[2] << " " << pointInfo[3] << endl;

        	cout << "[Debug] Point latitude and longitude:" << endl << "==> " << 
        	sPoint.lat << " " << sPoint.lon << endl << "==> " << 
        	ePoint.lat << " " << ePoint.lon << endl;
        }

        sPoint.lat = static_cast<long long>(stod(pointInfo[0])*100000);
        sPoint.lon = static_cast<long long>(stod(pointInfo[1])*100000);
        ePoint.lat = static_cast<long long>(stod(pointInfo[2])*100000);
        ePoint.lon = static_cast<long long>(stod(pointInfo[3])*100000);

        toParse = "";
    }
    close(ifd);
    unlink(inpipe);

	return 0;
}
