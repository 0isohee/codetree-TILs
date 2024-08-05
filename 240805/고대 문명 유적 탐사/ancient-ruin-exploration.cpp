#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<tuple>

using namespace std;

vector<vector<int>> map(5, vector<int> (5,0));
vector<vector<int>> smallMap(3, vector<int> (3,0));
vector<int> pieces;


int dx [] = {-1,0,1,0};
int dy [] = {0,-1,0,1};
int K, M;

struct comp {
    bool operator()(const tuple<int, int, int, int>& a, const tuple<int, int, int, int>& b) const {

        if (get<0>(a) != get<0>(b)) {
            return get<0>(a) > get<0>(b);
        }
        // 두 번째 요소 비교 (오름차순)
        if (get<1>(a) != get<1>(b)) {
            return get<1>(a) < get<1>(b);
        }
        // 네 번째 요소 비교 (오름차순)
        if (get<3>(a) != get<3>(b)) {
            return get<3>(a) < get<3>(b);
        }
        // 세 번째 요소 비교 (오름차순)
        return get<2>(a) < get<2>(b);
    }
};

struct comp1 {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) const {
        //열 번호 내림차순
        if (a.second == b.second) {
            return a.first < b.first;
        }
        //행 번호 오름차순
        return a.second > b.second;
    }
};

void printMap() {
    for (int i=0; i<5; i++){
        for (int j=0; j<5; j++){
            cout << map[i][j];
        }
        cout << "\n";
    }
}
vector<vector<int>> roate90(vector<vector<int>> &smap){
    vector<vector<int>> rotatemap(3, vector<int> (3,0));
    for (int i=0; i<3; i++){
        for (int j=2; j>-1; j--){
            rotatemap[i][2-j] = smap[j][i];
        }
    }
    return rotatemap;
}

int findMaxValue(vector<vector<int>> &map) {
    vector<vector<bool>> visited(5, vector<bool>(5, false));
    int max_length = 0;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (!visited[i][j]) {
                queue<pair<int, int>> pq;
                pq.push({i, j});
                visited[i][j] = true;
                int current_length = 1;

                while (!pq.empty()) {
                    pair<int, int> now = pq.front();
                    pq.pop();

                    for (int d = 0; d < 4; d++) {
                        int nx = now.first + dx[d];
                        int ny = now.second + dy[d];

                        if (nx < 0 || nx >= 5 || ny < 0 || ny >= 5 || visited[nx][ny])
                            continue;
                        if (map[nx][ny] == map[i][j]) {
                            visited[nx][ny] = true;
                            current_length++;
                            pq.push({nx, ny});
                        }
                    }
                }

                if (current_length >= 3) {
                    max_length += current_length;
                }
            }
        }
    }
    return max_length;
}

vector<pair<int, int>> erasePiece(vector<vector<int>> &map) {
    vector<vector<bool>> visited(5, vector<bool>(5, false));
    vector<pair<int, int>> piecesToRemove;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (!visited[i][j]) {
                queue<pair<int, int>> pq;
                vector<pair<int, int>> currentPiece;
                pq.push({i, j});
                visited[i][j] = true;
                currentPiece.push_back({i, j});

                while (!pq.empty()) {
                    pair<int, int> now = pq.front();
                    pq.pop();

                    for (int d = 0; d < 4; d++) {
                        int nx = now.first + dx[d];
                        int ny = now.second + dy[d];

                        if (nx < 0 || nx >= 5 || ny < 0 || ny >= 5 || visited[nx][ny])
                            continue;
                        if (map[nx][ny] == map[i][j]) {
                            visited[nx][ny] = true;
                            currentPiece.push_back({nx, ny});
                            pq.push({nx, ny});
                        }
                    }
                }
                if (currentPiece.size() >= 3) {
                    for (const auto &p : currentPiece) {
                        piecesToRemove.push_back(p);
                    }
                }
            }
        }
    }
   return piecesToRemove;
}

