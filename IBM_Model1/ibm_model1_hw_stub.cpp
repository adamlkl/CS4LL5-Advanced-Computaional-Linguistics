#include<vector>
#include<string>
#include<iostream>
#include<iomanip>
#include<cmath>

using namespace std;

// want to represents vocab items by integers because then various tables 
// need by the IBM model and EM training can just be represented as 2-dim 
// tables indexed by integers

// the following #defines, defs of VS, VO, S, O, and create_vocab_and_data()
// are set up to deal with the specific case of the two pair corpus
// (la maison/the house)
// (la fleur/the flower)

// S VOCAB
#define LA 0
#define MAISON 1
#define FLEUR 2
// O VOCAB
#define THE 0
#define HOUSE 1
#define FLOWER 2

#define VS_SIZE 3
#define VO_SIZE 3
#define D_SIZE 2

#define MAX_ITERATION 3
vector<string> VS(VS_SIZE); // S vocab: VS[x] gives Src word coded by x 
vector<string> VO(VO_SIZE); // O vocab: VO[x] gives Obs word coded by x

vector<vector<int> > S(D_SIZE); // all S sequences; in this case 2
vector<vector<int> > O(D_SIZE); // all O sequences; in this case 2

double exp_count[VO_SIZE][VS_SIZE];
double probability[VO_SIZE][VS_SIZE];

// sets S[0] and S[1] to be the int vecs representing the S sequences
// sets O[0] and O[1] to be the int vecs representing the O sequences
void create_vocab_and_data(); 

// functions which use VS and VO to 'decode' the int vecs representing the 
// Src and Obs sequences
void show_pair(int d);
void show_O(int d); 
void show_S(int d);

main() {
  create_vocab_and_data();

  // guts of it to go here
  // you may well though want to set up further global data structures
  // and functions which access them 

  // initialise tr(o|s) uniformly
  for(int i = 0; i < VO_SIZE; i++){
    for(auto j= 0; j < VS_SIZE; j++){
      probability[i][j] = 1.0/3.0;
    }
  }

  // repeat [E] followed by [M] till convergence
  for(int c = 0; c<MAX_ITERATION; c++){

    //[E ]
    for(int i = 0; i < VO_SIZE; i++){
      for(auto j= 0; j < VS_SIZE; j++){
        exp_count[i][j] = 0;
      }
    }

    std::vector<int> obs; 
    std::vector<int> src;

    for(int i = 0; i < D_SIZE; i++){
      obs = O[i];
      src = S[i];

      for(int j = 0; j<obs.size(); j++){
        int obs_word = obs[j];
        double prob_obs_word = 0.0;

        for(int k = 0; k < src.size(); k++){
          int src_word = src[k];
          prob_obs_word += probability[obs_word][src_word];
        }

        for(int l = 0; l < src.size(); l++){
          int src_word = src[l];
          if(probability[obs_word][src_word] > 0 ) exp_count[obs_word][src_word] += probability[obs_word][src_word]/prob_obs_word;
          else exp_count[obs_word][src_word] += 0; 
        }
      }
    }

    for(int s = 0; s < VS_SIZE; s++){
      double normalise_factor = 0.0;

      for(int o = 0; o < VO_SIZE; o++){
        normalise_factor +=  exp_count[o][s];
      }

      if(normalise_factor > 0){
        for (int o = 0; o<VO_SIZE; o++){
          probability[o][s] = exp_count[o][s] / normalise_factor;
        }
      }

      else{
        for (int o = 0; o<VO_SIZE; o++){
          probability[o][s] = normalise_factor;
        }
      }
    }
  

    cout << "expected count" << endl;
    for(int x = 0; x < VO_SIZE; x++){
      for(int y = 0; y < VS_SIZE; y++){
        cout << exp_count[x][y] << " | "; 
      }
      cout << endl; 
    }

    cout << "tr(o|s)" << endl;
    for(int x = 0; x < VO_SIZE; x++){
      for(int y = 0; y < VS_SIZE; y++){
        cout << probability[x][y] << " | "; 
      }
      cout << endl; 
    }  
  }
}

void create_vocab_and_data() {

  VS[LA] = "la";
  VS[MAISON] = "maison";
  VS[FLEUR] = "fleur";

  VO[THE] = "the";
  VO[HOUSE] = "house";
  VO[FLOWER] = "flower";

  cout << "source vocab\n";
  for(int vi=0; vi < VS_SIZE; vi++) {
    cout << VS[vi] << " ";
  }
  cout << endl;
  cout << "observed vocab\n";
  for(int vj=0; vj < VO_SIZE; vj++) {
    cout << VO[vj] << " ";
  }
  cout << endl;

  // make S[0] be {LA,MAISON}
  //      O[0] be {THE,HOUSE}
  S[0].resize(2);   O[0].resize(2);
  S[0] = {LA,MAISON};
  O[0] = {THE,HOUSE};

  // make S[1] be {LA,FLEUR}
  //      O[1] be {THE,FLOWER}
  S[1].resize(2);   O[1].resize(2);
  S[1] = {LA,FLEUR};
  O[1] = {THE,FLOWER};

  for(int d = 0; d < S.size(); d++) {
    show_pair(d);
  }
}

void show_O(int d) {
  for(int i=0; i < O[d].size(); i++) {
    cout << VO[O[d][i]] << " ";
  }
}

void show_S(int d) {
  for(int i=0; i < S[d].size(); i++) {
    cout << VS[S[d][i]] << " ";
  }
}

void show_pair(int d) {
  cout << "S" << d << ": ";
  show_S(d);
  cout << endl;
  cout << "O" << d << ": ";
  show_O(d);
  cout << endl;
}

 
