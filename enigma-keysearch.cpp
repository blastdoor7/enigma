#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <exception>
#include <limits>

//#include <chrono>
#include <time.h>

//template<typename TimeT = std::chrono::milliseconds>
//struct measure
//{
//  template<typename F, typename ...Args>
//  static typename TimeT::rep execution(F&& func, Args&&... args)
//  {
//    auto start = std::chrono::steady_clock::now();
//    std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
//    auto duration = std::chrono::duration_cast< TimeT >(std::chrono::steady_clock::now()  - start);
//    return duration.count();
//  }
//};


using namespace std;
const char* ROTORS[] = {
   //ABCDEFGHIJKLMNOPQRSTUVWXYZ
    "EKMFLGDQVZNTOWYHXUSPAIBRCJ<R",	//I
    "AJDKSIRUXBLHWTMCQGZNPYFVOE<F",	//II
    "BDFHJLCPRTXVZNYEIWGAKMUSQO<W",	//III
    "ESOVPZJAYQUIRHXLNFTGKDCMWB<K",	//IV
    "VZBRGITYUPSDNHLXAWMJQOFECK<A",	//V
    "JPGVOUMFYQBENHZRDKASXLICTW<AN",	//VI
    "NZJHGRCXMYSWBOUFAIVLPEKQDT<AN",	//VII
    "FKQHTLXOCBJSPDZRAMEWNIUYGV<AN"	//VIII
};

const char* ROTORS_FOURTH[] = {
    "LEYJVCNIXWPBQMDRTAKZGFUHOS",	//Beta
    "FSOKANUERHMBTIYCWLQPZXVGJD"	//Gamma
};

const char* REFLECTORS[] = {
    "AY BR CU DH EQ FS GL IP JX KN MO TZ VW",	//B
    "AF BV CP DJ EI GO HY KR LZ MX NW TQ SU",	//C
    "AE BN CK DQ FU GY HW IJ LO MP RX SZ TV",	//B Thin
    "AR BD CO EJ FN GT HK IV LM PW QZ SX UY"	//C Thin
};

const char LETTERS[] = { 'A', 'B', 'C', 'D', 'E', 
                          'F', 'G', 'H', 'I', 'J', 
                          'K', 'L', 'M', 'N', 'O',
                          'P', 'Q', 'R', 'S', 'T', 
                          'U', 'V', 'W', 'X', 'Y', 'Z' };

bool all_unique_uppercase(const string& s)
{
  for(int i=0; i<s.size(); i++)
  {
    if(!isupper(static_cast<unsigned char>(s[i])))
      return false;
  }

  set<char> u(s.begin(), s.end());
  return u.size() == s.size();
}

int Mod(int a, int m)
{
  int r = a % m;
  if(r < 0)
    r += m;

  return r;
}

///**
// * Map a letter to a number in 0..25.
// *
// * @param {char} c
// * @param {boolean} permissive - Case insensitive; don't throw errors on other chars.
// * @returns {number}
// */
int a2i(const char c) {
    //cout << "entered a2i arg char " << c << " numeric " << (int)c << endl;
    if (c >= 65 && c <= 90) {
        return c - 65;
    }
    else if(c == 32) 
      return -1;

    throw invalid_argument("a2i called on non-uppercase ASCII character");
}
//
///**
// * Map a number in 0..25 to a letter.
// *
// * @param {number} i
// * @returns {char}
// */
char i2a(const int i) {
    //cout << "i2a arg " << i << endl;
    if (i >= 0 && i < 26) {
        return (char)(i+65);
    }
    throw invalid_argument("i2a called on value outside 0..25");
}

void letterFrequency(const string& s)
{
  vector<double> freq(256, 0);

  for(unsigned i=0; i<s.size(); i++)
  {
    freq[s[i]]++;
  }

  //cout << "letter statisitics" << endl;
  //cout << "letter frequencies" << endl;
  for(unsigned i=0; i<freq.size(); i++)
  {
    if(freq[i] == 0) continue;
    //cout << "  " << (char)i << "   " << freq[i] << " times";
  }
  //cout << endl;
}

void rms(const string& s, string& r)
{
  for(int i=0; i<s.size(); i++)
  {
    if(::isspace(s[i])) continue;
    r+= s[i];
  }
}

