#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>
using namespace std;

vector<string> split_into_chunks(string* input) {
    vector<string> chunks;

    for(int i = 0; i < input->size(); i += 3) {
        chunks.push_back(input->substr(i, min(3, (int)(input->size() - i))));
    }

    // if(chunks.back().size() != 3) {
    //     int n = 3 - chunks.back().size();

    //     chunks.back().append(n, '=');
    // }

    return chunks;
}

void encode(vector<string>* chunks) {
    char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string base64_result = "";

    for(int i = 0; i < chunks->size() - 1; i++) {
        string chunk = (*chunks)[i];
        uint32_t combined = 0;

        // All regular chunks should have exactly 3 characters
        combined |= (static_cast<unsigned char>(chunk[0]) << 16);
        combined |= (static_cast<unsigned char>(chunk[1]) << 8);
        combined |= static_cast<unsigned char>(chunk[2]);

        // Extract 6-bit groups and map them to Base64
        base64_result += base64_chars[(combined >> 18) & 0x3F];
        base64_result += base64_chars[(combined >> 12) & 0x3F];
        base64_result += base64_chars[(combined >> 6) & 0x3F];
        base64_result += base64_chars[combined & 0x3F];

        // cout << i << " " << combined << " " << base64_result << endl;
    }

    // Handle the last chunk
    if(!chunks->empty()) {
        string last_chunk = chunks->back();
        uint32_t combined = 0;

        if(last_chunk.length() == 1) {
            combined |= (static_cast<unsigned char>(last_chunk[0]) << 16);
    
            base64_result += base64_chars[(combined >> 18) & 0x3F];
            base64_result += base64_chars[(combined >> 12) & 0x3F];
            base64_result += "==";
        } else if(last_chunk.length() == 2) {
            combined |= (static_cast<unsigned char>(last_chunk[0]) << 16);
            combined |= (static_cast<unsigned char>(last_chunk[1]) << 8);
    
            base64_result += base64_chars[(combined >> 18) & 0x3F];
            base64_result += base64_chars[(combined >> 12) & 0x3F];
            base64_result += base64_chars[(combined >> 6) & 0x3F];
            base64_result += "=";
        } else if (last_chunk.length() == 3) {
            combined |= (static_cast<unsigned char>(last_chunk[0]) << 16);
            combined |= (static_cast<unsigned char>(last_chunk[1]) << 8);
            combined |= static_cast<unsigned char>(last_chunk[2]);
    
            base64_result += base64_chars[(combined >> 18) & 0x3F];
            base64_result += base64_chars[(combined >> 12) & 0x3F];
            base64_result += base64_chars[(combined >> 6) & 0x3F];
            base64_result += base64_chars[combined & 0x3F];
        }

        // cout << chunks->size() - 1 << " " << combined << " " << base64_result << endl;
    }

    cout << "Base64: " << base64_result << endl;
}

int main() {
    string input;
    getline(cin, input);

    vector<string> chunks = split_into_chunks(&input);

    cout << chunks.size() << endl;
    
    for (const auto& it : chunks) {
        cout << "'" << it << "' ";
    }

    cout << endl << endl;
    encode(&chunks);

    return 0;
}