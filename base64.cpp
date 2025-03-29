#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> split_into_chunks(string* input) {
    vector<string> chunks;

    for(int i = 0; i < input->size(); i += 3) {
        chunks.push_back(input->substr(i, 3));
    }

    if(chunks.back().size() != 3) {
        int n = 3 - chunks.back().size();

        chunks.back().append(n, '=');
    }

    return chunks;
}

void convert_3bytes_into_24bits(vector<string>* chunks) {
    char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
}

int main() {
    string input;
    cin >> input;

    vector<string> chunk = split_into_chunks(&input);
    
    for (const auto& it : chunk) {
        cout << it << " ";
    }

    return 0;
}