string caesarshift(const string& s, int shift)
{
  string res;

  for(int i=0; i<s.size(); i++)
  {
	  char l = s[i];
	  l -= 65;
	  l+=shift;
	  l=l%26;
          res.push_back(l+65);
  }
  
  return res;
}
string text__ic = "";
vector<double> freq__ic(256,0);
double ic(const string& t)
{
  text__ic="";
  memset(freq__ic.data(), 0, freq__ic.size() * sizeof(freq__ic[0]));
  rms(t, text__ic);

  for(int i=0; i<text__ic.size(); i++)
  {
    if(text__ic[i] == ' ') continue;
    freq__ic[text__ic[i]] ++;
  }

  double sum=0;
  for(int i=0; i<freq__ic.size(); i++)
  {
    if(freq__ic[i] != 0)
    {
      double c = freq__ic[i];
      if(c != 0)
        sum += c * (c - 1);
    } 
  }
  double ic = 26 * sum / (text__ic.size() * (text__ic.size() - 1));
  int th_num=0;
  int the_num=0;
  for(size_t offset = t.find("TH"); offset != string::npos; offset = t.find("TH",offset+2))
	  th_num++;
  for(size_t offset = t.find("HE"); offset != string::npos; offset = t.find("HE",offset+2))
	  th_num++;
  for(size_t offset = t.find("IN"); offset != string::npos; offset = t.find("IN",offset+2))
	  th_num++;
  for(size_t offset = t.find("EN"); offset != string::npos; offset = t.find("EN",offset+2))
	  th_num++;
  for(size_t offset = t.find("IS"); offset != string::npos; offset = t.find("IS",offset+2))
	  th_num++;
  for(size_t offset = t.find("THE"); offset != string::npos; offset = t.find("THE",offset+2))
	  the_num++;
  for(size_t offset = t.find("AND"); offset != string::npos; offset = t.find("AND",offset+2))
	  the_num++;
  for(size_t offset = t.find("THA"); offset != string::npos; offset = t.find("THA",offset+2))
	  the_num++;
  for(size_t offset = t.find("ING"); offset != string::npos; offset = t.find("ING",offset+2))
	  the_num++;
  for(size_t offset = t.find("THI"); offset != string::npos; offset = t.find("ING",offset+2))
	  the_num++;

  float digramscore = th_num * 0.3;
  float trigramscore = the_num * 0.5;
  ic += digramscore;
  ic += trigramscore;

  return ic;
}

//
///**
// * A rotor in the Enigma machine.
// */
class Rotor {

  public:
    Rotor(string& wiring, string& steps)    
    {
        if (!all_unique_uppercase(wiring)) {
            throw invalid_argument("Rotor wiring must be 26 unique uppercase letters");
        }
        if (!all_unique_uppercase(steps)) {
            throw invalid_argument("Rotor steps must be 0-26 unique uppercase letters");
        }
        this->steps = steps;
        //if ('A' > ringSetting || ringSetting > 'Z') {
        //    throw invalid_argument("Rotor ring setting must be exactly one uppercase letter");
        //}
        //if ('A' > initialPosition || initialPosition > 'Z') {
        //    throw invalid_argument("Rotor initial position must be exactly one uppercase letter");
        //}
        set<int> uniq;
        for (unsigned i=0; i<26; i++) {
            const int a = a2i(LETTERS[i]);
            const int b = a2i(wiring[i]);
            this->map_[a] = b;
            this->revMap_[b] = a;
            uniq.insert(b);
        }
        if (uniq.size() != 26) {
            throw invalid_argument("Rotor wiring must have each letter exactly once");
        }

        //this->rs = a2i(ringSetting);
        //for(unsigned i=0; i<steps.size(); i++) {
        //    this->stepSet.insert(Mod((a2i(steps[i]) - rs) , 26));
        //}
        //if (this->stepSet.size() != steps.size()) {
        //    // This isn't strictly fatal, but it's probably a mistake
        //    throw invalid_argument("Rotor steps must be unique");
        //}
        //this->pos = Mod((a2i(initialPosition) - rs),  26);
        //cout << "Rotor initial pos " << pos << endl;
    }
    /**
     * Rotor constructor.
     *
     * @param {string} wiring - A 26 character string of the wiring order.
     * @param {string} steps - A 0..26 character string of stepping points.
     * @param {char} ringSetting - The ring setting.
     * @param {char} initialPosition - The initial position of the rotor.
     */
    Rotor(string& wiring, string& steps, char ringSetting, char initialPosition)    {
        if (!all_unique_uppercase(wiring)) {
            throw invalid_argument("Rotor wiring must be 26 unique uppercase letters");
        }
        if (!all_unique_uppercase(steps)) {
            throw invalid_argument("Rotor steps must be 0-26 unique uppercase letters");
        }
        if ('A' > ringSetting || ringSetting > 'Z') {
            throw invalid_argument("Rotor ring setting must be exactly one uppercase letter");
        }
        if ('A' > initialPosition || initialPosition > 'Z') {
            throw invalid_argument("Rotor initial position must be exactly one uppercase letter");
        }
        set<int> uniq;
        for (unsigned i=0; i<26; i++) {
            const int a = a2i(LETTERS[i]);
            const int b = a2i(wiring[i]);
            this->map_[a] = b;
            this->revMap_[b] = a;
            uniq.insert(b);
        }
        if (uniq.size() != 26) {
            throw invalid_argument("Rotor wiring must have each letter exactly once");
        }
        this->rs = a2i(ringSetting);
        for(unsigned i=0; i<steps.size(); i++) {
            this->stepSet.insert(Mod((a2i(steps[i]) - rs) , 26));
        }
        if (this->stepSet.size() != steps.size()) {
            // This isn't strictly fatal, but it's probably a mistake
            throw invalid_argument("Rotor steps must be unique");
        }
        this->pos = Mod((a2i(initialPosition) - rs),  26);
        //cout << "Rotor initial pos " << pos << endl;
        this->steps = steps;
    }

