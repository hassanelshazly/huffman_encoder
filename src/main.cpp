/******************************************************************************
 * Copyright (C) 2020 by Hassan El-shazly
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright.
 *
 *****************************************************************************/
/**
  *
  * @author Hassan El-shazly
  * @date Last Edit Aug-2020
  *
  */
#include <iostream>
#include <string>
#include <fstream>

#include "huffman.h"

using namespace std;
// using experimental::filesystem;

void print_inst();
bool exists(const string &path);

int main(int argc, char **argv)
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

    if (mood != "-e" && mood != "-d" && mood != "--encode" && mood != "--decode")
    {
        cout << "exit_status(1): you must provide \"-e\" or \"-d\" argument !" << endl;
        print_inst();
        return 1;
    }

    if (!exists(input))
    {
        cout << "exit_status(2): couldn't open \"" << input << "\"" << endl;
        return 2;
    }

    if (argc == 3)
        if (input != "huffman_out.txt")
            output = "huffman_out.txt";
        else
            output = "huffman_output.txt";
    else
        output = argv[3];

    if (input == output)
    {
        cout << "exit_status(5): you can't overwrite the input file" << endl;
        return 5;
    }

    if (exists(output))
    {
        cout << "\"" << output << "\" already exists, the contents will be overwitten\n"
             << "Do you want to continue [yes/no]: " << endl;
        string response;
        cin >> response;
        if (response != "y" && response != "yes")
        {
            cout << "process terminated" << endl;
            return 0;
        }      
    }

    ifstream input_file(input, std::ios::in | std::ios::binary);
    if (!input_file.is_open())
    {
        cout << "exit_status(2): couldn't open \"" << input << "\"" << endl;
        return 2;
    }

    ofstream output_file(output, std::ios::out | std::ios::binary);
    if (!output_file.is_open())
    {
        cout << "exit_status(2): couldn't open \"" << output << "\"" << endl;
        return 2;
    }

    if (mood == "-e" || mood == "--encode")
    {
        try
        {
            float ratio = huff.encode(input_file, output_file);
            cout << "Compression ratio: " << ratio * 100 << "%" << endl;
        }
        catch (const char *ex)
        {
            cout << "exit_status(3): Empty file \"" << input << "\"" << endl;
        }
    }
    else if (mood == "-d" || mood == "--decode")
    {
        try
        {
            huff.decode(input_file, output_file);
        }
        catch (const char *ex)
        {
            cout << "exit_status(4): \"" << input << "\""
                 << " is not vaild for decoding\n"
                 << "                 the file must be encoded with the same program " << endl;
        }
    }

    input_file.close();
    output_file.close();
}

void print_inst()
{
    cout << "huffman encoder\n\n";
    cout << "Usage: ./huffman.out [-e] [--encode] [-d] [--decode] <input_file> [output_file]\n\n";
    cout << "if the <output_file> is not provided the default will be \"huffman_out.txt\"\n\n";
    cout << "  -e --encode  encode the <input_file> an store in the <output_file>\n";
    cout << "  -d --decode  decode the <input_file> an store in the <output_file>\n";

    cout << endl;
}

bool exists(const string &path)
{
    std::ifstream f(path.c_str());
    return f.good();
}