int main(){

    cin.tie(NULL);
    cout.tie(NULL);
    ios::sync_with_stdio(false);

    cin >> K >> M;
    pieces.resize(M);
    vector<int> answer;

    for (int i=0; i<5; i++){
        for (int j=0; j<5; j++){
            cin >> map[i][j];
        }
    }
    for (int i=0; i<M; i++){
        cin >> pieces[i];
    }

    while (K > 0) {
        int ans = 0;
        vector<tuple<int, int, int, int>> maxList;

        //step 1. 최대가 가능한 회전 찾기
        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++){
                for (int x=0; x<3; x++){
                    for (int y=0; y<3; y++){
                        smallMap[x][y] = map[i+x][j+y];
                    }
                }
                //smallMap을 90, 180, 270도 돌리기!
                vector<vector<int>> firstMap = roate90(smallMap);
                vector<vector<int>> secondMap = roate90(firstMap);
                vector<vector<int>> thirdMap = roate90(secondMap);

                for (int x=0; x<3; x++){
                    for (int y=0; y<3; y++){
                        map[i+x][j+y] = firstMap[x][y];
                    }
                }
                int fMax = findMaxValue(map);
                
                for (int x=0; x<3; x++){
                    for (int y=0; y<3; y++){
                        map[i+x][j+y] = secondMap[x][y];
                    }
                }
                int sMax = findMaxValue(map);

                for (int x=0; x<3; x++){
                    for (int y=0; y<3; y++){
                        map[i+x][j+y] = thirdMap[x][y];
                    }
                }
                int tMax = findMaxValue(map);

                // 최댓값 저장
                int maxV = max(max(fMax, sMax), tMax);

                if (maxV == fMax) {
                    maxList.push_back(make_tuple(maxV, 1, i, j));
                } else if (maxV == sMax) {
                    maxList.push_back(make_tuple(maxV, 2, i, j));
                } else {
                    maxList.push_back(make_tuple(maxV, 3, i, j));
                }

                //Map 원상복구
                for (int x=0; x<3; x++){
                    for (int y=0; y<3; y++){
                        map[i+x][j+y] = smallMap[x][y];
                    }
                }
            }
        }

        //step 2. 어떤 조각인지 확인
        sort(maxList.begin(), maxList.end(), comp());

        tuple<int, int, int, int> maxPiece = maxList[0];
        // cout << get<0>(maxPiece) << " " << get<1>(maxPiece) << " " << get<2>(maxPiece) <<  " " << get<3>(maxPiece) << "\n";
        int rCnt = get<1>(maxPiece);
        int px = get<2>(maxPiece);
        int py = get<3>(maxPiece);

        for (int x=0; x<3; x++){
            for (int y=0; y<3; y++){
                smallMap[x][y] = map[px+x][py+y];
            }
        }

        //step 3. 조각 채우기
        while (rCnt > 0) {
            smallMap = roate90(smallMap);
            rCnt --;
        }

        //step 4. 최댓값 map 만들기
        for (int x=0; x<3; x++){
            for (int y=0; y<3; y++){
                map[px+x][py+y] = smallMap[x][y];
            }
        }

        //map 5. map 완성 -> 계속 확인하기
        while (true) {
            vector<pair<int, int>> piecesPos = erasePiece(map);
            if (piecesPos.empty()) break;
            if (piecesPos.size() > pieces.size()) break;
            ans += piecesPos.size();

            sort(piecesPos.rbegin(), piecesPos.rend(), comp1());
        
            for (int i=0; i<piecesPos.size(); i++){
                // cout << piecesPos[i].first << " " << piecesPos[i].second << "\n";
                map[piecesPos[i].first][piecesPos[i].second] = pieces[0];
                pieces.erase(pieces.begin());
            }
        }
        K--;

        if (pieces.empty()) break;
        if (ans > 0) {
            answer.push_back(ans);
        } else {
            break;
        }
    }

    for (int i=0; i<answer.size(); i++){
        cout << answer[i] << " ";
    }
    return 0;
}