    void ringSetting(const char& ringSetting)
    {
      this->stepSet.clear();
      if ('A' > ringSetting || ringSetting > 'Z') {
        throw invalid_argument("Rotor ring setting must be exactly one uppercase letter");
      }
      this->rs = a2i(ringSetting);
      for(unsigned i=0; i<steps.size(); i++) {
          this->stepSet.insert(Mod((a2i(steps[i]) - rs) , 26));
      }
      if (this->stepSet.size() != steps.size()) {
          // This isn't strictly fatal, but it's probably a mistake
          throw invalid_argument("Rotor steps must be unique");
      }
    }

    void startPosition(const char& initialPosition)
    {
      if ('A' > initialPosition || initialPosition > 'Z') {
        throw invalid_argument("Rotor initial position must be exactly one uppercase letter");
      }
      this->pos = Mod((a2i(initialPosition) - rs),  26);
    }

    /**
     * Step the rotor forward by one.
     */
    int step() {
      //cout << "Rotor::step before pos " << pos << endl;
        this->pos = Mod((this->pos + 1), 26);
      //cout << "Rotor::step after pos " << pos << endl;
        return this->pos;
    }

    /**
     * Transform a character through this rotor forwards.
     *
     * @param {number} c - The character.
     * @returns {number}
     */
    int transform(int c) {
        //cout << "  TRANS input letter " << i2a(c) << endl;
        char perm = this->map_[Mod(c + this->pos, 26)];
        //cout << "  letter permuted to " << i2a(perm) << endl;
        //cout << "  permuted letter after offset " << i2a(Mod((perm - pos), 26)) << endl; 
        return Mod((perm - this->pos), 26);
    }

    /**
     * Transform a character through this rotor backwards.
     *
     * @param {number} c - The character.
     * @returns {number}
     */
    int revTransform(char c) {
        //cout << "  REVERSE TRANS input letter " << i2a(c) << endl;
        char perm = this->revMap_[Mod(c + this->pos, 26)];
        //cout << "  REVERSE letter permuted to " << i2a(perm) << endl;
        //cout << "  REVERSE permuted letter after offset " << i2a(Mod((perm - pos), 26)) << endl; 
        return Mod(perm - this->pos , 26);
    }

  Rotor& operator=(const Rotor& o)
  {
    steps = o.steps;
    pos   = o.pos;
    rs    = o.rs;
    stepSet = o.stepSet;
    map_ = o.map_;
    revMap_ = o.revMap_;
  }

