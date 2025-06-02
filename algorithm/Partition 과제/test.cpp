//2021203041 소프트웨어학부 함대영
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <cfloat>
#include <climits>
#include <fstream>
using namespace std;


int idx = 0;
pair<int, int> arr[10002];       // (값, 번호)
double dp[10002][14];  // n명의 학생을 k개의 그룹으로 나눌때의 최소 분산 값
int path[10002][14];  // 경로 복원
int n,k;
bool cmp(const pair<int, int>& a, const pair<int, int>& b) {
    if (a.first == b.first) return a.second < b.second; // 점수가 같으면 번호 오름차순
    return a.first > b.first; // 점수 내림차순
}
bool sec_cmp(const pair<int, int>& a, const pair<int, int>& b){
    return a.second<b.second;
}

double prefixSum[10002];
double prefixSqSum[10002];


void precompute() {     //먼저 계산해 호출 할때마다 계산하는 시간을 줄임
    prefixSum[0] = arr[0].first;
    prefixSqSum[0] = arr[0].first * arr[0].first;
    for (int i = 1; i < n; i++) {
        double val = arr[i].first;
        prefixSum[i] = prefixSum[i - 1] + val;
        prefixSqSum[i] = prefixSqSum[i - 1] + val * val;
    }
}

double getV(int left, int right) {  //분산 계산
    int len = right - left + 1;
    double sum = prefixSum[right] - (left > 0 ? prefixSum[left - 1] : 0);
    double sqSum = prefixSqSum[right] - (left > 0 ? prefixSqSum[left - 1] : 0);
    double avg = sum / len;
    return sqSum / len - avg * avg;
}


void method1(){
    vector<pair<int,int>> diff;
    vector<int> pos;
    int ans = 0;
    for(int i = 0; i<n-1;i++){
        int sub = arr[i].first - arr[i+1].first;
        diff.push_back({sub,i+1});      
    }
    sort(diff.begin(), diff.end(), greater<>());    
    for(int i = 0; i<k-1; i++){
        pos.push_back(diff[i].second);
        ans += diff[i].first;
    }
    pos.push_back(n);
    sort(pos.begin(), pos.end());
    cout << ans<< endl;
    ofstream file1("Partition1.txt");
    int idx = 0;
    for(int i = 0; i<pos.size(); i++){
        vector<pair<int, int>> group;
        for(int j = idx; j<pos[i]; j++){
            group.push_back(arr[j]);
        }
        sort(group.begin(),group.end(),sec_cmp);
        for(int j = 0; j < group.size();j++){
            file1 << group[j].second << '(' << group[j].first << ") ";
        }
        file1 << endl;
        idx = pos[i];
    }
    file1.close();
}

void method2(){
    precompute();
    for(int i = 0; i<=n;i++){
        for(int j = 0; j<=k;j++){
            dp[i][j] = DBL_MAX;
        }
    }
    dp[0][0] = 0;   //0명 학생을 0개의 그룹으로 나눈 경우
    for(int i = 1; i<= n ; i++){ // i: 1번째 학생부터 n번째 학생까지
        for(int j = 1; j<=k; j++){  // j: 1개 그룹부터 k개 그룹까지
            for(int t = j-1; t<i;t++){  // t: j-1개의 그룹으로 나누었을 때 마지막 학생 위치
            if (dp[t][j - 1] == DBL_MAX) continue; 
                double cost = getV(t, i-1);  // t부터 i-1까지 학생들의 분산 계산
                if (dp[t][j - 1] + cost <= dp[i][j]) {  // 분산 합 최소화
                    dp[i][j] = dp[t][j - 1] + cost;
                    path[i][j] = t;  // 분할 지점 기록
                }
            }
        }
    }
    cout << dp[n][k] << endl;
    // 그룹 나누기
    ofstream file2("partition2.txt");
    vector<int> path_idx;
    int cur = n;  
    for (int i = k; i > 0; i--) {
        cur = path[cur][i];
        path_idx.push_back(cur);                   
    }
    reverse(path_idx.begin(), path_idx.end());  // 오름차순 정렬 (왼쪽 → 오른쪽)
    path_idx.push_back(n);                // 맨 처음 시작 지점
    
    for(int i = 0; i<path_idx.size();i++){
        cout << path_idx[i] << ' ';
    }
   
    vector<vector<pair<int, int>>> groups(k);
    for (int i = 0; i < k; i++) {
        int left = path_idx[i];           // inclusive
        int right = path_idx[i + 1];      // exclusive
        for (int j = left; j < right; j++) {
            groups[i].push_back(arr[j]);  
        }
        
    }
    for (int j = 0; j < k; j++) {
        sort(groups[j].begin(), groups[j].end(), sec_cmp);  // 학생을 인덱스 기준으로 오름차순 정렬
        for (int i = 0; i < groups[j].size(); i++) {
            file2 << groups[j][i].second << "(" << groups[j][i].first << ") ";  // 학생 인덱스(점수)
        }
        file2 << endl;
    }
    file2.close();


}
int main(){
    
    cin >> n >> k;
    for (int i = 0; i < n; i++) {
        int value;
        cin >> value;
        arr[i] = {value, i+1}; 
    }

    sort(arr,arr+n, cmp); //점수 순으로 내림차순 정렬하되 같은 점수를 가진 학생은 번호 오름차순으로 정렬한다
    method1();
    method2();

}