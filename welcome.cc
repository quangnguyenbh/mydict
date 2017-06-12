/*
 * Copyright (c) 2009-2010, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Oracle nor the names of its contributors
 *   may be used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>

#include "Dictionary.h"
using namespace std;

void printHelp() {
    cout << "Please use dict [command] " << endl;
    cout <<"  CREATE \"dname\" : create a dictionary named 'dname' " << endl;
    cout <<"  OPEN \"dname\" : open a dictionary named 'dname' " << endl;
    cout <<"  ADD \"word\" \"explanation\" : add an entry to opened dictionary with word and explanation" << endl;
    cout <<"  MODIFY \"word\" \"explanation\" : modify word with new explanation" << endl;
    cout <<"  DELETE \"word\" : delete word from opened dictionary" << endl;
    cout <<"  LOOKUP \"word\" : lookup a word from opened dictionary" << endl;
}

static Dictionary * currentDic_ = NULL;
bool CreateDictionary(string name);
bool OpenDictionary(string name);
bool AddEntry(Dictionary* dic, string word, string expl);
bool ModifyEntry(Dictionary* dic, string word, string newExpl);
bool DeleteEntry(Dictionary* dic, string word);
string LookUp(Dictionary* dic, string word);

int main(int argc, char**argv) {
    // Prints welcome message...

    if (argc < 2) {
        printHelp();
        return -1;
    }
    cout << "argc = " <<argc << endl;
    // Prints arguments...
    if (argc > 1) {
        string command(argv[1]);
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);
        if(command == "create"){
            if(argc != 3){
                printHelp();
                return -1;
            }
            string dicname(argv[2]);
            CreateDictionary(dicname);
        }
        if(command == "open"){
            
        }
        if(command == "add"){
        
        }
        if(command == "delete"){
        
        }
        if(command == "modify"){
            
        }
        
        if(command == "lookup"){
            
        }
    }
    


    Dictionary * dic = new Dictionary("dict1");

    dic->AddWord("schoool", "a place where children go to be educated");
    dic->AddWord("Member", "a person, animal, or thing that is part of a group");
    
    string s = dic->LookUp("Member");
    cout << "Lookup result " << s  << endl;
    string m = dic->LookUp("School");
    cout << "Lookup result : " << m  << endl;
    
    dic->AddWord("consciousness", "the state of understanding and realizing something");
    cout << "look up consciousness: " << dic->LookUp("consciousness") << endl;
    
    Dictionary * dic2 = new Dictionary("dict2");
    dic2->AddWord("product advertising", "the act of advertising a particular product or service, rather than a company name, etc");
    cout << "Look up product advertising: " << dic2->LookUp("product advertising") << endl;
    
    return 0;
}


bool CreateDictionary(string name){
    currentDic_ = new Dictionary(name);
    return true;
}