  //private: 
    string steps;
    int pos;
    int rs;
    set<int> stepSet;
    map<int, int> map_;
    map<int, int> revMap_;
};
//
///**
// * Base class for plugboard and reflector (since these do effectively the same
// * thing).
// */
class PairMapBase {
  public:
   /**
     * PairMapBase constructor.
     *
     * @param {string} pairs - A whitespace separated string of letter pairs to swap.
     * @param {string} [name='PairMapBase'] - For errors, the name of this object.
     */
    PairMapBase(const string& pairs, const string& name="PairMapBase") {
        // I've chosen to make whitespace significant here to make a) code and
        // b) inputs easier to read
        this->pairs = pairs;
        if (pairs == "") {
            return;
        }
      
        //cout << "PairMapBase " << pairs << endl;  
        istringstream iss(pairs);
        string pair;
        while(getline(iss, pair, ' '))
        {
          //cout << "parsing pairs " << pair << endl;
          if(pair.size() != 2 && !all_unique_uppercase(pair))
          {
            throw invalid_argument(name + " must be a whitespace-separated list of uppercase letter pairs");
          }
          const int a = a2i(pair[0]), b = a2i(pair[1]);
          if (this->map_.count(a)) {
            throw invalid_argument(name + " connects " + pair[0] + " more than once");
          }
          if (this->map_.count(b)) {
            throw invalid_argument(name + " connects " + pair[1] + " more than once");
          }
          //cout << "PairMapBase set map " << a << " -> " << b << endl;
          //cout << "PairMapBase set map " << b << " -> " << a << endl;
          this->map_[a] = b;
          this->map_[b] = a;
        } 

      //cout << "PairMapBase map_ size " << this->map_.size() << endl;
    }

    /**
     * Transform a character through this object.
     * Returns other characters unchanged.
     *
     * @param {number} c - The character.
     * @returns {number}
     */
    int transform(int c) {
        if (!this->map_.count(c)) {
            return c;
        }
        return this->map_[c];
    }

    /**
     * Alias for transform, to allow interchangeable use with rotors.
     *
     * @param {number} c - The character.
     * @returns {number}
     */
    int revTransform(int c) {
        return this->transform(c);
    }
  protected:
    string pairs;
    map<int, int>  map_;
};

///**
// * Reflector. PairMapBase but requires that all characters are accounted for.
// *
// * Includes a couple of optimisations on that basis.
// */
class Reflector : public PairMapBase {
  public:
    /**
     * Reflector constructor. See PairMapBase.
     * Additional restriction: every character must be accounted for.
     */
    Reflector(const string& pairs) : PairMapBase(pairs, "Reflector") {
        this->pairs = pairs;
        if (this->map_.size() != 26) {
            throw invalid_argument("Reflector must have exactly 13 pairs covering every letter");
        }
        for(map<int, int>::iterator iter = this->map_.begin(); iter != this->map_.end(); iter++) {
            this->optMap_[iter->first] = this->map_[iter->first];
        }
        this->map_ = this->optMap_;
    }

    Reflector() : PairMapBase("","") {}
    /**
     * Transform a character through this object.
     *
     * @param {number} c - The character.
     * @returns {number}
     */
    int transform(int c) {
        return this->map_[c];
    }
  private:
    map<int, int> optMap_;
};

/**
 * Plugboard. Unmodified PairMapBase.
 */
class Plugboard : public PairMapBase {
  public:
    /**
     * Plugboard constructor. See PairMapbase.
     */
    Plugboard(const string& pairs) : PairMapBase(pairs, "Plugboard") {

    }
    Plugboard() : PairMapBase("","") {}
};

//
///**
// * Base class for the Enigma machine itself. Holds rotors, a reflector, and a plugboard.
// */
class EnigmaBase {
  public:
    /**
     * EnigmaBase constructor.
     *
     * @param {Object[]} rotors - List of Rotors.
     * @param {Object} reflector - A Reflector.
     * @param {Plugboard} plugboard - A Plugboard.
     */
    EnigmaBase(vector<Rotor>& rotors, const Reflector& reflector, const Plugboard& plugboard) {
    this->rotors = rotors;
    Rotor& RIGHT  = rotors[0];
    Rotor& MIDDLE = rotors[1];
    Rotor& LEFT   = rotors[2];
    this->rotorsRev.push_back(LEFT); 
    this->rotorsRev.push_back(MIDDLE); 
    this->rotorsRev.push_back(RIGHT); 
    this->reflector = reflector;
    this->plugboard = plugboard;
  }

