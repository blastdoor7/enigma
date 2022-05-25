/*******************************************************************************
* The MIT License (MIT)                                                        
*                                                         
* Copyright (c) 2022 blastdoor7 [51786421+blastdoor7@users.noreply.github.com]
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the Software), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*******************************************************************************/
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
#include <assert.h>
#include <time.h>

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
//
///**
// * A rotor in the Enigma machine.
// */
class Rotor {

  public:
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
    }

    Rotor(const string& wiring, const string& steps)    {
      this->steps = steps;
        if (!all_unique_uppercase(wiring)) {
            throw invalid_argument("Rotor wiring must be 26 unique uppercase letters");
        }
        if (!all_unique_uppercase(steps)) {
            throw invalid_argument("Rotor steps must be 0-26 unique uppercase letters");
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
    }

    void ringSetting(char ringSetting)
    {
        this->rs = a2i(ringSetting);
        this->stepSet.clear();
        for(unsigned i=0; i<steps.size(); i++) {
            this->stepSet.insert(Mod((a2i(steps[i]) - rs) , 26));
        }
        if (this->stepSet.size() != steps.size()) {
            // This isn't strictly fatal, but it's probably a mistake
            throw invalid_argument("Rotor steps must be unique");
        }
    }

    void windowSetting(char initialPosition)
    {
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
     this->steps = o.steps;
     this->pos = o.pos;
     this->rs = o.rs;
     this->stepSet = o.stepSet;
     this->map_ = o.map_;
     this->revMap_ = o.revMap_;
     return *this;
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

    Plugboard& operator=(const Plugboard& o)
    {
      this->pairs = o.pairs;
      this->map_ = o.map_;
      return *this;
    }
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
  void conf(vector<Rotor>& rotors, const Reflector& reflector, const Plugboard& plugboard) {
    this->rotors[0] = rotors[0];
    this->rotors[1] = rotors[1];
    this->rotors[2] = rotors[2];
    this->rotorsRev[0] = rotors[2]; 
    this->rotorsRev[1] = rotors[1]; 
    this->rotorsRev[2] = rotors[0]; 
    //this->reflector = reflector;
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
    void crypt(char* input, char* result, unsigned LEN) {
        //cout << "  crypt input " << input << endl;
        for(unsigned ii=0; ii<LEN; ii++) {
          //cout << "input letter " << input[i] << endl;
            int letter = a2i(input[ii]);
            if (letter == -1) {
                result[ii] = input[ii];
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
            result[ii] = i2a(letter);
        }
    }
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

int main(int argc, char** argv)
{
  {
	  cout << "running test 1 ..." << endl;
    const string rotorSelection = "IV V III";
    const string ringSetting    = "X R V";
    const string messageKey     = "M P Y";
    const string plugboardCfg   = "SY EK NZ OR CG JM QU PV BI LW";
    const string reflectorCfg      = "B";
    const string  ciphertext = 

  "UFJZS NKIRA CGTPF UONXD GQMPU QXUGF OWEZS TCBJD"
  "JLFME AZQRM NZZYI CGSSR YOFQX ADSPU QIMXM MELYR"
  "XKXYI MDEEW ISKDP RSTFR TCOKB GGQTQ KPKMP NCCGH"
  "YUVJO TIVMA IVIGK WQKWJ FOYMR VFBVY RKEZF SYCBY"
  "QQSOQ CIZUU SUTB";
  
    map<string, int> rotorNameMap;
    rotorNameMap["I"]    = 0; 
    rotorNameMap["II"]   = 1; 
    rotorNameMap["III"]  = 2; 
    rotorNameMap["IV"]   = 3; 
    rotorNameMap["V"]    = 4; 
    rotorNameMap["VI"]   = 5; 
    rotorNameMap["VII"]  = 6; 
    rotorNameMap["VIII"] = 7; 
  
   
    vector<char> ringSettingVec; 
    istringstream issRingSetting(ringSetting);
    string rs;
    while(getline(issRingSetting, rs, ' '))
    {
      ringSettingVec.push_back(rs[0]);
    }
  
  
    vector<char> messageKeyVec;
    istringstream issMessageKeyVec(messageKey);
    string wp;
    while(getline(issMessageKeyVec, wp, ' '))
    {
      messageKeyVec.push_back(wp[0]);
    }
  
  
    vector<Rotor> rotorsRev;
    istringstream iss(rotorSelection);
    int k=0;
    string rotorName;
    while(getline(iss, rotorName, ' '))
    {
      if(rotorNameMap.count(rotorName) == 0)
        throw invalid_argument("selected rotor name incorrect must be I...VIII");
  
      string rotorCfg = ROTORS[rotorNameMap[rotorName]];
      istringstream ss(rotorCfg);
      string token;
      string rotorCfgArray[2]; int i=0;
      while(getline(ss, token, '<'))
      {
        rotorCfgArray[i] = token; i++;
      }
      
      string rotorWiring = rotorCfgArray[0];
      string rotorSteps  = rotorCfgArray[1];
      Rotor r = Rotor(rotorWiring, rotorSteps, ringSettingVec[k], messageKeyVec[k]);
      rotorsRev.push_back(r);
      k++; 
    }
  
    Reflector      reflector(REFLECTORS[0]);
    Plugboard      plugboard(plugboardCfg);
    string input;
    if(argc == 2) input = string(argv[1]);
    else input = ciphertext;
  
    vector<Rotor> rotors;
    rotors.push_back(rotorsRev[2]);
    rotors.push_back(rotorsRev[1]);
    rotors.push_back(rotorsRev[0]);
    string result;
    EnigmaMachine(rotors, reflector, plugboard).crypt(input, result);
    cout << "test input " << input << endl;  
    cout << "result " << result << endl;  
  
    string test1_decrypted = "ANOKH XDRIN GENDX MELDE XAUFF INDUN GXZAH LREICHERXM ENSCH LICHE RXUEB ERRES TEXZW OELFX KMXWESTLIC HXSMO LENSK XGROE SSERE XAUSM ASSEX MOGLICHXTA USEND EYWIE XVERF AHREX ICHXW EITER YOBERSTXFE LDPOL IZEI";
    assert(result == test1_decrypted);
  
    cout << "test 1 success" << endl;
  }  
  {
	  cout << "running test 2 ..." << endl;
    const string rotorSelection = "IV II V";
    const string ringSetting    = "G M Y";
    const string messageKey     = "D H O";
    const string plugboardCfg   = "DN GR IS KC QX TM PV HY FW BJ";
    const string reflectorCfg      = "B";
    const string  ciphertext = 
	    "GXS";
//"NQVLT YQFSE WWGJZ GQHVS EIXIM YKCNW IEBMB ATPPZ TDVCU PKAY";

    map<string, int> rotorNameMap;
    rotorNameMap["I"]    = 0; 
    rotorNameMap["II"]   = 1; 
    rotorNameMap["III"]  = 2; 
    rotorNameMap["IV"]   = 3; 
    rotorNameMap["V"]    = 4; 
    rotorNameMap["VI"]   = 5; 
    rotorNameMap["VII"]  = 6; 
    rotorNameMap["VIII"] = 7; 
  
   
    vector<char> ringSettingVec; 
    istringstream issRingSetting(ringSetting);
    string rs;
    while(getline(issRingSetting, rs, ' '))
    {
      ringSettingVec.push_back(rs[0]);
    }
  
  
    vector<char> messageKeyVec;
    istringstream issMessageKeyVec(messageKey);
    string wp;
    while(getline(issMessageKeyVec, wp, ' '))
    {
      messageKeyVec.push_back(wp[0]);
    }
  
  
    vector<Rotor> rotorsRev;
    istringstream iss(rotorSelection);
    int k=0;
    string rotorName;
    while(getline(iss, rotorName, ' '))
    {
      if(rotorNameMap.count(rotorName) == 0)
        throw invalid_argument("selected rotor name incorrect must be I...VIII");
  
      string rotorCfg = ROTORS[rotorNameMap[rotorName]];
      istringstream ss(rotorCfg);
      string token;
      string rotorCfgArray[2]; int i=0;
      while(getline(ss, token, '<'))
      {
        rotorCfgArray[i] = token; i++;
      }
      
      string rotorWiring = rotorCfgArray[0];
      string rotorSteps  = rotorCfgArray[1];
      Rotor r = Rotor(rotorWiring, rotorSteps, ringSettingVec[k], messageKeyVec[k]);
      rotorsRev.push_back(r);
      k++; 
    }
  
    Reflector      reflector(REFLECTORS[0]);
    Plugboard      plugboard(plugboardCfg);
    string input;
    if(argc == 2) input = string(argv[1]);
    else input = ciphertext;
  
    vector<Rotor> rotors;
    rotors.push_back(rotorsRev[2]);
    rotors.push_back(rotorsRev[1]);
    rotors.push_back(rotorsRev[0]);
    string result;
    EnigmaMachine(rotors, reflector, plugboard).crypt(input, result);
    cout << "test input " << input << endl;  
    cout << "result " << result << endl;  
  
    assert(result == "RLP");
  
    cout << "test 2 success" << endl;
  }  
  {
	  cout << "running test 3 ..." << endl;
    const string rotorSelection = "IV II V";
    const string ringSetting    = "G M Y";
    const string messageKey     = "R L P";
    const string plugboardCfg   = "DN GR IS KC QX TM PV HY FW BJ";
    const string reflectorCfg      = "B";
    const string  ciphertext = 
      "NQVLT YQFSE WWGJZ GQHVS EIXIM YKCNW IEBMB ATPPZ TDVCU PKAY";

    map<string, int> rotorNameMap;
    rotorNameMap["I"]    = 0; 
    rotorNameMap["II"]   = 1; 
    rotorNameMap["III"]  = 2; 
    rotorNameMap["IV"]   = 3; 
    rotorNameMap["V"]    = 4; 
    rotorNameMap["VI"]   = 5; 
    rotorNameMap["VII"]  = 6; 
    rotorNameMap["VIII"] = 7; 
  
   
    vector<char> ringSettingVec; 
    istringstream issRingSetting(ringSetting);
    string rs;
    while(getline(issRingSetting, rs, ' '))
    {
      ringSettingVec.push_back(rs[0]);
    }
  
  
    vector<char> messageKeyVec;
    istringstream issMessageKeyVec(messageKey);
    string wp;
    while(getline(issMessageKeyVec, wp, ' '))
    {
      messageKeyVec.push_back(wp[0]);
    }
  
  
    vector<Rotor> rotorsRev;
    istringstream iss(rotorSelection);
    int k=0;
    string rotorName;
    while(getline(iss, rotorName, ' '))
    {
      if(rotorNameMap.count(rotorName) == 0)
        throw invalid_argument("selected rotor name incorrect must be I...VIII");
  
      string rotorCfg = ROTORS[rotorNameMap[rotorName]];
      istringstream ss(rotorCfg);
      string token;
      string rotorCfgArray[2]; int i=0;
      while(getline(ss, token, '<'))
      {
        rotorCfgArray[i] = token; i++;
      }
      
      string rotorWiring = rotorCfgArray[0];
      string rotorSteps  = rotorCfgArray[1];
      Rotor r = Rotor(rotorWiring, rotorSteps, ringSettingVec[k], messageKeyVec[k]);
      rotorsRev.push_back(r);
      k++; 
    }
  
    Reflector      reflector(REFLECTORS[0]);
    Plugboard      plugboard(plugboardCfg);
    string input;
    if(argc == 2) input = string(argv[1]);
    else input = ciphertext;
  
    vector<Rotor> rotors;
    rotors.push_back(rotorsRev[2]);
    rotors.push_back(rotorsRev[1]);
    rotors.push_back(rotorsRev[0]);
    string result;
    EnigmaMachine(rotors, reflector, plugboard).crypt(input, result);
    cout << "test input " << input << endl;  
    cout << "result " << result << endl;  
  
    string test1_decrypted =
"FLUGZ EUGFU EHRER ISTOF WYYXF UELLG RAFXF UELLG PAFXP OFOP";
    assert(result == test1_decrypted);
  
    cout << "test 3 success" << endl;
    //return 0;
  }  
  {
	  cout << "generating test encryption  ..." << endl;
    const string rotorSelection = "IV V III";
    const string ringSetting    = "X R V";
    const string messageKey     = "M P Y";
    const string plugboardCfg   = "";
    const string reflectorCfg      = "B";
    const string  ciphertext = 

  "HEREISATESTTEXTFORDECRYPTIONTESTING";
  
    map<string, int> rotorNameMap;
    rotorNameMap["I"]    = 0; 
    rotorNameMap["II"]   = 1; 
    rotorNameMap["III"]  = 2; 
    rotorNameMap["IV"]   = 3; 
    rotorNameMap["V"]    = 4; 
    rotorNameMap["VI"]   = 5; 
    rotorNameMap["VII"]  = 6; 
    rotorNameMap["VIII"] = 7; 
  
   
    vector<char> ringSettingVec; 
    istringstream issRingSetting(ringSetting);
    string rs;
    while(getline(issRingSetting, rs, ' '))
    {
      ringSettingVec.push_back(rs[0]);
    }
  
  
    vector<char> messageKeyVec;
    istringstream issMessageKeyVec(messageKey);
    string wp;
    while(getline(issMessageKeyVec, wp, ' '))
    {
      messageKeyVec.push_back(wp[0]);
    }
  
  
    vector<Rotor> rotorsRev;
    istringstream iss(rotorSelection);
    int k=0;
    string rotorName;
    while(getline(iss, rotorName, ' '))
    {
      if(rotorNameMap.count(rotorName) == 0)
        throw invalid_argument("selected rotor name incorrect must be I...VIII");
  
      string rotorCfg = ROTORS[rotorNameMap[rotorName]];
      istringstream ss(rotorCfg);
      string token;
      string rotorCfgArray[2]; int i=0;
      while(getline(ss, token, '<'))
      {
        rotorCfgArray[i] = token; i++;
      }
      
      string rotorWiring = rotorCfgArray[0];
      string rotorSteps  = rotorCfgArray[1];
      Rotor r = Rotor(rotorWiring, rotorSteps, ringSettingVec[k], messageKeyVec[k]);
      rotorsRev.push_back(r);
      k++; 
    }
  
    Reflector      reflector(REFLECTORS[0]);
    Plugboard      plugboard(plugboardCfg);
    string input;
    if(argc == 2) input = string(argv[1]);
    else input = ciphertext;
  
    vector<Rotor> rotors;
    rotors.push_back(rotorsRev[2]);
    rotors.push_back(rotorsRev[1]);
    rotors.push_back(rotorsRev[0]);
    string result;
    EnigmaMachine(rotors, reflector, plugboard).crypt(input, result);
    cout << "test input " << input << endl;  
    cout << "result " << result << endl;  
  
    string test1_decrypted = "ILMNOTZFGHNEFHAPDSXJXWUMBLCJMQWKQAS";
    assert(result == test1_decrypted);
  
    cout << "sample decrypted successfully" << endl;
  }  
  {
	  cout << "generating test encryption single character" << endl;
    const string rotorSelection = "V IV III";
    const string ringSetting    = "C B A";
    const string messageKey     = "A A A";
    const string plugboardCfg   = "";
    const string reflectorCfg      = "B";
    const string  ciphertext = "A";

  
    map<string, int> rotorNameMap;
    rotorNameMap["I"]    = 0; 
    rotorNameMap["II"]   = 1; 
    rotorNameMap["III"]  = 2; 
    rotorNameMap["IV"]   = 3; 
    rotorNameMap["V"]    = 4; 
    rotorNameMap["VI"]   = 5; 
    rotorNameMap["VII"]  = 6; 
    rotorNameMap["VIII"] = 7; 
  
   
    vector<char> ringSettingVec; 
    istringstream issRingSetting(ringSetting);
    string rs;
    while(getline(issRingSetting, rs, ' '))
    {
      ringSettingVec.push_back(rs[0]);
    }
  
  
    vector<char> messageKeyVec;
    istringstream issMessageKeyVec(messageKey);
    string wp;
    while(getline(issMessageKeyVec, wp, ' '))
    {
      messageKeyVec.push_back(wp[0]);
    }
  
  
    vector<Rotor> rotorsRev;
    istringstream iss(rotorSelection);
    int k=0;
    string rotorName;
    while(getline(iss, rotorName, ' '))
    {
      if(rotorNameMap.count(rotorName) == 0)
        throw invalid_argument("selected rotor name incorrect must be I...VIII");
  
      string rotorCfg = ROTORS[rotorNameMap[rotorName]];
      istringstream ss(rotorCfg);
      string token;
      string rotorCfgArray[2]; int i=0;
      while(getline(ss, token, '<'))
      {
        rotorCfgArray[i] = token; i++;
      }
      
      string rotorWiring = rotorCfgArray[0];
      string rotorSteps  = rotorCfgArray[1];
      Rotor r = Rotor(rotorWiring, rotorSteps, ringSettingVec[k], messageKeyVec[k]);
      rotorsRev.push_back(r);
      k++; 
    }
  
    Reflector      reflector(REFLECTORS[0]);
    Plugboard      plugboard(plugboardCfg);
    string input;
    if(argc == 2) input = string(argv[1]);
    else input = ciphertext;
  
    vector<Rotor> rotors;
    rotors.push_back(rotorsRev[2]);
    rotors.push_back(rotorsRev[1]);
    rotors.push_back(rotorsRev[0]);
    string result;
    EnigmaMachine(rotors, reflector, plugboard).crypt(input, result);
    cout << "test input " << input << endl;  
    cout << "result " << result << endl;  
  
    assert(result == "M");
  
    cout << "sample encrypted successfully" << endl;
  }  
  {
	  cout << "generating test encryption single character" << endl;
    const string rotorSelection = "V IV III";
    const string ringSetting    = "C B A";
    const string messageKey     = "A A A";
    const string plugboardCfg   = "";
    const string reflectorCfg      = "B";
    const string  ciphertext = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  
    map<string, int> rotorNameMap;
    rotorNameMap["I"]    = 0; 
    rotorNameMap["II"]   = 1; 
    rotorNameMap["III"]  = 2; 
    rotorNameMap["IV"]   = 3; 
    rotorNameMap["V"]    = 4; 
    rotorNameMap["VI"]   = 5; 
    rotorNameMap["VII"]  = 6; 
    rotorNameMap["VIII"] = 7; 
  
   
    vector<char> ringSettingVec; 
    istringstream issRingSetting(ringSetting);
    string rs;
    while(getline(issRingSetting, rs, ' '))
    {
      ringSettingVec.push_back(rs[0]);
    }
  
  
    vector<char> messageKeyVec;
    istringstream issMessageKeyVec(messageKey);
    string wp;
    while(getline(issMessageKeyVec, wp, ' '))
    {
      messageKeyVec.push_back(wp[0]);
    }
  
  
    vector<Rotor> rotorsRev;
    istringstream iss(rotorSelection);
    int k=0;
    string rotorName;
    while(getline(iss, rotorName, ' '))
    {
      if(rotorNameMap.count(rotorName) == 0)
        throw invalid_argument("selected rotor name incorrect must be I...VIII");
  
      string rotorCfg = ROTORS[rotorNameMap[rotorName]];
      istringstream ss(rotorCfg);
      string token;
      string rotorCfgArray[2]; int i=0;
      while(getline(ss, token, '<'))
      {
        rotorCfgArray[i] = token; i++;
      }
      
      string rotorWiring = rotorCfgArray[0];
      string rotorSteps  = rotorCfgArray[1];
      Rotor r = Rotor(rotorWiring, rotorSteps, ringSettingVec[k], messageKeyVec[k]);
      rotorsRev.push_back(r);
      k++; 
    }
  
    Reflector      reflector(REFLECTORS[0]);
    Plugboard      plugboard(plugboardCfg);
    string input;
    if(argc == 2) input = string(argv[1]);
    else input = ciphertext;
  
    vector<Rotor> rotors;
    rotors.push_back(rotorsRev[2]);
    rotors.push_back(rotorsRev[1]);
    rotors.push_back(rotorsRev[0]);
    string result;
    EnigmaMachine(rotors, reflector, plugboard).crypt(input, result);
    cout << "test input " << input << endl;  
    cout << "result " << result << endl;  
  
    assert(result == "MPOJQALCDAATXGDXUOJMXTJEBD");
    cout << "sample encrypted successfully" << endl;
  }  
  {
    cout << "generating test encryption single character" << endl;
    const string rotorSelection = "V IV III";
    const string ringSetting    = "C B A";
    const string messageKey     = "A A A";
    const string plugboardCfg   = "AY BX DE FG QZ HP MW RS JV UT";
    const string reflectorCfg      = "B";
    const string  ciphertext = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  
    map<string, int> rotorNameMap;
    rotorNameMap["I"]    = 0; 
    rotorNameMap["II"]   = 1; 
    rotorNameMap["III"]  = 2; 
    rotorNameMap["IV"]   = 3; 
    rotorNameMap["V"]    = 4; 
    rotorNameMap["VI"]   = 5; 
    rotorNameMap["VII"]  = 6; 
    rotorNameMap["VIII"] = 7; 
  
   
    vector<char> ringSettingVec; 
    istringstream issRingSetting(ringSetting);
    string rs;
    while(getline(issRingSetting, rs, ' '))
    {
      ringSettingVec.push_back(rs[0]);
    }
  
  
    vector<char> messageKeyVec;
    istringstream issMessageKeyVec(messageKey);
    string wp;
    while(getline(issMessageKeyVec, wp, ' '))
    {
      messageKeyVec.push_back(wp[0]);
    }
  
  
    vector<Rotor> rotorsRev;
    istringstream iss(rotorSelection);
    int k=0;
    string rotorName;
    while(getline(iss, rotorName, ' '))
    {
      if(rotorNameMap.count(rotorName) == 0)
        throw invalid_argument("selected rotor name incorrect must be I...VIII");
  
      string rotorCfg = ROTORS[rotorNameMap[rotorName]];
      istringstream ss(rotorCfg);
      string token;
      string rotorCfgArray[2]; int i=0;
      while(getline(ss, token, '<'))
      {
        rotorCfgArray[i] = token; i++;
      }
      
      string rotorWiring = rotorCfgArray[0];
      string rotorSteps  = rotorCfgArray[1];
      Rotor r = Rotor(rotorWiring, rotorSteps, ringSettingVec[k], messageKeyVec[k]);
      rotorsRev.push_back(r);
      k++; 
    }
  
    Reflector      reflector(REFLECTORS[0]);
    Plugboard      plugboard(plugboardCfg);
    string input;
    if(argc == 2) input = string(argv[1]);
    else input = ciphertext;
  
    vector<Rotor> rotors;
    rotors.push_back(rotorsRev[2]);
    rotors.push_back(rotorsRev[1]);
    rotors.push_back(rotorsRev[0]);
    string result;
    EnigmaMachine(rotors, reflector, plugboard).crypt(input, result);
    cout << "test input " << input << endl;  
    cout << "result " << result << endl;  
    assert(result == "PNORAUPMEWYUIFEZNEHEZWEUBU");
    cout << "sample encrypted successfully" << endl;
  }  
}



