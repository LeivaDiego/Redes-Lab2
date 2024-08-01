#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <algorithm>

using namespace std;

class HammingEncoder {
public:
    HammingEncoder() {}

    vector<string> encodeList(const vector<string>& binaryStrings) {
        vector<string> encodedList;
        for (const string& binStr : binaryStrings) {
            encodedList.push_back(encode(binStr));
        }
        return encodedList;
    }

private:
    string encode(const string& data) {
        int m = data.size();
        int r = 0;

        while ((1 << r) < (m + r + 1)) {
            r++;
        }

        string encoded(m + r, '0');
        int j = 0;

        for (int i = 0; i < m + r; i++) {
            if ((i & (i + 1)) == 0) {
                encoded[i] = '0';
            } else {
                encoded[i] = data[j];
                j++;
            }
        }

        for (int i = 0; i < r; i++) {
            int pos = (1 << i) - 1;
            encoded[pos] = calculateParityBit(encoded, pos, m + r);
        }

        return encoded;
    }

    char calculateParityBit(const string& encoded, int pos, int length) {
        int parity = 0;
        for (int i = 0; i < length; i++) {
            if (i != pos && (i & (pos + 1))) {
                parity ^= (encoded[i] - '0');
            }
        }
        return parity ? '1' : '0';
    }
};

class CRC32Encoder {
public:
    CRC32Encoder() : POLY("100000100110000010001110110110111") {}

    vector<string> encodeList(const vector<string>& binaryStrings) {
        vector<string> encodedList;
        for (const string& binStr : binaryStrings) {
            encodedList.push_back(encodeCRC32(binStr));
        }
        return encodedList;
    }

private:
    const string POLY;

    string binaryXor(const string& a, const string& b) {
        string result = "";
        int n = b.length();
        for (int i = 1; i < n; i++) {
            result += (a[i] == b[i]) ? "0" : "1";
        }
        return result;
    }

    string modulo2division(const string& dividend, const string& divisor) {
        int pick = divisor.length();
        string block = dividend.substr(0, pick);
        int n = dividend.length();

        while (pick < n) {
            if (block[0] == '1') {
                block = binaryXor(divisor, block) + dividend[pick];
            } else {
                block = binaryXor(string(pick, '0'), block) + dividend[pick];
            }
            pick++;
        }

        if (block[0] == '1') {
            block = binaryXor(divisor, block);
        } else {
            block = binaryXor(string(pick, '0'), block);
        }

        return block;
    }

    string encodeCRC32(const string& message) {
        int n = POLY.length();
        string augmented_message = message + string(n - 1, '0');
        string remainder = modulo2division(augmented_message, POLY);
        return message + remainder;
    }
};

// Function to convert a string to a list of binary ASCII codes
vector<string> stringToBinaryList(const string& input) {
    vector<string> binaryList;
    for (char c : input) {
        binaryList.push_back(bitset<8>(c).to_string());
    }
    return binaryList;
}

int main() {
    string choice, message;

    cout << "Encoding methods: " << endl ;
    cout << "- [1] Hamming Encoding" << endl;
    cout << "- [2] CRC-32 Encoding" << endl;
    cout << "Enter choice: ";
    cin >> choice;

    cout << "Enter message to encode: ";
    cin.ignore(); // Ignore newline character left in buffer
    getline(cin, message);

    vector<string> binaryList = stringToBinaryList(message);
    
    cout << endl;
    
    if (choice == "1") {
        HammingEncoder hammingEncoder;
        vector<string> encodedList = hammingEncoder.encodeList(binaryList);
        
        cout << "Encoded message using Hamming: " << endl;
        for (const string& encodedStr : encodedList) {
            cout << encodedStr << endl;
        }
    } else if (choice == "2") {
        CRC32Encoder crc32Encoder;
        vector<string> encodedList = crc32Encoder.encodeList(binaryList);
        cout << "Encoded message using CRC-32: " << endl;
        for (const string& encodedStr : encodedList) {
            cout << encodedStr << endl;
        }
    } else {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}