    /**
     * Step the rotors forward by one.
     *
     * This happens before the output character is generated.
     *
     * Note that rotor 4, if it's there, never steps.
     *
     * Why is all the logic in EnigmaBase and not a nice neat method on
     * Rotor that knows when it should advance the next item?
     * Because the double stepping anomaly is a thing. tl;dr if the left rotor
     * should step the next time the middle rotor steps, the middle rotor will
     * immediately step.
     */
    void step() {
        Rotor& r0 = this->rotors[0];
        Rotor& r1 = this->rotors[1];
        //cout << "EnigmaBase Rotor 0 pos " << r0.pos << " Rotor 1 pos " << r1.pos << endl; 
        r0.step();
        //cout << "EnigmaBase after calling step Rotor 0 pos " << r0.pos << endl; 
        // The second test here is the double-stepping anomaly
        if (r0.stepSet.count(r0.pos) || r1.stepSet.count(Mod((r1.pos + 1) , 26))) {
            r1.step();
            if (r1.stepSet.count(r1.pos)) {
                Rotor& r2 = this->rotors[2];
                r2.step();
            }
        }
    }

    /**
     * Encrypt (or decrypt) some data.
     * Takes an arbitrary string and runs the Engima machine on that data from
     * *its current state*, and outputs the result. Non-alphabetic characters
     * are returned unchanged.
     *
     * @param {string} input - Data to encrypt.
     * @returns {string}
     */
    void crypt(const string& input, string& result) {
        //cout << "  crypt input " << input << endl;
        result = "";
        for(unsigned i=0; i<input.size(); i++) {
          //cout << "input letter " << input[i] << endl;
            int letter = a2i(input[i]);
            if (letter == -1) {
                result += input[i];
                continue;
            }
            // First, step the rotors forward.
            //for (unsigned i=0; i<this->rotors.size(); i++) {
            //  cout << "BEFORE STEP rotor " << i << " POS " << rotors[i].pos << endl;
            //}
            this->step();
            //for (unsigned i=0; i<this->rotors.size(); i++) {
              //cout << "AFTER STEP rotor " << i << " POS " << rotors[i].pos << endl;
            //}
            // Now, run through the plugboard.
            //cout << " before plugboard transform " << letter << endl;
            letter = this->plugboard.transform(letter);
            //cout << " plugboard transform " << letter << endl;
            // Then through each wheel in sequence, through the reflector, and
            // backwards through the wheels again.
            for (unsigned i=0; i<this->rotors.size(); i++) {
                //cout << "ROTOR vector index " << i << endl;
                //cout << "ROTOR pos " << rotors[i].pos << endl;
                //cout << "before rotor trans letter " << i2a(letter) << endl;
                letter = rotors[i].transform(letter);
                //cout << "after rotor trans letter " << i2a(letter) << endl;
            }
            letter = this->reflector.transform(letter);
            //cout << "after reflection letter " << i2a(letter) << endl;
            for (int i=2; i>=0; i--) {
                //cout << "REVERSE PATH ROTOR vector index " << i << endl;
                //cout << "REVERSE ROTOR pos " << rotors[i].pos << endl;
                //cout << "before rotor reverse trans letter " << i2a(letter) << endl;
                letter = rotors[i].revTransform(letter);
                //cout << "after rotor reverse trans letter " << i2a(letter) << endl;
            }
            // Finally, back through the plugboard.
            letter = this->plugboard.revTransform(letter);
            //cout << " i2a 1" << endl;
            result += i2a(letter);
        }
        //return result;
    }
   protected:
    vector<Rotor> rotors;
    vector<Rotor> rotorsRev;
    Reflector reflector;
    Plugboard plugboard;
};

/**
 * The Enigma machine itself. Holds 3-4 rotors, a reflector, and a plugboard.
 */
class EnigmaMachine : public EnigmaBase {
  public:
    /**
     * EnigmaMachine constructor.
     *
     * @param {Object[]} rotors - List of Rotors.
     * @param {Object} reflector - A Reflector.
     * @param {Plugboard} plugboard - A Plugboard.
     */
    EnigmaMachine(vector<Rotor>& rotors, Reflector& reflector, Plugboard& plugboard) : EnigmaBase(rotors, reflector, plugboard) {
        if (rotors.size() != 3 && rotors.size() != 4) {
            throw invalid_argument("Enigma must have 3 or 4 rotors");
        }
    }


};

