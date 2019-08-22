/**
 * Program to split CSV file into smaller chunks.
 * Run the program like this: split [input.csv]
 **/


// Define number of columns, and amount of rows to allocate per file.
// #define NUM_COL 48
// #define ROWS_PER_FILE 100
#define FILE_PREFIX "output_"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    if(argc < 2) {
        cout << "You need to input a CSV file as the first argument. Run Split with '--help' for more info." << endl << endl;
        exit(1);
    }
    int rows_per_file = 100;
    if(argc > 2) {
        rows_per_file = stoi(argv[2]);
    }
    string arg_1 = argv[1];
    if(arg_1 == "--help") {
        cout << "Program to split a CSV file into smaller chunks." << endl;
        cout << "Split can be run by defining the amount of rows to allocate per file as an argument." << endl;
        cout << "The amount of rows to allocate will default to 100 if not defined." << endl;
        cout << "Syntax to run Split is as follows:\n" << endl;
        cout << "\tsplit filename [number of rows per file]" << endl << endl;
        exit(0);
    }

    ifstream fin(argv[1]);
    if( !fin.good() ) {
        cout << "There was a problem opening the file. Terminating." << endl;
        exit(1);
    }
    string col_names = "";
    getline(fin, col_names);

    string input_filename = argv[1];
    string file_prefix;
    if(input_filename.find('.') != string::npos) {
        file_prefix = input_filename.substr(0, input_filename.find('.'));
    } else {
        file_prefix = input_filename;
    }

    int num_col = 1;
    for(int i = 0; i < col_names.length(); i++) {
        char current = col_names[i];
        bool string_open = false;
        if(current == '\"') {
            if(string_open) {
                string_open = false;
            } else {
                string_open = true;
            }
        } else if(!string_open) {
            if(current == ',') {
                num_col++;
            }
        }
    }

    int rows_processed = 0;
    int file_suffix = 1;
    while( !fin.eof() ) {
        string output_file = file_prefix + "_" + to_string(file_suffix) + ".csv";
        ofstream fout(output_file);
        fout << col_names << endl;
        for(int i = 0; i < rows_per_file; i++) {
            int current_col = 1;
            string row = "";
            bool eol = false;
            bool string_open = false;
            while(!eol) {
                char current = fin.get();
                if( fin.eof() ) break;
                if(current == '\"') {
                    if(string_open) {
                        string_open = false;
                    } else {
                        string_open = true;
                    }
                } else if(!string_open) {
                    if(current == ',') {
                        current_col++;
                    }
                }
                row += current;
                if(current_col == num_col && current == '\n') eol = true;
            }
            if( fin.eof() ) break;
            fout << row;
            rows_processed++;
        }
        fout.close();
        cout << "Finished file " << file_suffix << endl;
        file_suffix++;
    }

    fin.close();

    cout << "\nThe input file has been split into " << (file_suffix - 1) << " chunks." << endl;
    cout << rows_processed << " rows have been processed.\n" << endl;

    return 0;
}