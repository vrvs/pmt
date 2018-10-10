#include <cstring>

#define ALPHA_SIZE 128

bool shiftor(char* pattern, char* text){
    int p_size = strlen(pattern);
    int t_size = strlen(text);
    int c_size = ((p_size - 1) >> 6) + 1;
    long** C = new long*[ALPHA_SIZE];

    buildMasks(C, c_size, pattern, p_size);

    // TODO running with the window in the text and making shift or
}

void buildMasks(long** C, int c_size, char* pattern, int p_size){
    int remain_bits = p_size%64;
    for(int i = 0;i<ALPHA_SIZE;i++){
        C[i] = new long[c_size];
        memset(C[i],-1,sizeof(long*));
    }

    for(int i = 0;i<p_size;i++){
        if(i<remain_bits){
            C[pattern[i]][0] = C[pattern[i]][0] & (~(1 << (remain_bits-i-1))); 
        } else {
            int j = ((i-remain_bits)/64)+1;
            int i_l = (i-((j-1)*64))-remain_bits;
            C[pattern[i]][j] = C[pattern[i]][j] & (~(1 << (64-i_l)));        
        }
    }
}

void ShiftAndOr(long* p, long* c, int p_size){
    p[0] <<= 1;
    for(int i = 1;i<p_size;i++){
        p[i-1] |= (p[i] < 0) ? 1 : 0;
        c[i-1] |= p[i-1]; //OR
        p[i] <<= 1; 
    }
    c[p_size-1] |= p[p_size-1];
}

int main(){
    shiftor("ar", "a aranha arranha");
    return 0;
}