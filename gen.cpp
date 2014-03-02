#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <functional> 
#include <locale> 

using namespace std;

string consonants("pfgcrldhtnsqjkxbmwvz");
string vovels("aoeuiy");
bool consonant[128];
bool vovel[128];

// trim from start
static inline string &ltrim(string &s) {
        s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
        return s;
}

// trim from end
static inline string &rtrim(string &s) {
        s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline string &trim(string &s) {
        return ltrim(rtrim(s));
}

string get_file_contents(const char *filename) {
  ifstream in(filename, ios::in | ios::binary);
  if (in)
  {
    string contents;
    in.seekg(0, ios::end);
    contents.resize(in.tellg());
    in.seekg(0, ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}

size_t E;
const char* S;

int cmp(const void* a, const void* b) {
  const int A = *(const int*)a;
  const int B = *(const int*)b;
  return memcmp(S + A, S + B, E - max(A, B));
}

bool comp(const int A, const int B) {
  return memcmp(S + A, S + B, E - max(A, B)) < 0;
}

vector<int> compute_suffix_array(const string& str) {
  const char* data = str.c_str();
  const size_t size = str.size();

  S = data;
  E = size;
  
  vector<int> index(size);
  for(int i = 0; i < size; ++i) index[i] = i;
  
  qsort(&index[0], size, sizeof(int), cmp);
  //sort(index.begin(), index.end(), comp);
  return index;
}

string gen_name(string& buffer, vector<int>& suffix_array, vector<int>& suffix_array_index) {
  string name(" ");

  auto compA = [&](const int A, const int B)->bool {
    const int result = memcmp(&buffer[A], &name[B], min(buffer.size() - A, name.size() - B));
    //cerr << "Comparing left " << A << "(" << buffer[A] << ") with " << B << "(" << name[B] << "): " << result << endl;
    return result < 0;
  };


  auto compB = [&](const int A, const int B)->bool {
    const int result = memcmp(&name[A], &buffer[B], min(name.size() - A, buffer.size() - B));
    //cerr << "Comparing right " << A << "(" << name[A] << ") with " << B << "(" << buffer[B] << "): " << result << endl;
    return result < 0;
  };

  auto next_char = [&]()->char { 
    for(int i = 0; i < name.size(); ++i) {
      int a = lower_bound(suffix_array.begin(), suffix_array.end(), i, compA) - suffix_array.begin();
      int b = upper_bound(suffix_array.begin(), suffix_array.end(), i, compB) - suffix_array.begin();
      int d = b - a;
      if(name[name.size() - 1] & 0x80) { // inside utf8 characters, just go with it
        return buffer[suffix_array[a] + name.size() - i];
      }
      //cerr << "Found range: " << a << " - " << b << endl;
      if(d <= 1) continue;
      if(buffer[suffix_array[a]  + name.size() - i] == buffer[suffix_array[b - 1] + name.size() - i]) continue;
      //if((i+1 < name.size()) && (rand() % 2)) continue;
      int c = a + rand() % d;
      return buffer[suffix_array[c] + name.size() - i];
    }
    return '#'; 
  };

  int failed = 0;
  while(true) {
    ++failed;
    if(failed >= 1000) {
      name = " ";
      failed = 0;
    }
    int s = name.size();
    char c = next_char();
    if(c == ' ') {
      if(s < 4) continue; // at least 4 letters
      else break;
    }
    /*
    if(c == name[s-1]) continue; // no repeats
    if(s >= 2 && consonant[c] && consonant[name[s-1]] && consonant[name[s-2]]) continue; // no triple consonants
    if(s >= 1 && vovel[c] && vovel[name[s-1]]) continue; // no diphtongs
    */
    failed = 0;
    name += c;
    if(name.size() > 15) break;
  }
  name = trim(name);
  return name;
}

int main(int argc, char* argv[]) {
  srand(time(0));
  if(argc < 2) {
    cerr << "Specify file to analyze" << endl;
    return 1;
  }

  for(int i = 0; i < consonants.size(); ++i) {
    consonant[consonants[i]] = true;
  }
  for(int i = 0; i < vovels.size(); ++i) {
    vovel[vovels[i]] = true;
  }


  cerr << "Reading " << argv[1] << "..." << endl;
  string buffer = get_file_contents(argv[1]);
  cerr << "Reading done." << endl;

  cerr << "Sorting characters..." << endl;
  vector<int> suffix_array = compute_suffix_array(buffer);
  vector<int> suffix_array_index(suffix_array.size());
  for(int i = 0; i < suffix_array.size(); ++i) suffix_array_index[suffix_array[i]] = i;
  cerr << "Sorting done." << endl;


  cerr << "Generating names..." << endl;
  int width = 0;
  for(int i = 0; i < 40;) {
    string name = gen_name(buffer, suffix_array, suffix_array_index);
    int pos = buffer.find(name);
    if(pos >= 0)
      continue;
    width += name.size();
    if(width >= 80) {
      width = name.size();
      cout << endl;
      ++i;
    }
    name[0] = toupper(name[0]);
    cout << name << ' ';
  }
  cerr << "Generating done." << endl;
  
}
