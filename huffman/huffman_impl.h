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
#ifndef _HUFFMAN_IMPL_H_
#define _HUFFMAN_IMPL_H_

#include <fstream>
#include <string>
#include <queue>
#include <cmath>

#include "tree.h"

using std::greater;
using std::pair;
using std::priority_queue;

#define NUM_BITS 8
#define PSEU_EOF 0
#define SIGN (char)0xAA

huffman::huffman()
    : text("")
{
    // do nthing
}

float huffman::encode(ifstream &input_file, ofstream &output_file)
{
     
    read_file(input_file);
    if (text == "")
        throw "huffman::encode -> Empty file";

    vector<int> freqs = compute_freqs();
    tree_node *root = generate_tree(freqs);
    map<char, string> codes = generate_codes(root);
    float ratio = (float)file_size(freqs, codes) / text.size();
    
    store_sign(output_file);
    store_tree(output_file, root);
    encode_file(output_file, codes);

    // release resources 
    read_bit(input_file, 1);
    write_bit(output_file, 0, 1);
    del_tree(root);
    text.clear();

    return ratio;
}

void huffman::decode(ifstream &input_file, ofstream &output_file)
{
    if (!verify_sign(input_file))
        throw "huffman::decode -> file not valid";
        
    tree_node *root = read_tree(input_file);
    decode_file(input_file, output_file, root);

    // release resources 
    read_bit(input_file, 1);
    del_tree(root);
}

void huffman::read_file(ifstream &input_file)
{
    input_file.seekg(0, std::ios::end);
    text.resize(input_file.tellg());
    input_file.seekg(0, std::ios::beg);
    input_file.read(&text[0], text.size());
}

vector<int> huffman::compute_freqs()
{
    vector<int> freqs(256, 0);
    // pseudo PEOF
    freqs[PSEU_EOF] = 1;
    for (const auto &ch : text)
        freqs[(uint8_t)ch]++;
    return freqs;
}

tree_node *huffman::generate_tree(vector<int>& freqs)
{
    priority_queue<pair<int, tree_node *>,
        vector<pair<int, tree_node *>>,
        greater<pair<int, tree_node *>>> pq;

    // create leaf nodes
    for (int i = 0; i < 256; i++)
    {
        if (freqs[i] != 0)
            pq.push({ freqs[i], new tree_node(i) });
    }

    // make huffman tree
    while (pq.size() > 1)
    {
        pair<int, tree_node*> item1 = pq.top();
        pq.pop();
        pair<int, tree_node*> item2 = pq.top();
        pq.pop();
        tree_node *holder = new tree_node(item1.second, item2.second, 0);
        pq.push({ item1.first + item2.first, holder });
    }
    return pq.top().second;
}

void huffman::store_tree(ofstream &output_file, tree_node *root)
{
    if (root == nullptr)
        return;

    if (root->is_leaf())
    {
        write_bit(output_file, 1);
        write_byte(output_file, root->get_data());
        return;
    }
    write_bit(output_file, 0);
    store_tree(output_file, root->get_left());
    store_tree(output_file, root->get_right());
}

void traverse(tree_node *root, string str, map<char, string> &codes)
{
    if (root == nullptr)
        return;

    if (root->is_leaf())
        codes.insert({ root->get_data(), str });

    traverse(root->get_left(), str + "0", codes);
    traverse(root->get_right(), str + "1", codes);
}

map<char, string> huffman::generate_codes(tree_node *root)
{
    map<char, string> codes;
    traverse(root, "", codes);
    return codes;
}

uint64_t huffman::file_size(vector<int>& freqs,map<char, string> &codes)
{
    uint64_t fsize = 0;
    for (int i = 0; i < freqs.size(); i++) {
        fsize += freqs[i] * codes[(char)i].length();
    }
    fsize += 10 * codes.size() -1;
    return ceil((double)fsize / 8);
}

void huffman::encode_file(ofstream &output_file, map<char, string> &codes)
{
    for (const auto &ch : text)
        for (const auto &bit : codes[ch])
            write_bit(output_file, bit - '0');
    // PEOF
    for (const auto &bit : codes[PSEU_EOF])
        write_bit(output_file, bit - '0');
}

tree_node *huffman::read_tree(ifstream &input_file)
{
    if (read_bit(input_file))
    {
        char byte = read_byte(input_file);
            return new tree_node(byte);
    }
    else
    {
        tree_node *left = read_tree(input_file);
        tree_node *right = read_tree(input_file);
        return new tree_node(left, right, 0);
    }
}

void huffman::decode_file(ifstream &input_file, ofstream &output_file, tree_node *root)
{
    tree_node *curr = root;
    while (true)
    {
        if (read_bit(input_file))
            curr = curr->get_right();
        else
            curr = curr->get_left();

        if (curr->is_leaf())
        {
            if (curr->get_data() == PSEU_EOF)
                break;
            char data = curr->get_data();
            output_file.write(&data, 1);
            curr = root;
        }
    }
}

bool huffman::read_bit(ifstream &input_file, bool final)
{
    static char bitcount = NUM_BITS;
    static char byte = 0;
    if (final)
    {
        bitcount = NUM_BITS;
        byte = 0;
        return 0;
    }

    if (bitcount == NUM_BITS)
    {
        input_file.read(&byte, 1);
        bitcount = 0;
    }

    bool bit = (byte << bitcount) & 0x80;
    bitcount++;

    return bit;
}

char huffman::read_byte(ifstream &input_file)
{
    char byte = 0;
    for (int i = 0; i < NUM_BITS; i++)
        byte = (byte << 1) | read_bit(input_file);
    return byte;
}

void huffman::write_bit(ofstream &output_file, bool bit, bool final)
{
    static char bitcount = 0;
    static char byte = 0;
    if (final && bitcount)
    {
        byte <<= NUM_BITS - bitcount;
        output_file.write(&byte, 1);
    }
    byte = (byte << 1) | bit;
    bitcount++;
    if (bitcount == NUM_BITS)
    {
        output_file.write(&byte, 1);
        bitcount = byte = 0;
    }
}
void huffman::write_byte(ofstream &output_file, char byte)
{
    for (int i = 0; i < NUM_BITS; i++)
    {
        write_bit(output_file, byte & 0x80);
        byte <<= 1;
    }
}

bool huffman::verify_sign(ifstream &input_file)
{
    if (read_byte(input_file) == SIGN)
        return true;
    return false;
}

void huffman::store_sign(ofstream &output_file)
{
    write_byte(output_file, SIGN);
}

#endif // End of the file