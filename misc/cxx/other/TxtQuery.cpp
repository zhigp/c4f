#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstddef>
#include <locale>
using namespace std;

class TxtQuery{
public:
    TxtQuery(){ memset(this, 0, sizeof(TxtQuery)); }
    ~TxtQuery(){
        delete _txtlines;
        delete _words;
        delete _locations;
        delete _wordmap;
    }
    static void FilterElements(string felms){ _filtelms=felms; }
    void QueryTxt();
    void DisplayMapTxt();
    void DisplayTxtLocations();
    void DoIt(){
        RetrieveTxt();
        SeparateWords();
        FilterTxt();
        SuffixTxt();
        StripCaps();
        BuildWordMap();
        }

private:
    void RetrieveTxt();
    void SeparateWords();
    void FilterTxt();
    void SuffixS(string &);
    void SuffixTxt();
    void StripCaps();
    void BuildWordMap();

private:
    static string                                   _filtelms;
    vector<string>                                  *_txtlines;
    vector<string>                                  *_words;
    vector< pair<short, short> >                    *_locations;
    map< string, vector< pair<short,short> >* >     *_wordmap;
};

string TxtQuery::_filtelms("\",.;:!?)(\\/");

int main(){
    TxtQuery tq;
    tq.DoIt();
    tq.QueryTxt();
    tq.DisplayMapTxt();
}


void TxtQuery::RetrieveTxt(){
    string filename;
    cout << "\nplease enter file name: ";
    cin >> filename;

    ifstream infile(filename.c_str(), ios::in);
    if(!infile){
        cerr << "\n-->oops! unable to open file " << filename << "   -- exiting!!!" << endl;
        exit(-1);
    }
    
    _txtlines = new vector<string>;
    string line;
    while(getline(infile, line, '\n'))
      _txtlines -> push_back(line);
}

void TxtQuery::SeparateWords(){
    _words = new vector<string>;
    _locations = new vector< pair<short, short> >;
    for(short linepos=0; linepos < _txtlines->size(); linepos++){
        short wordpos=0;
        string tmpline=(*_txtlines)[linepos];
        string::size_type eol = tmpline.length();
        string::size_type pos = 0, ppos = 0;

        while((pos = tmpline.find_first_of(' ', pos)) != string::npos){
            _words->push_back(tmpline.substr(ppos, pos-ppos));
            _locations->push_back(make_pair(linepos, wordpos));
            wordpos++; pos++; ppos = pos;
        }

        _words->push_back(tmpline.substr(ppos, pos-ppos));
        _locations->push_back(make_pair(linepos, wordpos));
    }
}

void TxtQuery::FilterTxt(){
    if(_filtelms.empty())
      return;
    
    vector<string>::iterator it = _words->begin();
    vector<string>::iterator ite = _words->end();

    while(it != ite){
        string::size_type pos = 0;
        while((pos = (*it).find_first_of(_filtelms, pos)) != string::npos)
          (*it).erase(pos,1);
        it++;
    }
}

void TxtQuery::SuffixTxt(){
    vector<string>::iterator it = _words->begin();
    vector<string>::iterator ite = _words->end();
    while(it != ite){
        if((*it).size() <= 3){ it++; continue; }
        if((*it)[(*it).size()-1] == 's')
          SuffixS(*it);
        it++;
    }
}

void TxtQuery::SuffixS(string &word){
    string::size_type spos = 0;
    string::size_type pos3 = word.size() - 3;

    //"ous", "ss", "is", "ius"
    string suffixes("oussisius");
    if(!word.compare(pos3, 3, suffixes, spos, 3) ||
       !word.compare(pos3, 3, suffixes, spos+6, 3) ||
       !word.compare(pos3+1, 2, suffixes, spos+2, 2) ||
       !word.compare(pos3+1, 2, suffixes, spos+4, 2))
        return;

    string ies("ies");
    if(!word.compare(pos3, 3, ies)){
        word.replace(pos3, 3, 1, 'y');
        return;
    }
    
    string ses("ses");
    if(!word.compare(pos3, 3, ses)){
        word.erase(pos3+1, 2);
        return;
    }

    word.erase(pos3+2);
    // for 's end
    if(word[pos3+1] == '\'')
      word.erase(pos3+1);
}

