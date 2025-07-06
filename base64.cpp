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

    cout << "\033[1;33m"   // Bright Yellow
     << "Base"
     << "\033[1;36m"   // Bright Cyan
     << "64"
     << "\033[0m"      // Reset
     << ": "
     << "\033[1;34m"   // Bright blue./ (for result)
     << base64_result
     << "\033[0m"      // Reset
     << endl;


    // string site_encode;
    // cout << "Insert site encode:\n";
    // getline(cin, site_encode);
    // if(base64_result == site_encode)
    //     cout << true << endl;
    // else 
    //     cout << false << endl;
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

    cout << "\033[1;35m"  // Bright magenta for label
     << "Decoded"
     << "\033[0m"     // Reset color
     << ": "
     << "\033[1;34m"  // Bright blue for result
     << result
     << "\033[0m"     // Reset
     << endl;


    // string site_decode;
    // cout << "Insert site decode:\n";
    // getline(cin, site_decode);

    // if(result == site_decode)
    //     cout << true << endl;
    // else 
    // cout << false << endl;
}

void banner() {
    cout << "\033[0;34m"  // bright blue
         << R"(
         __                                  _____   __ __
        / /_   ____ _   _____  ___          / ___/  / // /
       / __ \ / __ `/  / ___/ / _ \        / __ \  / // /_
      / /_/ // /_/ /  (__  ) /  __/       / /_/ / /__  __/
     /_.___/ \__,_/  /____/  \___/        \____/    /_/   
        )"
         << "\033[0m\n\n"
         << "\033[1;33mBASE64\033[0m Encoder/Decoder Tool "
         << "\033[1;31mintelkumi\033[0m v1.0\n\n";
}

void header() {
    cout << "\033[1;32m"
         << "================== BASE64 ENCODER/DECODER ==================\n"
         << "          Powered by El Kumi Cipher Labs (v1.0)\n"
         << "===========================================================\n"
         << "\033[0m";
}

void help() {
    cout << "\033[1;33mUsage:\033[0m\n"
         << "  ./base64 -e <string_to_encode>    Encode input string\n"
         << "  ./base64 -d <string_to_decode>    Decode input string\n\n";
}

int main(int argc, char* argv[]) {

    if(argc < 2) {
        help();

        return 1;
    }

    banner();
    header();

    string option = argv[1];
    string input;
    string line;

    if (option == "-e") {
        if (argc < 3) {
        std::cerr << "Error: Missing string to encode" << std::endl;
        help();
        return 1;
    }
    std::string input = argv[2];

    // If you want to support multi-word input, you could join argv[2..] here

    auto chunks = split_into_chunks(&input);
    encode(&chunks);

    } 
    else if (option == "-d") {
        if (argc < 3) {
            std::cerr << "Error: Missing argument for -d" << std::endl;
            help();
            return 1;
        }
        std::string encoded_input = argv[2];
        decode(encoded_input);
    }
    else {
        help();
        return 1;
    }
    return 0;
}