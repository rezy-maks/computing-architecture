#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/time.h>
#include <termios.h>

using namespace std;

double wtime() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double) t.tv_sec + (double) t.tv_usec * 1E-6;
}

double getAverage(const double *array, int size) {
    double average = 0;
    for (int i = 0; i < size; i++) {
        average += array[i];
    }
    return average / size;
}

long long getSize(string block) {
    long long size = 1, _size;
    bool isNum = true;
    long long i;
    string s = "";
    for (i = 0; i < block.size(); i++) {
        if (!isdigit(block[i])) {
            isNum = false;
            break;
        }
        s.push_back(block[i]);
    }
    if (block[i] == 'K' && block[i + 1] == 'b') {
        size = 1024;
    } else if (block[i] == 'M' && block[i + 1] == 'b') {
        size = 1048576;
    }

    return static_cast<long long>(stoi(s)) * size;
}

void initArray(char *array, long long size) {
    for (long long i = 0; i < size; i++) {
        array[i] = rand() % 256;
    }
}

double maxElem(double *timeBuf, int size) {
    double max = timeBuf[0];
    for (int i = 1; i < size; i++) {
        if (max < timeBuf[i]) {
            max = timeBuf[i];
        }
    }
    return max;
}

void writeCSV(int mode, long long blockSize, double *writeTimeBuf, double avWriteTime, double *readTimeBuf, double avReadTime, int launchCount) {
    string memoryType;
    if (mode == 1)
        memoryType = "RAM";
    else if (mode == 2)
        memoryType = "HDD";
    else if (mode == 3)
        memoryType = "SSD";
    else if (mode == 4)
        memoryType = "flash";
    string elementType = "char";
    long long bufferSize = blockSize;
    int launchNum = 5;
    double writeTime = round(writeTimeBuf[launchNum] * 100) / 100;
    string timer = "sys/time.h";
    string writeBandWidth = to_string(round(blockSize / avWriteTime * 100) / (100 * 1024 * 1024));
    double absErrorWrite = round((maxElem(writeTimeBuf, launchCount) - avWriteTime) * 100) / 100;
    double relErrorWrite = absErrorWrite / avWriteTime * 100;
    double readTime = round(readTimeBuf[launchNum] * 100) / 100;
    string readBandWidth = to_string(round(blockSize / avReadTime * 100) / (100 * 1024 * 1024));
    double absErrorRead = round((maxElem(readTimeBuf, launchCount) - avReadTime) * 100) / 100;
    double relErrorRead = absErrorRead / avReadTime * 100;

    ofstream fout("file.csv", ios_base::app);
    if (fout.good()) {
        fout << memoryType << ";" << blockSize << ";" << elementType << ";" << bufferSize << ";"
             << launchNum << ";" << timer << ";" << writeTime << ";" << avWriteTime << ";"
             << writeBandWidth << ";" << absErrorWrite << ";" << relErrorWrite << "%;"
             << readTime << ";" << avReadTime << ";" << readBandWidth << ";"
             << absErrorRead << ";" << relErrorRead << "%\n";
    }
    fout.close();
}

double noEchoRead(char *array, long long size, const string &path = "", int mode = 1) {
    double time = -1;
    if (mode == 1) {
        time = wtime();
        for (long long i = 0; i < size; i++) {
            *(array + i);
        }
        time = wtime() - time;
    } else {
        ifstream fin(path);
        if (fin.good()) {
            time = wtime();
            while (!fin.eof()) {
                fin.get();
            }
            time = wtime() - time;
        }
    }

    return time;
}

pair<double, double> testMemory(long long size, int mode, const string& path = "") {
    double timeRead = -1;
    double timeWrite = -1;
    char *buffer = new char[size];

    if (mode == 1) {
        timeWrite = wtime();
        for (long long i = 0; i < size; i++) {
            buffer[i] = static_cast<char>(rand() % 26 + 65);
        }
        timeWrite = wtime() - timeWrite;
        timeRead = noEchoRead(buffer, size);
    } else if (mode > 1 && mode < 5) {
        ofstream fout(path);
        if (fout.good()) {
            initArray(buffer, size);
            timeWrite = wtime();
            for (long long i = 0; i < size; i++) {
                fout << buffer[i];
            }
            timeWrite = wtime() - timeWrite;
        }
        fout.close();
        timeRead = noEchoRead(buffer, size, path, mode);
    }
    return make_pair(timeWrite, timeRead);
}

int main(int argc, char **argv) {
    srand(time(NULL));

    int launchCount = 0;
    int mode;
    long long size = 0;
    string path;

    if (argc != 9) {
        cout << "./memory_test –m [RAM/HDD/SSD/flash] –b [size(b/Kb/Mb (b default))] –l [launch count] -p [path]" << endl;
        return 1;
    } else {
        for (int i = 1; i < 9; i += 2) {
            if (strcmp(argv[i], "-m") == 0) {
                if (strcmp(argv[i + 1], "RAM") == 0)
                    mode = 1;
                else if (strcmp(argv[i + 1], "HDD") == 0)
                    mode = 2;
                else if (strcmp(argv[i + 1], "SSD") == 0)
                    mode = 3;
                else if (strcmp(argv[i + 1], "flash") == 0)
                    mode = 4;
            } else if (strcmp(argv[i], "-b") == 0) {
                string strSize = argv[i + 1];
                size = getSize(strSize);
            } else if (strcmp(argv[i], "-l") == 0)
                launchCount = stoi(argv[i + 1]);
            else if (strcmp(argv[i],"-p") == 0 && mode > 1) {
            path = argv[i + 1];
            path += "/test.txt";
            }
        }
    }

    double *writeTimeBuffer = new double[launchCount];
    double *readTimeBuffer = new double[launchCount];

    for (int i = 0; i < launchCount; i++) {
        pair<double, double> time;
        if (mode != 1) {
            time = testMemory(size, mode, path);
        }
        else {
            time = testMemory(size, mode);
        }
        writeTimeBuffer[i] = round(time.first * 100) / 100;
        readTimeBuffer[i] = round(time.second * 100) / 100;
    }
    double averageWriteTime = round(getAverage(writeTimeBuffer, launchCount) * 100) / 100;
    double averageReadTime = round(getAverage(readTimeBuffer, launchCount) * 100) / 100;

    writeCSV(mode, size, writeTimeBuffer, averageWriteTime, readTimeBuffer, averageReadTime, launchCount);
    delete(writeTimeBuffer);
    delete(readTimeBuffer);
    return 0;
}

//6 Mb - cache memory

