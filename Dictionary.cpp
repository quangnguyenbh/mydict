/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Dictionary.cpp
 * Author: quanghuynh
 * 
 * Created on June 1, 2017, 10:34 PM
 */

#include "Dictionary.h"

Dictionary* Dictionary::Open(string dictName){

    return new Dictionary(dictName);
}

Dictionary::Dictionary(string name) : data_(NULL) {
    if (name.empty()) {
        name = "default";
    }
    file_name_.assign(name);
    file_name_.append(".dict");
    cout << "Opening a file name : " << file_name_ << endl;
    //    file_.open(file_name_, ios::binary);
    //    if(file_.is_open()){
    //        //first 4 bytes contain index offset value, default 12
    //        //next 8bytes contains content start offset value
    //        uint32_t indexOffset = 12;
    //        uint64_t contentOffset = 12;//not updated yet
    //        file_.write((char*)&indexOffset, INDEX_OFFSET_HEADER_SIZE);
    //        file_.write((char*)&contentOffset, CONTENT_OFFSET_HEADER_SIZE);
    //        file_.close();
    //    }
    if(is_file_exist(file_name_)){
        cout << "File already exist";
        return;
    }
    data_ = fopen(file_name_.c_str(), "w+b");
    if (data_) {
        uint64_t tail = 3 * sizeof (uint64_t);
        uint64_t head = 3 * sizeof (uint64_t);
        uint64_t count = 0;

        fwrite((char*) &tail, sizeof (char), sizeof (uint64_t), data_);
        fwrite((char*) &head, sizeof (char), sizeof (uint64_t), data_);
        fwrite((char*) &count, sizeof (char), sizeof (uint64_t), data_);
        fclose(data_);
        data_ = NULL;
        
        tail_pointer_ = tail;
        head_pointer_ = head;
        num_entries_ = count;
        
    } else {
        cout << "Error opening file " << endl;
    }


}

void Dictionary::ReadContent() {

    data_ = fopen(file_name_.c_str(), "rb");
    if (data_ == NULL) {
        cout << "Could not read file";
        return;
    }
    long lSize;
    fseek(data_, 0, SEEK_END);
    lSize = ftell(data_);
    rewind(data_);

    char index[INDEX_OFFSET_HEADER_SIZE];
    char content[CONTENT_OFFSET_HEADER_SIZE];
    char element[INDEX_ELEMENT_HEADER_SIZE];

    int s = fread(&index[0], 1, INDEX_OFFSET_HEADER_SIZE, data_);
    cout << "total byte read = " << s << endl;
    uint32_t num = 0;
    for (int i = 0; i < INDEX_OFFSET_HEADER_SIZE; ++i) {
        //        cout << i << ": " << (uint32_t)(unsigned char)(index[i]) << ", ";
        num += ((uint32_t) (unsigned char) index[i]) << (i * 8); // |= could have also been used
    }
    //    cout << endl;
    //    unsigned char uchar = (unsigned char)index[0];
    //    unsigned int uint1 = (unsigned int) uchar ;
    //    cout << "usigned int = " << uint1 << endl;
    //    for(int i = 0; i < 8; i++){
    //        cout << "bit " << i << ":" << ((uchar >> i) & 1) << ", "  << endl;
    //    }


    cout << "Num = " << num << endl;
    index_offset_ = num;

    s = fread(&content[0], 1, CONTENT_OFFSET_HEADER_SIZE, data_);
    cout << "Content  byte read = " << s << endl;
    uint64_t num2 = 0;
    for (int i = 0; i < CONTENT_OFFSET_HEADER_SIZE; i++) {
        num2 += ((uint64_t) (unsigned char) content[i]) << (i * 8);
    }
    cout << "Content offset = " << num2 << endl;
    content_offset_ = num2;

    if (content_offset_ < index_offset_) {
        cout << "There something wrong reading file " << endl;
        exit(0);
    }
    index_size_ = content_offset_ - index_offset_;

    //read index element size
    s = fread(&element[0], 1, INDEX_ELEMENT_HEADER_SIZE, data_);

    num = 0;
    for (int i = 0; i < INDEX_ELEMENT_HEADER_SIZE; ++i) {
        //        cout << i << ": " << (uint32_t)(unsigned char)(index[i]) << ", ";
        num += ((uint32_t) (unsigned char) element[i]) << (i * 8); // |= could have also been used
    }
    index_element_size_ = num;
    cout << "Index element size = " << num << endl;
    cout << "Index size = " << index_size_ << endl;
    //    int indexInt = 7;
    //    sscanf(index, "%u", &indexInt);
    //    cout << "File size = " << lSize << endl;
    fclose(data_);
}

uint64_t Dictionary::GetTailValue() {
    data_ = fopen(file_name_.c_str(), "rb");
    if (data_ == NULL) {
        cout << "Could not read file";
        return 0;
    }

    //read the first 8 bytes and parse it's value
    char tailValue[8];
    int s = fread(&tailValue[0], 1, 8, data_);

    //parse data
    int num = 0;
    if (s > 0) {
        num = convertCharArrayToUint64(tailValue, 8);
    }
    fclose(data_);
    data_ = NULL;
    return num;

}