bool present(const string& s, char c)
{
  for(int i=0; i<s.size(); i++)
  {
    if(s[i] == c) return true;
  }
  return false;
}

class rotorstats
{
  public:
    string rotorsel;
    float  best;
    string decrypt;
    string msgkey;
    string ringsetting;
};
int main(int argc, char** argv)
{
  time_t start;
  time(&start);

  vector<rotorstats> stats;
  
  string input;
  input = string(argv[1]);
  const string plugboardCfg   = 
"DN GR IS KC QX TM PV HY FW BJ";
  const string reflectorCfg      = "B";
  //403 ABC INPUTJHJHJH <PLUGS> <REF>
  cout << "input " << input << endl;
  cout << "plugs " << plugboardCfg << endl;
  cout << "reflector " << reflectorCfg << endl;

  int refid = -1;
  if(reflectorCfg == "B") refid = 0;
  if(reflectorCfg == "C") refid = 1;

  map<string, int> rotorNameMap;
  rotorNameMap["I"]    = 0; 
  rotorNameMap["II"]   = 1; 
  rotorNameMap["III"]  = 2; 
  rotorNameMap["IV"]   = 3; 
  rotorNameMap["V"]    = 4; 
  rotorNameMap["VI"]   = 5; 
  rotorNameMap["VII"]  = 6; 
  rotorNameMap["VIII"] = 7; 

  map<int, string> rotorIndexToNameMap;
  rotorIndexToNameMap[0] = "I"; 
  rotorIndexToNameMap[1] = "II"; 
  rotorIndexToNameMap[2] = "III"; 
  rotorIndexToNameMap[3] = "IV"; 
  rotorIndexToNameMap[4] = "V"; 
  rotorIndexToNameMap[5] = "VI"; 
  rotorIndexToNameMap[6] = "VII"; 
  rotorIndexToNameMap[7] = "VIII"; 

  vector<Rotor> rotors;
  istringstream iss("I II III IV V");

    //"EKMFLGDQVZNTOWYHXUSPAIBRCJ<R",	//I
    //"AJDKSIRUXBLHWTMCQGZNPYFVOE<F",	//II
    //"BDFHJLCPRTXVZNYEIWGAKMUSQO<W",	//III
    //"ESOVPZJAYQUIRHXLNFTGKDCMWB<K",	//IV
    //"VZBRGITYUPSDNHLXAWMJQOFECK<A",	//V


   string wI = "EKMFLGDQVZNTOWYHXUSPAIBRCJ";  string stepsI = "R";
   string wII = "AJDKSIRUXBLHWTMCQGZNPYFVOE";  string stepsII = "F";
   string wIII = "BDFHJLCPRTXVZNYEIWGAKMUSQO";  string stepsIII = "W";
   string wIV = "ESOVPZJAYQUIRHXLNFTGKDCMWB";  string stepsIV = "K";
   string wV = "VZBRGITYUPSDNHLXAWMJQOFECK";  string stepsV = "A";
   Rotor r1 = Rotor(wI, stepsI, 'Z', 'Z');
   Rotor r2 = Rotor(wII, stepsII, 'Z', 'Z');
   Rotor r3 = Rotor(wIII, stepsIII, 'Z', 'Z');
   Rotor r4 = Rotor(wIV, stepsIV, 'Z', 'Z');
   Rotor r5 = Rotor(wV, stepsV, 'Z', 'Z');
   vector<Rotor> rotorVec;
   rotorVec.push_back(r1);
   rotorVec.push_back(r2);
   rotorVec.push_back(r3);
   rotorVec.push_back(r4);
   rotorVec.push_back(r5);

  Reflector      reflector(REFLECTORS[refid]);
  Plugboard      plugboard(plugboardCfg);
  vector<Rotor> rotorSel;
  rotorSel.push_back(rotorVec[0]);
  rotorSel.push_back(rotorVec[1]);
  rotorSel.push_back(rotorVec[2]);

  string result = ""; 
  double bestIC=0;
  
  //for(unsigned ri=0; ri<5; ri++)
  int ri = 4; 
  {
  //for(unsigned rj=0; rj<5; rj++)
  int rj=1; 
	  {
    //if(ri == rj) continue;
  //for(unsigned rk=0; rk<5; rk++)
  int rk= 3; 
  {
   //if(rk == ri || rk == rj) continue;
//	    rotorstats rs;
  rotorSel[0] = rotorVec[ri];
  rotorSel[1] = rotorVec[rj];
  rotorSel[2] = rotorVec[rk]; 
  bestIC = 0;
  time_t start;
  time(&start);
  //for(unsigned ringSettingR=0; ringSettingR<26; ringSettingR++)
  {
  int ringSettingR='Y' - 'A';
  //for(unsigned ringSettingM=0; ringSettingM<26; ringSettingM++)
  int ringSettingM='M' - 'A';
  {
  //for(unsigned ringSettingL=0; ringSettingL<26; ringSettingL++)
  int ringSettingL='G' - 'A';
  {
    //cout << "begin message key scan " << endl;
    time_t start;
    time(&start);
    for(unsigned i=0; i<26; i++)
    //int i=4;
    {
      for(unsigned j=0; j<26; j++)
      //int j=1;
      {
        for(unsigned k=0; k<26; k++)
        //unsigned k=3;
        {
          rotorSel[0].ringSetting(LETTERS[ringSettingR]); rotorSel[0].startPosition(LETTERS[i]);
          rotorSel[1].ringSetting(LETTERS[ringSettingM]); rotorSel[1].startPosition(LETTERS[j]);
          rotorSel[2].ringSetting(LETTERS[ringSettingL]); rotorSel[2].startPosition(LETTERS[k]);
          rotorSel[2].startPosition(LETTERS[k]);
          Plugboard p(plugboardCfg);
          EnigmaMachine em = EnigmaMachine(rotorSel, reflector, p);
          em.crypt(input, result);
	  {
          double ic0 = ic(result);
	  //if(ic0 > bestIC)
          {
            //bestIC = ic0;  
            //cout << "new best " << ic0 << endl;
            //cout << "rotor order " << rotorIndexToNameMap[rk] << " " << rotorIndexToNameMap[rj] << " " << rotorIndexToNameMap[ri] << endl;
            string rotorsel = rotorIndexToNameMap[rk] + " " + rotorIndexToNameMap[rj] + " " + rotorIndexToNameMap[ri] ;
            string ringsetting;
	    ringsetting += LETTERS[ringSettingL];
	    ringsetting += LETTERS[ringSettingM];
	    ringsetting += LETTERS[ringSettingR];

            string msgkey;
	    msgkey += LETTERS[k];
	    msgkey +=  LETTERS[j];
	    msgkey += LETTERS[i] ;

	    //cout << rotorsel << endl;
	    //cout << ringsetting << endl;
	    //cout << msgkey << endl;
	    //cout << result << endl;
            if(result == "FLUGZ EUGFU EHRER ISTOF WYYXF UELLG RAFXF UELLG PAFXP OFOP") 
            {
	    cout << rotorsel << endl;
	    cout << ringsetting << endl;
	    cout << msgkey << endl;
	    cout << result << endl;
	      break;
            }
          }
	  }
        }
      }
    }
  time_t end;
  time(&end);
  double diff = difftime(end, start);
  //cout << "elapsed time for message key scan " << diff  << " seconds" << endl;
  }
  }
  time_t end;
  time(&end);
  double diff = difftime(end, start);
  //cout << "elapsed time for rotor order scan " << diff / 60 << " minutes" << endl;
  }
	    //stats.push_back(rs);
  }
  }
  }

  //cout << "stats" << endl;
  //for(int i=0; i<stats.size(); i++)
 // {
  //  rotorstats rs = stats[i];
   // cout << rs.rotorsel << endl;
 //   cout << rs.best << endl;
  //  cout << rs.ringsetting << endl;
   // cout << rs.msgkey << endl;
    //cout << rs.decrypt << endl;
 // }

  //}
  //}
  time_t end;
  time(&end);
  double diff = difftime(end, start);
  //cout << "elapsed time " << diff / 60 << " minutes" << endl;
  return 0;
}
//XZBIMAMGLZGGDXFVWFOEHQMCDBAGKCORXXOOJICHLYLBOIRGMJXKRTGFJMBNTHAIQWDLJHWBAGQQFLJBEYQAZYYRGWBHWBGNVKILZWNDYLEBETS