void TxtQuery::StripCaps(){
    vector<string>::iterator it = _words->begin();
    vector<string>::iterator ite = _words->end();

    string caps("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    while(it != ite){
        string::size_type pos = 0;
        while((pos=(*it).find_first_of(caps, pos)) != string::npos)
          (*it)[pos] = tolower((*it)[pos]);
        it++;
    }
}

void TxtQuery::BuildWordMap(){
    _wordmap = new map< string, vector< pair<short, short> >* >;
    set<string> exclusion_set;
    ifstream infile("exclusion_set");
    if(!infile){
        static string default_exclusion_words[25]={
            "the","and","but","that","then","are","been","can","can't","cannot","could","did","for",
            "had","have","him","his","her","its","into","were","which","when","with","would"
        };

    cerr << "\n---> warning! unable to open word exclusion file! --using default set;" << endl;
    copy(default_exclusion_words, default_exclusion_words+25,inserter(exclusion_set,exclusion_set.begin()));
    }
    else{
        istream_iterator<string> input_set(infile), eos;
        copy(input_set, eos, inserter(exclusion_set, exclusion_set.begin()));
    }

    int words_cnt = _words->size();
    string tmpword;
    for(int i=0; i<words_cnt; i++){
        tmpword = (*_words)[i];
        if(tmpword.size() < 3 || exclusion_set.count(tmpword))
          continue;
        if(!_wordmap->count(tmpword)){
            vector< pair<short,short> > *ploc = new vector< pair<short,short> >;
            ploc->push_back((*_locations)[i]);
            _wordmap->insert(map< string, vector< pair<short,short> >* >::value_type(tmpword,ploc));
        }
        else
            (*_wordmap)[tmpword]->push_back((*_locations)[i]);
    }
}

void TxtQuery::QueryTxt(){
    string querytxt;
    do{
        cout << "\nenter a word against which to search the text,\nto quit, enter a single character ==> ";
        cin >> querytxt;
        if(querytxt.size() < 2) break;

        string caps("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        string::size_type pos = 0;
        while((pos = querytxt.find_first_of(caps, pos)) != string::npos)
            querytxt[pos] = tolower(querytxt[pos]);

        if(!_wordmap->count(querytxt)){
            cout << "\nsorry, there are no entries for " << querytxt << ".\n";
            continue;
        }

        vector< pair<short,short> > *ploc = (*_wordmap)[querytxt];
        set<short> occurrence_lines;
        vector< pair<short,short> >::iterator it = ploc->begin(),
                                            ite = ploc->end();
        while(it != ite){
            occurrence_lines.insert(occurrence_lines.end(), (*it).first);
            it++;
        }

        int size = occurrence_lines.size();
        cout << endl << '\"' << querytxt << "\" occurs " << size << " time(s):" << endl;
        set<short>::iterator its = occurrence_lines.begin();
        for( ; its != occurrence_lines.end(); its++){
            int line = *its;
            cout << "\t(line " << line+1 << "):\t" << (*_txtlines)[line] << endl;
        }
    }
    while(!querytxt.empty());
    cout << "ok, bye!" << endl;
}

void TxtQuery::DisplayMapTxt(){
    map< string, vector< pair<short,short> >* >::iterator it = _wordmap->begin(), ite = _wordmap->end();

    while(it != ite){
        cout << "word: " << setiosflags(ios::left) << setw(40) << "\""+(*it).first+"\":" << "[";
        int loc_cnt = 0;
        vector< pair<short,short> > *locs = (*it).second;
        vector< pair<short,short> >::iterator lit = locs->begin(), lite = locs->end();

        while(lit != lite){
            if(loc_cnt) cout << ',';
            else        loc_cnt++;
            cout << "(" << (*lit).first << "," << (*lit).second << ")";
            lit++;
        }
    cout << "]\n";
    it++;
    }
}

void TxtQuery::DisplayTxtLocations(){
    int elem_cnt = _words->size();
    if(elem_cnt != _locations->size()){
        cerr << "-->oops! internal error: word and position vectors are of unequal size:\n"
             << "words: " << elem_cnt << " "
             << "locs: " << _locations->size()
             << "   -- exiting!!!\n";
        exit(-2);
    }

    for(int i=0; i<elem_cnt; i++){
        cout << "words: " << (*_words)[i] << "\t"
             << "locations: (" << (*_locations)[i].first << ',' << (*_locations)[i].second << ')' << endl;
    }

    cout << endl;
}
