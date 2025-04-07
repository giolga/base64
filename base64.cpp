#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>
#include <bitset>
using namespace std;

vector<string> split_into_chunks(string* input) {
    vector<string> chunks;

    for(int i = 0; i < input->size(); i += 3) {
        chunks.push_back(input->substr(i, min(3, (int)(input->size() - i))));
    }

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

void decode(string input) {
    string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    int padding = 0;
    while (!input.empty() && input.back() == '=') {
        padding++;
        input.pop_back();
    }

    string bitstream = "";

    // Step 2 & 3: Convert each character to its 6-bit binary representation
    for (char c : input) {
        int index = base64_chars.find(c);
        bitset<6> b(index);
        bitstream += b.to_string();
    }

    // Step 4 & 5: Split bitstream into 8-bit chunks and convert to characters
    string result = "";
    for (size_t i = 0; i + 8 <= bitstream.size(); i += 8) {
        string byte_str = bitstream.substr(i, 8);
        bitset<8> byte(byte_str);
        result += static_cast<char>(byte.to_ulong());
    }

    cout << "Decoded: " << result << endl;
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

    string decode_input ;
    cin >> decode_input;
    decode(decode_input);

    return 0;
}