bool Dictionary::UpdateTailValue(uint64_t tail) {
    data_ = fopen(file_name_.c_str(), "r+b");
    if (data_ == NULL) {
        cout << "Could not read file";
        return 0;
    }
#if DEBUG
      cout << "Update tail value to := " << tail << endl;
#endif 
    
    char t[8];
    convertUint64ToCharArray(tail, t, 8);

    rewind(data_);
    fwrite(&t[0], 1, 8, data_ );
    fclose(data_);
    data_ = NULL;
    tail_pointer_ = tail;
    return true;
}

uint64_t Dictionary::GetHeadValue() {
        data_ = fopen(file_name_.c_str(), "rb");
    if (data_ == NULL) {
        cout << "Could not read file";
        return 0;
    }

    //read the first 8 bytes and parse it's value
    fseek(data_, 8, SEEK_SET);
    char headValue[8];
    int s = fread(&headValue[0], 1, 8, data_);

    //parse data
    int num = 0;
    if (s > 0) {
        num = convertCharArrayToUint64(headValue, 8);
    }
    fclose(data_);
    data_ = NULL;
    return num;
}

bool Dictionary::UpdateHeadValue(uint64_t head) {
    data_ = fopen(file_name_.c_str(), "r+b");
    if (data_ == NULL) {
        cout << "Could not read file";
        return 0;
    }
    char t[8];
    this->convertUint64ToCharArray(head, t, 8);

    fseek(data_, 8, SEEK_SET);
    fwrite(&t[0], 1, 8, data_ );
    fclose(data_);
    data_ = NULL;
    head_pointer_ = head;
    return true;
}

uint64_t Dictionary::GetNumberOfEntries() {
       data_ = fopen(file_name_.c_str(), "rb");
    if (data_ == NULL) {
        cout << "Could not read file";
        return 0;
    }

    //read the first 8 bytes and parse it's value
    fseek(data_, 16, SEEK_SET);
    char count[8];
    int s = fread(&count[0], 1, 8, data_);
//    cout <<"Get entries read = " << s << endl;
    //parse data
    int num = 0;
    if (s > 0) {
        num = convertCharArrayToUint64(count, 8);
    }
    fclose(data_);
    data_ = NULL;
    return num;
}

bool Dictionary::UpdateNumberOfEntries(uint64_t count) {
    data_ = fopen(file_name_.c_str(), "r+b");
    if (data_ == NULL) {
        cout << "Could not read file";
        return 0;
    }
    char t[8];
    this->convertUint64ToCharArray(count, t, 8);

    fseek(data_, 16, SEEK_SET);
    fwrite(&t[0], 1, 8, data_ );
    fclose(data_);
    data_ = NULL;
    num_entries_ = count;
    return true;
}


int Dictionary::AddWord(string word, string explaination) {
    
    if(word.size() == 0 || explaination.size() == 0){
        return -1;
    }
    WordEntry* entry = new WordEntry(word, explaination);
    //append word to the end of file and update tail value + count
    
    this->addWordEntry(entry);
    
    delete entry;
    
    return 0;
}

string Dictionary::LookUp(string word) {
    
    if(word.size() == 0){
        return "";
    }
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    
    WordEntry * entry = this->getWordEntry(word);
    
    if(entry == NULL){
        return "";
    }
    string ret ;
    ret.assign(entry->GetExplanation());
    delete entry;
    
    return ret;
}

int Dictionary::addWordEntry(WordEntry* entry) {
    
    if(entry == NULL){
        return -1;
    }
    
    uint64_t tailValue = this->GetTailValue();
    uint64_t headValue = this->GetHeadValue();
    uint64_t countValue = this->GetNumberOfEntries();
    
    //open file for write
    data_ = fopen(file_name_.c_str(), "r+b");
    if (data_ == NULL) {
        cout << "Could not read file";
        return 0;
    }
    
    //get the size of file
    long lSize;
    fseek(data_, 0, SEEK_END);
    lSize = ftell(data_);
    
    uint64_t dataSize = entry->GetDataLength(); //data length of explanation
    char t[8];
    convertUint64ToCharArray(dataSize, &t[0], 8);
//    cout << entry->GetWord() << ", Size = " << convertCharArrayToUint64(t, 8) << endl;
    
    
    //write first element to file
    fwrite(&t[0], 1, 8, data_ );
    
    //set the value of next entry to zero
    convertUint64ToCharArray(0, t, 8);
    fwrite(&t[0], 1, 8, data_ );
    
    //write word of 1024 bytes
    char word[1024];
    entry->GetWord().copy(word, 1024, 0);
    word[entry->GetWord().size()] = '\0';
    fwrite(&word[0], 1, 1024, data_ );
    
    //write explanation
    const char * expla = entry->GetExplanation().c_str();
    fwrite(expla, 1, dataSize, data_);
    
    uint64_t entryLength = entry->GetTotalEntryDataSize();
//    cout << "Entry data length :" << entryLength << endl;
    
    //get current wordEntry and update the next value
    if(tailValue >= lSize){ //this dic has no entry
//        cout << "Add first entry: " << entry->GetWord() << ", tail = " << tailValue << ", lSize = " << lSize << endl;
    }else{
        //Get the last entry
//        cout << "Update previous entry's next pointer to : " << lSize << ", tail = " << tailValue << endl;
        fseek(data_, tailValue + 8, SEEK_SET);
        //read 8 bytes contain next info
        char next[8];
        this->convertUint64ToCharArray((uint64_t)lSize, next, 8);
        fwrite(next, 1, 8, data_);
    }

    fclose(data_);
    data_ = NULL;
    //update other counter, number of entries and tail value
    countValue += 1;
    tailValue = (uint64_t)lSize;
    this->UpdateTailValue(tailValue);
    this->UpdateNumberOfEntries(countValue);
    
    return 0;
}

