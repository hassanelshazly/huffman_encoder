#include <iostream>

#include "huffman.h"

using namespace std;

void print_inst();

int main(int argc, char** argv)
{
    string mood;
    string input;
    string output;
    huffman huff;

    if (argc < 3)
    {
        print_inst();
        return 0;
    }
    mood = argv[1];
    input = argv[2];

    if (argc == 3)
        output = "out.txt";
    else
        output = argv[3];

    if (mood != "-e" && mood != "-d")
    {
        cout << "exit_statue(1): you must provide \"-e\" or \"-d\" argument !" << endl;
        print_inst();
        return 1;
    }

    ifstream input_file(input, std::ios::in | std::ios::binary);
    if (!input_file.is_open())
        cout << "exit_statue(2): couldn't open " << input << endl;

    ofstream output_file(output, std::ios::out | std::ios::binary);
    if (!input_file.is_open())
        cout << "exit_statue(2): couldn't open " << output << endl;

    if (mood ==  "-e")
    {
        try
        {
            float ratio = huff.encode(input_file, output_file);
            cout << "Compression ratio: " << ratio*100 << "%" << endl;
        }
        catch (const char * ex) {
            cout << "exit_statue(3): Empty file" << output << endl;
        }
    }
    else if (mood ==  "-d")
    {
        try 
        {
            huff.decode(input_file, output_file);
        }
        catch (const char * ex)
        {
            cout << "exit_statue(4): file is not vaild" << output << endl;
        }
    }

    input_file.close();
    output_file.close();
}

void print_inst() {
    cout << "huffman encoder\n";
    cout << "Usage: ./huffman [mode] [input_file] [output_file]\n";
    cout << "mode: -e --> for encoding";
    cout << "      -d --> for decoding";
    cout << endl;
}