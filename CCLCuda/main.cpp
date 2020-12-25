#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <list>
#include <algorithm>
#include <utility>
#include <cmath>
#include <functional>
#include <cstring>
#include <cmath>
#include <limits>
#include "ccl.hpp"
using namespace std;

int main(int argc, char* argv[])
{
    ios_base::sync_with_stdio(false);

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " input_file" << endl;
        exit(1);
    }

    vector<int> image;
    int W, degree_of_connectivity, threshold;
    read_data(argv[1], image, W, degree_of_connectivity, threshold);

    CCL ccl;

    vector<int> result(ccl.ccl(image, W, degree_of_connectivity, threshold));

    cout << result.size() << endl; /// number of pixels
    cout << W << endl; /// width
    for (int i = 0; i < static_cast<int>(result.size()) / W; i++) {
        for (int j = 0; j < W; j++) cout << result[i*W+j] << " ";
        cout << endl;
    }

    return 0;
}
