#include <iostream>
#include <cstring>
#include "algorithms.h"

using namespace std;

#define ALPHA_SIZE 256

inline void And(long* c, long* mask, int c_size){
    for(int i = 0;i<c_size;i++){
        c[i] &= mask[i];
    }
}

inline void AndShift(long* c, long* p, int p_size){
    c[0] &= p[0];
    p[0] <<= 1;
    for(int i = 1;i<p_size;i++){
        c[i] &= p[i];
        p[i-1] |= (p[i] < 0) ? 1l : 0l;
        p[i] <<= 1; 
    }
}

inline void And4(long* p1, long* p2, long* p3, long* p4, int c_size){
    for(int i = 0;i<c_size;i++){
        p1[i] &= p2[i] & p3[i] & p4[i];
    }
}

inline void Shift(long* p, int p_size){
    p[0] <<= 1;
    for(int i = 1;i<p_size;i++){
        p[i-1] |= (p[i] < 0) ? 1l : 0l;
        p[i] <<= 1; 
    }
}

inline void Shift2(long* p1, long* p2, int p_size){
    p1[0] <<= 1; p2[0] <<= 1;
    for(int i = 1;i<p_size;i++){
        p1[i-1] |= (p1[i] < 0) ? 1l : 0l;
        p1[i] <<= 1;
        p2[i-1] |= (p2[i] < 0) ? 1l : 0l;
        p2[i] <<= 1; 
    }
}

inline void ShiftAndOr(long* w, long* c, int c_size){
    w[0] <<= 1l;
    for(int i = 1;i<c_size;i++){
        w[i-1] |= (w[i] < 0) ? 1l : 0l;
        w[i-1] |= c[i-1]; 
        w[i] <<= 1l; 
    }
    w[c_size-1] |= c[c_size-1];
}

long** buildMasks(char* pattern){
    int p_size = strlen(pattern);
    int c_size = ((p_size - 1) >> 6) + 1;
    long** C = new long*[ALPHA_SIZE];
    
    for(int i = 0;i<ALPHA_SIZE;i++){
        C[i] = new long[c_size];
        memset(C[i],-1,c_size*sizeof(long));
    }

    long pos_mask[c_size];
    memset(pos_mask,-1,sizeof(pos_mask));
    pos_mask[c_size-1] &= ~(1l);

    for(int i =0;i<p_size;i++){
        int letter = pattern[i];
        AndShift(C[letter],pos_mask,c_size);
        // And(C[letter], pos_mask, c_size);
        // Shift(pos_mask, c_size);
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

    long window[c_size];
    memset(window,-1,sizeof(window));

    long set_i_1 = (1l << (remain_bits-1));
    
    for(long i = 0;i<t_size;i++){

        int letter = text[i];
        if(letter < 0 || letter > 255){
            continue;
        }
        ShiftAndOr(window, C[letter], c_size);

        if((window[0] & set_i_1) == 0){
            ans++;
        }
    }
    return ans;
}

long WuManber(char* pattern, char* text, long** C, int r){
    long ans = 0;
    int p_size = strlen(pattern);
    long t_size = strlen(text);
    int c_size = ((p_size - 1) >> 6) + 1;
    int remain_bits = p_size%64;

    long windows[r+1][c_size];
    long old[r+1][c_size];
    memset(windows,-1,sizeof(windows));
    memset(old,-1,sizeof(old));
    int k = c_size-1;
    long shift_num = 0;
    for(int i=0;i<r+1;i++){
        if(i>0){
            for(int j = c_size-1;j>k;j--){
                windows[i][j] = 0;
            }
            windows[i][k] = -1;
            windows[i][k] <<= shift_num;
        }
        if(shift_num == 64){
            k--;
            shift_num = 1;
        }
        shift_num++;
    }
    long aux[c_size];
    long s1[c_size];
    long s2[c_size];
    long s3[c_size];

    long set_i_1 = (1l << (remain_bits-1));
    
    for(long i = 0;i<t_size;i++){

        int letter = text[i];
        if(letter < 0 || letter > 255){
            continue;
        }

        for(int j =0;j<c_size;j++) aux[j] = old[0][j] = windows[0][j];
        ShiftAndOr(aux, C[letter], c_size);
        for(int j =0;j<c_size;j++) windows[0][j] = aux[j];

        for(int j = 1;j<r+1;j++){
            
            for(int k = 0;k<c_size;k++){
                old[j][k] = windows[j][k];

                s1[k] = old[j-1][k];
                s2[k] = windows[j-1][k];
                s3[k] = s1[k];
            }
            Shift2(s2,s3,c_size);
            ShiftAndOr(windows[j], C[letter], c_size);
            And4(windows[j], s1, s2, s3, c_size);
        }
        if(!(windows[r][0] & set_i_1)){
            ans++;
            //cout << "i: " << i << endl;
            // return true;
        }
    }
    return ans;
}

/*int main(){
    bool ans = Shiftor("a", "a arranhaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa aarranhaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa aar areeee");
    if(ans) printf("true\n");
    else printf("false\n");
    return 0;
}*/