WordEntry* Dictionary::getWordEntry(string word){
    
    
    uint64_t headValue = this->GetHeadValue();
    uint64_t tailValue = this->GetTailValue();
    uint64_t entries = this->GetNumberOfEntries();
    
    
    data_ = fopen(file_name_.c_str(), "rb");
    if (data_ == NULL) {
        cout << "Could not read file";
        return 0;
    }
        
//    cout << "Look up " << word << ", current head = " << headValue << ", tail = " << tailValue << ", entries = " << entries << endl;
    long lSize;
    
    fseek(data_, 0, SEEK_END) ;
    lSize = ftell(data_);
    rewind(data_);
#if DEBUG
    cout << "lsize = " << lSize << endl;
#endif
    if(tailValue >= lSize){
        return NULL;
    }
    fseek(data_, headValue, SEEK_SET);
    //read first 8 byte to get the data length of first entry
    bool endOfFile = false;
    bool foundEntry = false;
    WordEntry * entry = NULL;
    while(!endOfFile && !foundEntry){
        long curPos = ftell(data_);
        char len[8];
        int s = fread(&len[0], 1, 8, data_);

    //    cout << "read = " << s << endl;
        uint64_t dataLength = convertCharArrayToUint64(len, 8);
//        cout << "Data lengh at position " << curPos << " is : " << dataLength << endl;

        s = fread(&len[0], 1, 8, data_);
        uint64_t nextEntry = convertCharArrayToUint64(len, 8);
//        cout << "Next entry = " << nextEntry << endl;
        char wordChar[1024];
        s = fread(&wordChar[0], 1, 1024, data_);
        string w(wordChar);
#if DEBUG
      cout << "Word read := " << w << " size = " << w.size() << ",lokkup word = " << word << ",size = " << word.size() << endl;
#endif  

        if(w == word){
//            cout << "found word = " << w << endl;
            entry = new WordEntry(word, "");
            //read explanation 
            char* ex = new char[dataLength + 1];
            ex[dataLength] = '\0';
            s = fread(ex, 1, dataLength, data_);
            string expl(ex);
            entry->SetExplaination(expl);
            delete[] ex;
            foundEntry = true;
        }else{
            if(nextEntry <= 0){
                endOfFile = true;
                break;
            }
            fseek(data_, dataLength, SEEK_CUR);
            
        }
       
    }
    fflush(data_);
    fclose(data_); data_ = NULL;
    
    
    return entry;
}

bool Dictionary::convertUint64ToCharArray(uint64_t number, char * array, int arraySize){
    if(array == NULL)
        return false;
    
    for(int i = 0; i < 8; i++){
        array[7-i] = (char) ((number >> (7 - i)*8) & 0xFF);
    }
    return true;
}

uint64_t Dictionary::convertCharArrayToUint64(char* array, int arraySize){
    uint64_t num = 0;
            for (int i = 0; i < arraySize; i++) {
            num += ((uint64_t) (unsigned char) array[i]) << (i * 8);
        }
    return num;
}

Dictionary::~Dictionary() {
    if (file_.is_open()) {
        file_.close();
    }
}


///

//WordIndex::WordIndex() {
//}
//
//WordIndex::WordIndex(string word) {
//    word_.assign(word);
//    word_data_offset_ = 0;
//    word_data_size_ = 0;
//
//}

WordEntry::WordEntry(string word, string explanation) {
    
    //convert word to lower case first
    
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    
    word_.assign(word);
    explanation_.assign(explanation);
    data_length_ = explanation.size();
    next_entry_ = 0;
}

WordEntry::~WordEntry() {
}

string WordEntry::GetWord() {
    return word_;
}

string WordEntry::GetExplanation() {
    return explanation_;
}
void WordEntry::SetExplaination(string ex){
    explanation_.assign(ex);
}
uint64_t WordEntry::GetDataLength() {
    return data_length_;
}

uint64_t WordEntry::GetNextEntry() {
    return next_entry_;
}

uint64_t WordEntry::GetTotalEntryDataSize(){
    return 2*sizeof(uint64_t) + kWordDataSize + explanation_.size();
}
