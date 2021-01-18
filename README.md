# Huffman Encoder

Simple compression / decompression tool using standard huffman algorithm

# Getting Started
```
$ git clone https://https://github.com/hassanelshazly/huffman_encoder

$ cd huffman_encoder

$ make
```

## Usage
```
./huffman.out <mood> <input_file> [output_file]
```

`mood` can be either `-e` `--encode` for compression or `-d` `--decode` for decompression

if the `output_file` is not provided the default will be `"huffman_out.txt"` or `"huffman_output.txt"`

## Help
```
Usage: ./huffman.out [-e] [--encode] [-d] [--decode] <input_file> [output_file]

if the <output_file> is not provided the default will be \"huffman_out.txt

  -e --encode  encode the <input_file> an store in the <output_file>
  -d --decode  decode the <input_file> an store in the <output_file>
```

## License

This project is licensed under MIT License
