#include <iostream>
#include <cstring>
#include "algorithms.h"

using namespace std;

#define ALPHA_SIZE 128

void And(long* c, long* mask, int c_size){
    for(int i = 0;i<c_size;i++){
        c[i] &= mask[i];
    }
}

void Shift(long* p, int p_size){
    p[0] <<= 1;
    for(int i = 1;i<p_size;i++){
        p[i-1] |= (p[i] < 0) ? 1l : 0l;
        p[i] <<= 1; 
    }
}

void buildMasks(long** C, int c_size, char* pattern, int p_size, int remain_bits){
    
    for(int i = 0;i<ALPHA_SIZE;i++){
        C[i] = new long[c_size];
        memset(C[i],-1,c_size*sizeof(long));
    }

    long* pos_mask = new long[c_size];
    memset(pos_mask,-1,c_size*sizeof(long));
    pos_mask[c_size-1] &= ~(1l);
    long* one = new long[c_size];
    memset(one,0,c_size*sizeof(long));
    one[c_size-1] |= 1l;

    for(int i =0;i<p_size;i++){
        And(C[(int)pattern[i]], pos_mask, c_size);
        Shift(pos_mask, c_size);
        pos_mask[c_size-1] |= 1l;
    }
}


void ShiftAndOr(long* w, long* c, int c_size){
    w[0] <<= 1l;
    for(int i = 1;i<c_size;i++){
        w[i-1] |= (w[i] < 0) ? 1l : 0l;
        w[i-1] |= c[i-1]; //OR
        w[i] <<= 1l; 
    }
    w[c_size-1] |= c[c_size-1];
}

bool Shiftor(char* pattern, char* text){
    bool ans = false;
    int p_size = strlen(pattern);
    long t_size = strlen(text);
    int c_size = ((p_size - 1) >> 6) + 1;
    int remain_bits = p_size%64;
    long** C = new long*[ALPHA_SIZE];
    
    buildMasks(C, c_size, pattern, p_size, remain_bits);

    long* window = new long[c_size];
    memset(window,-1,c_size*sizeof(long));

    long set_i_1 = (1l << (remain_bits-1));
    int cont = 0;
    for(int i =0 ;i<t_size;i++){

        int letter = text[i];
        ShiftAndOr(window, C[letter], c_size);

        if((window[0] & set_i_1) == 0){
            cont++;
            //printf("%d\n", (i-p_size+1));
            ans = true;
            return true;
        }
    }
    //cout << "number of occ - " << cont << endl;
    return ans;
}

/*int main(){
    bool ans = Shiftor("a", "a arranhaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa aarranhaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa aar areeee");
    if(ans) printf("true\n");
    else printf("false\n");
    return 0;
}*/