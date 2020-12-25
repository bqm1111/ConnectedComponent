#include "ccl.hpp"


#define NOMINMAX

#ifdef _MSC_VER
#include <ctime>
inline double get_time()
{
        return static_cast<double>(std::clock()) / CLOCKS_PER_SEC;
}
#else
#include <sys/time.h>
inline double get_time()
{
        timeval tv;
        gettimeofday(&tv, 0);
        return tv.tv_sec + 1e-6 * tv.tv_usec;
}
#endif


void init_CCL(int L[], int R[], int N)
{
    for (int id = 0; id < N; id++) L[id] = R[id] = id;
}

int diff(int d1, int d2)
{
    return abs(((d1>>16) & 0xff) - ((d2>>16) & 0xff)) + abs(((d1>>8) & 0xff) - ((d2>>8) & 0xff)) + abs((d1 & 0xff) - (d2 & 0xff));
}

bool scanning(int D[], int L[], int R[], int N, int W, int th)
{
    int m = false;

    for (int id = 0; id < N; id++) {
        int Did = D[id];
        int label = N;
        if (id - W >= 0 && diff(Did, D[id-W]) <= th) label = std::min(label, L[id-W]);
        if (id + W < N  && diff(Did, D[id+W]) <= th) label = std::min(label, L[id+W]);
        int r = id % W;
        if (r           && diff(Did, D[id-1]) <= th) label = std::min(label, L[id-1]);
        if (r + 1 != W  && diff(Did, D[id+1]) <= th) label = std::min(label, L[id+1]);

        if (label < L[id]) {
            R[L[id]] = label;
            m = true;
        }
    }

    return m;
}

bool scanning8(int D[], int L[], int R[], int N, int W, int th)
{
    bool m = false;

    for (int id = 0; id < N; id++) {
        int Did = D[id];
        int label = N;
        if (id - W >= 0 && diff(Did, D[id-W]) <= th) label = std::min(label, L[id-W]);
        if (id + W < N  && diff(Did, D[id+W]) <= th) label = std::min(label, L[id+W]);
        int r = id % W;
        if (r) {
            if (diff(Did, D[id-1]) <= th) label = std::min(label, L[id-1]);
            if (id - W - 1 >= 0 && diff(Did, D[id-W-1]) <= th) label = std::min(label, L[id-W-1]);
            if (id + W - 1 < N  && diff(Did, D[id+W-1]) <= th) label = std::min(label, L[id+W-1]);
        }
        if (r + 1 != W) {
            if (diff(Did, D[id+1]) <= th) label = std::min(label, L[id+1]);
            if (id - W + 1 >= 0 && diff(Did, D[id-W+1]) <= th) label = std::min(label, L[id-W+1]);
            if (id + W + 1 < N  && diff(Did, D[id+W+1]) <= th) label = std::min(label, L[id+W+1]);
        }

        if (label < L[id]) {
            R[L[id]] = label;
            m = true;
        }
    }

    return m;
}

void analysis(int D[], int L[], int R[], int N)
{
    for (int id = 0; id < N; id++) {
        int label = L[id];
        int ref;
        if (label == id) {
            do { label = R[ref = label]; } while (ref ^ label);
            R[id] = label;
        }
    }
}

void labeling(int D[], int L[], int R[], int N)
{
    for (int id = 0; id < N; id++)
        L[id] = R[R[L[id]]];
}



std::vector<int> CCL::ccl(std::vector<int>& image, int W, int degree_of_connectivity, int threshold)
{
    int* D = static_cast<int*>(&image[0]);
    int N = image.size();
    int* L = new int[N];
    int* R = new int[N];

    init_CCL(L, R, N);

    for (;;) {
        if (degree_of_connectivity == 4 ? scanning(D, L, R, N, W, threshold) : scanning8(D, L, R, N, W, threshold)) {
            analysis(D, L, R, N);
            labeling(D, L, R, N);
        } else break;
    }

    std::vector<int> result(L, L + N);

    delete [] L;
    delete [] R;

    return result;
}

void read_data(const std::string filename, std::vector<int>& image, int& W, int& degree_of_connectivity, int& threshold)
{
    std::fstream fs(filename.c_str(), std::ios_base::in);
    std::string line;
    std::stringstream ss;
    int data;

    std::getline(fs, line);
    ss.str(line);
    ss >> W >> degree_of_connectivity >> threshold;
    std::getline(fs, line);
    ss.str("");  ss.clear();
    for (ss.str(line); ss >> data; image.push_back(data));
}

