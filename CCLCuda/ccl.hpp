#ifndef CCL_HPP
#define CCL_HPP
#include "define.h"
#include "string.h"
#include <string>
void init_CCL(int L[], int R[], int N);
int diff(int d1, int d2);
bool scanning(int D[], int L[], int R[], int N, int W, int th);
bool scanning8(int D[], int L[], int R[], int N, int W, int th);
void analysis(int D[], int L[], int R[], int N);
void labeling(int D[], int L[], int R[], int N);
class CCL {
public:
    std::vector<int> ccl(std::vector<int>& image, int W, int degree_of_connectivity, int threshold);
};
void read_data(const std::string filename, std::vector<int>& image, int& W, int& degree_of_connectivity, int& threshold);
#endif // CCL_HPP
