/*
 * Implementation of the CMine coverage pattern mining algorithm
 * Abinash Maharana
*/

#include<bits/stdc++.h>
using namespace std;

using namespace std;
using namespace std::chrono;
typedef long long unsigned llu;
typedef long long ll;
typedef pair<ll, ll> ii;
typedef vector<ll> vl;

//use as vector<ii> v;
#define ff(a,b) for(ll i=a; i<b;i++)
#define ff2(a,b) for(ll j=a; j<b;j++)
#define MAX INT_MAX	
#define trace(x) cerr<<#x<<": "<<x<<" "<<'\n';
#define pb push_back
#define mp make_pair

#define NTxn 10
// webpage-> mapped to the txns it occurs in. 
// const number for bitset = no of txns.


map <string,bitset<NTxn>> m;
map <int,vector<string>> Txn;
vector <map <string,bitset<NTxn>> > C,L,NO;

bool compareFn(const string &a, const string &b) 
{ 
    return (m[a].count() > m[b].count()); 
} 

vector<string> formvec(string a){
    vector<string> temp;
    stringstream ss(a);
    string word;
    while(ss>>word){
        temp.pb(word);
    }

    return temp;
}

string formstr(vector<string> a){
    string ret;
    int n=a.size();
    ff(0,n){
        ret+=a[i];
        ret+=' ';
    }
    // prune extra space at the end
    string r2="";
    if(ret[ret.length()-1]==' '){
        ff(0,ret.length()-1){
            r2+=ret[i];
        }
    }
    return r2;
}

string strcombine(string a, string b){
    vector<string> s1 = formvec(a);
    vector<string> s2 = formvec(b);
    vector<string> utd(s1.size()*4);

    vector<string>::iterator it;
    // sort(s1.begin(),s1.end(),compareFn);
    // sort(s2.begin(),s2.end(),compareFn);
    it = set_union(s1.begin(),s1.end(),s2.begin(),s2.end(),utd.begin());
    utd.resize(it-utd.begin());
    sort(utd.begin(),utd.end());
    utd.erase( unique( utd.begin(), utd.end() ), utd.end() );
    sort(utd.begin(),utd.end(),compareFn);

    string ret = formstr(utd);
    return ret;
}

float getRF(string s) {
    return float(m[s].count())/NTxn;
}

float getCS(string s) {
    vector<string> X = formvec(s);
    bitset<NTxn> res=0;
    for(auto ix: X){
        res|=m[ix];
    }

    return float(res.count())/NTxn;
}

float getOR(string s) {
    vector<string> X = formvec(s);
    sort(X.begin(),X.end(),compareFn);
    bitset<NTxn> res = 0;
    bitset<NTxn> Tr = m[X[X.size()-1]];

    ff(0,X.size()-1){
        res|=m[X[i]];
    }

    res&=Tr;

    return float(res.count())/Tr.count();
}


int main(){

    //CMine parameters 
    float minCS=0.7;
    float minRF=0.4;
    float maxOR=0.5;

    string pages[] = {"a","b","c","d","e","f"};
    string preprocd[] = {"1111000001","1000111111","1111000000",
                    "0001111000","0110000110","0000100000"};

    for(int i=0; i<6; i++){
        m.insert(mp(pages[i],bitset<NTxn>(string(preprocd[i]))));
    }

    ff(0,10){
        map <string,bitset<NTxn>> d1,d2,d3;
        C.pb(d1); L.pb(d2); NO.pb(d3);
    }
    
    // -----------------------------------------------------------
    // m <- C1
    C[1] = m;
    int idx=1;

    // doing C1 separately

    for(auto ix: C[idx]){
        // adding to Li
        if(getRF(ix.first) >= minCS){
            L[idx].insert(mp(ix.first,ix.second));
        }
        // Pruning for NOi
        if(getRF(ix.first) >= minRF){
            NO[idx].insert(mp(ix.first,ix.second));
        }
    }
    idx++;
    // making C_i+1 from NO_i
    for(auto i = NO[1].begin(); i != NO[1].end(); i++){
        auto j = i;
        j++;
        for(j; j!= NO[1].end(); j++){

            // combine and insert
            string cstr = strcombine(i->first,j->first);
            if(C[idx].find(cstr)==C[idx].end()){
                bitset<NTxn> tempbs = i->second;
                tempbs|=j->second;
                C[idx].insert(mp(cstr,tempbs));
            }
        }
    }

    while(!C[idx].empty()){
        for(auto ix: C[idx]){
        // adding to Li
        auto ORvalue = getOR(ix.first);
            if(getCS(ix.first) >= minCS && ORvalue <= maxOR)
                L[idx].insert(mp(ix.first,ix.second));
            // Pruning for NOi
            if(ORvalue <= maxOR){
                NO[idx].insert(mp(ix.first,ix.second));
        }
        }
        
        idx++;
        // making C_i+1 from NO_i
        for(auto i = NO[idx-1].begin(); i != NO[idx-1].end(); i++){
            auto j = i;
            j++;
            for(j; j!= NO[idx-1].end(); j++){

                // combine and insert
                string test1=i->first.substr(0, i->first.size() - 1);
                string test2=j->first.substr(0, j->first.size() - 1);
                if(!test1.compare(test2)) {
                    string cstr = strcombine(i->first,j->first);
                    if(cstr.length()==idx+idx-1){
                        if(C[idx].find(cstr)==C[idx].end()){
                            bitset<NTxn> tempbs = i->second;
                            tempbs|=j->second;
                            C[idx].insert(mp(cstr,tempbs));
                        }
                    }
            }
            }
        }
    }

    for(auto ix:L) {
        for(auto it:ix) {
            cout<<it.first<<'|';
        }
        if(ix.size())
            cout<<'\n';
    }
    cout<<"Success!\n";
    return 0;
}
