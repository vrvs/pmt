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

void ShiftAndOr(long* w, long* c, int c_size){
    w[0] <<= 1l;
    for(int i = 1;i<c_size;i++){
        w[i-1] |= (w[i] < 0) ? 1l : 0l;
        w[i-1] |= c[i-1]; //OR
        w[i] <<= 1l; 
    }
    w[c_size-1] |= c[c_size-1];
}

long** buildMasks(char* pattern){
    int p_size = strlen(pattern);
    int c_size = ((p_size - 1) >> 6) + 1;
    long** C = (long **)malloc(ALPHA_SIZE * sizeof(long *));

    for(int i = 0;i<ALPHA_SIZE;i++){
        C[i] = (long *)malloc(c_size * sizeof(long));
        memset(C[i],-1,c_size*sizeof(long));
    }

    long* pos_mask = (long *)malloc(c_size * sizeof(long));
    memset(pos_mask,-1,c_size*sizeof(long));
    pos_mask[c_size-1] &= ~(1l);

    for(int i =0;i<p_size;i++){
        int letter = pattern[i];
        And(C[letter], pos_mask, c_size);
        Shift(pos_mask, c_size);
        pos_mask[c_size-1] |= 1l;
    }

    return C;
}

long ShiftOr(char* pattern, char* text, long** C){
    long ans = 0;
    int p_size = strlen(pattern);
    long t_size = strlen(text);
    int c_size = ((p_size - 1) >> 6) + 1;
    int remain_bits = p_size%64;

    long* window =  (long *)malloc(c_size * sizeof(long));
    memset(window,-1,c_size*sizeof(long));

    long set_i_1 = (1l << (remain_bits-1));
    
    for(long i = 0;i<t_size;i++){

        int letter = text[i];
        ShiftAndOr(window, C[letter], c_size);

        if((window[0] & set_i_1) == 0){
            ans++;
            //printf("%d\n", (i-p_size+1));
            //return true;
        }
    }
    delete [] window;
    return ans;
}

long WuManber(char* pattern, char* text, long** C, int r){
    long ans = 0;
    int p_size = strlen(pattern);
    long t_size = strlen(text);
    int c_size = ((p_size - 1) >> 6) + 1;
    int remain_bits = p_size%64;

    long** windows = (long **)malloc((r+1) * sizeof(long*));
    long** old = (long **)malloc((r+1) * sizeof(long*));
    for(int i  =0;i<r+1;i++){
        windows[i] =  (long *)malloc(c_size * sizeof(long));
        old[i] =  (long *)malloc(c_size * sizeof(long));
        memset(old[i],-1,c_size*sizeof(long));
        memset(windows[i],-1,c_size*sizeof(long));
        if(i>0)
            windows[i][c_size-1] = -2;
    } 

    long set_i_1 = (1l << (remain_bits-1));
    
    for(long i = 0;i<t_size;i++){

        int letter = text[i];

        long* aux = (long *)malloc(c_size * sizeof(long)); 
        for(int i1 =0;i1<c_size;i1++) aux[i1] = old[0][i1] = windows[0][i1];
        ShiftAndOr(aux, C[letter], c_size);
        for(int i1 =0;i1<c_size;i1++) windows[0][i1] = aux[i1];
        delete aux;

        for(int j = 1;j<r+1;j++){
            long* s1 =  (long *)malloc(c_size * sizeof(long));
            long* s2 =  (long *)malloc(c_size * sizeof(long));
            long* s3 =  (long *)malloc(c_size * sizeof(long));
            
            for(int k = 0;k<c_size;k++){
                old[j][k] = windows[j][k];
                s1[k] = old[j-1][k];
                s2[k] = windows[j-1][k];
                s3[k] = s1[k];
            }
            Shift(s2,c_size);
            Shift(s3,c_size);
            ShiftAndOr(windows[j], C[letter], c_size);
            And(windows[j], s1, c_size);
            And(windows[j], s2 , c_size);
            And(windows[j], s3, c_size);
            
            delete s1; delete s2; delete s3;
        }
        if((windows[r][0] & set_i_1) == 0){
            ans++;
            //cout << "i: " << i << endl;
            // return true;
        }
    }
    
    delete [] windows;
    return ans;
}

/*int main(){
    bool ans = Shiftor("a", "a arranhaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa aarranhaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa aar areeee");
    if(ans) printf("true\n");
    else printf("false\n");
    return 0;
}*/