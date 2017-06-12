/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Dictionary.h
 * Author: quanghuynh
 *
 * Created on June 1, 2017, 10:34 PM
 */

#include <iostream>
#include <fstream>

#ifndef DICTIONARY_H
#define DICTIONARY_H

#define INDEX_OFFSET_HEADER_SIZE 4
#define CONTENT_OFFSET_HEADER_SIZE 8
#define INDEX_ELEMENT_HEADER_SIZE 4

#define DEBUG 0

#define MAX_WORD_SIZE 1024
#define ELEMENT_SIZE MAX_WORD_SIZE + 12
using namespace std;
class WordEntry;

/*
 * Dictionary file format
 *  ----------------------------------------------------------------
 * |    0    |    1     |    2    | entry1 |  entry2 | ...| entryN  |
 *  ----------------------------------------------------------------
 * section O : 8bytes - tail - point to the last entry's position in file
 * section 1 : 8bytes - head - point to the first entry
 * section 2 : 8bytes - count - number of entry in dictionary.
 * Each entry is defined by WordEntry class
 */

class Dictionary {
public:
    
    inline static bool is_file_exist(string fileName){
        std::ifstream infile(fileName);
        return infile.good();
    }
    
    static Dictionary* Open(string dictName);
    
    explicit Dictionary(string name);
        virtual ~Dictionary();
        
        
    int AddWord(string word, string explaination);
    string LookUp(string word);
    
    
    void ReadContent();
    uint64_t GetTailValue();
    bool    UpdateTailValue(uint64_t tail);
    
    uint64_t GetHeadValue();
    bool    UpdateHeadValue(uint64_t head);
    
    uint64_t GetNumberOfEntries();
    bool    UpdateNumberOfEntries(uint64_t count);
private:
    int addWordEntry(WordEntry* entry);
    WordEntry* getWordEntry(string word);
    bool convertUint64ToCharArray(uint64_t number, char * array, int arraySize);
    uint64_t convertCharArrayToUint64(char* array, int arraySize);
private:
    /*
     // 
     */
    std::fstream file_;
    FILE* data_;
    string file_name_;
    uint32_t header_size_;//header contain index and content offset when loading a file.
    uint32_t index_offset_;//start offset of index section
    uint64_t index_size_;
    uint32_t index_element_size_;
    uint64_t content_offset_; //start offset of content
    uint64_t content_size_;
    
    uint64_t tail_pointer_;
    uint64_t head_pointer_;
    uint64_t num_entries_;
};


    /* 
     ----------------------------------------
    |   0   |   1   |    2     |      3      |
     ----------------------------------------
     * when write to file, each entry contains 4 fields :
     *  0 - 8 bytes, is the data length of field 3
     *  1 - 8 bytes, contain next entry address in file, 
     *  2 - word, 1024 bytes, fixed size
     *  3 - word's explanation, can be from 1 to 1GB long.
     */
class WordEntry {
public:
//    WordIndex();
//    WordIndex(string word);
    const static int kWordDataSize = 1024;
    
    WordEntry(string word, string explanation);
    ~WordEntry();
    
    uint64_t GetDataLength();
    uint64_t GetNextEntry();
    string GetWord();
    string GetExplanation();
    void SetExplaination(string ex);
    uint64_t GetTotalEntryDataSize();
private:

    

    
    uint64_t data_length_;
    uint64_t next_entry_;
    string word_; //can be from 1 to 1024 character long
    string explanation_; //size of word's explanation, max is 1GB ~ 2^30 bytes
};



#endif /* DICTIONARY_H */

