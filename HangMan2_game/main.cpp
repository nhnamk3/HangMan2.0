#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
using namespace std;
const char MASK_CHAR = '-';
int ReadLengthWord()
{
    int lengthWord;
    cout << "Enter the number characters of your secret word: ";
    cin >> lengthWord;
    return lengthWord;
}
int ReadMaxGuess()
{
    int maxGuess;
    cout << "Enter the number of incorrect guesses: ";
    cin >> maxGuess;
    return maxGuess;
}
vector<string> ReadVocabulary(const string& fileName)
{
    vector<string> vocabularyArr;
    ifstream vocabularyFile(fileName);
    while (!vocabularyFile.eof()){
        string word;
        vocabularyFile>>word;
        if (word != "") vocabularyArr.push_back(word);
    }
    return vocabularyArr;
}
void FilterWordsByLen(vector<string>& candidateWords, const int& lengthWord)
{
    for(int i=0; i<candidateWords.size(); i++){
        if (candidateWords[i].length() != lengthWord){
            candidateWords.erase(candidateWords.begin() + i);
            i--;
        }
    }
}
//Loc tu: Xoa nhung string co thu tu char khong giong guessWord
    //guessWord: tu dang doan den hien tai
void FilterWordsByGuessedChar(vector<string>& candidateWords, const string& guessWord)
{
    for(int i=0; i<candidateWords.size(); i++){
        for(int j=0; j<candidateWords[i].length(); j++){
            if (isalpha(guessWord[j])){
                if (guessWord[j] != candidateWords.at(i)[j]){
                    candidateWords.erase(candidateWords.begin() + i);
                    i--;
                }
            }
        }
    }
}
//Loc tu: xoa nhung string co chua ki tu khong dung
void 
FilterWordsByIncorrectChar(vector<string>& candidateWords, set<char> inCorrectChar)
{
    for(int i=0; i<candidateWords.size(); i++){
        for(char ch : candidateWords[i]){
            if (inCorrectChar.find(ch) != inCorrectChar.end()){
                candidateWords.erase(candidateWords.begin() + i);
                i--;
                break;
            }
        }
    }
}
//Cap nhat lai map theo candidateWords hien tai, cho nhung tu da sai = -1
map<char, int> GetMapChar(const vector<string>& candidateWords, set<char> inCorrectChar, set<char> CorrectChar)
{
    map<char, int> answer;
    for(string st : candidateWords){
        for(char ch : st){
            answer[ch]++;
        }
    }
    for(char ch : inCorrectChar){
        answer[ch] = -1;
    }
    for(char ch : CorrectChar){
        answer[ch] = -1;
    }
    return answer;
}
//Dua ra lua chon char xuat hien nhieu nhat
char BestChooseChar (map<char, int>& mapChar)
{
    int max = 0;
    char answer;
    for(auto mp : mapChar){
        if (mp.second > max){
            max = mp.second;
            answer = mp.first;
         }
    }
    return answer;
}
//Doc mask
string ReadMask(const char& guessChar)
{
    string mask;
    cout << "I guess your word have : " << guessChar<<endl;
    cout << "Please give me your answer: " ;
    cin >> mask;
    return mask;
}
//doc mask xem char vua nhap dung khong
bool IsCorrectChar(const string& mask, string& guessWord)
{
    bool answer = false;
    for(int i = 0; i<mask.length(); i++){
        if (mask[i] != MASK_CHAR){
            guessWord[i] = mask[i];
            answer = true;
        }
    }
    return answer;
}
//Kiem tra da thang hay chua qua guessWord
bool IsWon(const string& guessWord)
{
    for(char ch : guessWord){
        if (ch == MASK_CHAR)    return false;
    }
    cout<<"I WON !"<<endl;
    return true;
}
int main()
{
    //int maxGuess = ReadMaxGuess();
    int lengthWord = ReadLengthWord();
    string guessWord = string(lengthWord, MASK_CHAR);
    vector<string> vocabularyArr = ReadVocabulary("vocabulary.txt");
    FilterWordsByLen(vocabularyArr, lengthWord);
    set<char> inCorrectChar;
    set<char> CorrectChar;
    bool isQuit = false;
    int count = 0;
    while (!isQuit){
        map<char, int> mapChar = GetMapChar(vocabularyArr, inCorrectChar, CorrectChar);
        cout<<endl<<"Your word : "<<guessWord<<endl;
        char guessChar = BestChooseChar(mapChar);
        string mask = ReadMask(guessChar);

        if (!IsCorrectChar(mask, guessWord)){
            inCorrectChar.insert(guessChar);
            count++;
        }else CorrectChar.insert(guessChar);

        FilterWordsByGuessedChar(vocabularyArr, guessWord);
        FilterWordsByIncorrectChar(vocabularyArr, inCorrectChar);
        if (IsWon(guessWord))   isQuit = true;
    }
    return 0;
}