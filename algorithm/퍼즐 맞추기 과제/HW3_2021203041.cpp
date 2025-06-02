//2021203041 소프트웨어학부 함대영
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

int puzzle[10][4]; // 퍼즐의 갯수와 북,동,남,서쪽 순서대로 그림을 저장하는 2차원 배열
int rotate_num[10]; // 회전 표시 (한번 회전시 90도로 표시하며 회전할때마다 90씩 더해진다)
bool used[10];
pair<int,int> placed[3][3];
int g_piece1 = 10, g_piece2 = -10; // 여자 그림
int b_piece1 = 100, b_piece2 = -100; // 남자 그림
int a_piece1 = 1000, a_piece2 = -1000; // 동물 그림
int r_piece1 = 10000, r_piece2 = -10000; // 로봇 그림

int u1 = 1, u2 = -1, u3 = 2, u4 = -2; // 우산 그림
int b1 = 100, b2 = -100, b3= 200, b4 = -200;// 나비 그림
int k1 = 1000, k2 = -1000, k3 = 2000, k4= -2000; // 주전자 그림
int p1 = 10000, p2 = -10000, p3= 20000, p4 =-20000; // 우편함 그림
int ans = 0;

void initializePuzzle1(){
    puzzle[1][0] = g_piece1; puzzle[1][1] = b_piece2; puzzle[1][2] = b_piece1; puzzle[1][3] = a_piece1; 
    
    puzzle[2][0] = b_piece1; puzzle[2][1] = g_piece1; puzzle[2][2] = g_piece2; puzzle[2][3] = a_piece2; 

    puzzle[3][0] = a_piece2; puzzle[3][1] = g_piece1; puzzle[3][2] = b_piece1; puzzle[3][3] = b_piece2;

    puzzle[4][0] = r_piece1; puzzle[4][1] = r_piece2; puzzle[4][2] = g_piece2; puzzle[4][3] = a_piece1;

    puzzle[5][0] = r_piece1; puzzle[5][1] = b_piece1; puzzle[5][2] = a_piece2; puzzle[5][3] = g_piece2;

    puzzle[6][0] = a_piece2; puzzle[6][1] = b_piece2; puzzle[6][2] = b_piece1; puzzle[6][3] = g_piece1;

    puzzle[7][0] = r_piece1; puzzle[7][1] = b_piece2; puzzle[7][2] = a_piece2; puzzle[7][3] = a_piece1;

    puzzle[8][0] = a_piece2; puzzle[8][1] = r_piece2; puzzle[8][2] = g_piece1; puzzle[8][3] = b_piece1;

    puzzle[9][0] = r_piece2; puzzle[9][1] = g_piece2; puzzle[9][2] = a_piece1; puzzle[9][3] = a_piece2;

}
void initializePuzzle2(){
    puzzle[1][0] = u1; puzzle[1][1] = p2; puzzle[1][2] = k1; puzzle[1][3] = b3; 
    
    puzzle[2][0] = u2; puzzle[2][1] = b4; puzzle[2][2] = p2; puzzle[2][3] = k2; 

    puzzle[3][0] = b2; puzzle[3][1] = p3; puzzle[3][2] = u1; puzzle[3][3] = k3;

    puzzle[4][0] = u3; puzzle[4][1] = b2; puzzle[4][2] = k1; puzzle[4][3] = p4;

    puzzle[5][0] = p1; puzzle[5][1] = b4; puzzle[5][2] = u4; puzzle[5][3] = k4;

    puzzle[6][0] = k2; puzzle[6][1] = u4; puzzle[6][2] = p3; puzzle[6][3] = b3;

    puzzle[7][0] = p2; puzzle[7][1] = b4; puzzle[7][2] = k1; puzzle[7][3] = u3;

    puzzle[8][0] = u2; puzzle[8][1] = k3; puzzle[8][2] = b1; puzzle[8][3] = p3;

    puzzle[9][0] = k2; puzzle[9][1] = u1; puzzle[9][2] = p1; puzzle[9][3] = b1;
}

int getPiece(int idx, int dir, int rot) {
    return puzzle[idx][(dir - rot + 4) % 4];
}
bool isPromising(int row, int col, int i, int r){

    if(row>0){
        int Uidx = placed[row-1][col].first;
        int Ur = placed[row-1][col].second;
        if(Uidx != 0){
            int s_picture = getPiece(Uidx,2,Ur);  //현재 위치 위쪽의 퍼즐의 남쪽 사진
            int n_picture = getPiece(i,0,r);    //현재 위치의 퍼즐의 북쪽 사진
            if(s_picture+n_picture!=0) return false;
        }
    }

    if(col>0){
        int Uidx = placed[row][col-1].first;
        int Ur = placed[row][col-1].second;
        if(Uidx!=0){
            int E_picture = getPiece(Uidx,1,Ur);
            int W_picture = getPiece(i,3,r);
            if(E_picture+W_picture != 0) return false;
        }
    }
    return true;

}
void sol(int cnt){
    if(cnt == 9){
        ans++;
        cout << "=== 정답 #" << ans << " ===" << endl;
        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                cout << placed[i][j].first << "(" << placed[i][j].second*90 << "도) ";
            }
            cout << endl;
        }
        return;
    }

    int row = cnt/3, col = cnt%3;
    for(int i = 1; i<=9; i++){
        if(used[i]) continue;
        for(int r = 0; r<4;r++){
            if(isPromising(row,col,i,r)){
                used[i] = true;
                placed[row][col] = {i,r};
                sol(cnt+1);
                used[i] = false;
                placed[row][col] = {0,0};
            }
        }
    }

}
int main(){
    initializePuzzle2();    
    for(int i = 0; i<3;i++){
        for(int j = 0; j<3;j++){
            placed[i][j] = {0,0};
        }
    }
    sol(0);
    if(ans == 0){
        cout << "정답이 존재하지 않습니다." <<  endl;
    }   
    return 0;
}