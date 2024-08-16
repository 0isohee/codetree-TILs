#include<iostream>
#include<vector>
#include<utility>
using namespace std;

struct Person {
    int num,r,c,h,w,k,ok;
    bool isdead;
};

int L, N, Q;
int dx [] = {-1,0,1,0};
int dy [] = {0,1,0,-1};
vector<vector<int>> map;
vector<vector<pair<int,int>>> visited;
vector<Person> peopleList;
vector<pair<int,int>> orders;

bool isRange(int x, int y) {
    return x >= 0 && x < L && y >= 0 && y < L;
}

void init() {
    map.resize(L, vector<int>(L, 0));
    visited.resize(L, vector<pair<int,int>>(L));
    peopleList.resize(N);
    orders.resize(Q);

    for (int i=0; i<L; i++) {
        for (int j=0; j<L; j++) {
            cin >> map[i][j];
        }
    }
    for (int i=0; i<N; i++) {
        Person p;
        p.num = i + 1;
        cin >> p.r >> p.c >> p.h >> p.w >> p.k;
        p.ok = p.k;
        p.isdead = false;
        peopleList[i] = p;
    }
    for (int i=0; i<Q; i++) {
        int num, dir;
        cin >> num >> dir;
        orders[i] = {num, dir};
    }

    // 현재 상태를 visited에 표시
    for (int i=0; i<N; i++) {
        Person p = peopleList[i];
        int sx = p.r - 1;
        int sy = p.c - 1;
        for (int x = sx; x < sx + p.h; x++) {
            for (int y = sy; y < sy + p.w; y++) {
                if (isRange(x, y)) { // 경계 체크 추가
                    visited[x][y].first = p.num;
                    visited[x][y].second = p.k;
                }
            }
        }
    }
}

int checkIdx(int n) {
    for (int i = 0; i < peopleList.size(); i++) {
        if (!peopleList[i].isdead && peopleList[i].num == n) {
            return i;
        }
    }
    return -1;
}

void movePerson(int orin, int idx, Person p, int d) {
    if (p.isdead) return;

    int sx = p.r - 1;
    int sy = p.c - 1;

    // 기존 위치 비우기
    for (int x = sx; x < sx + p.h; x++) {
        for (int y = sy; y < sy + p.w; y++) {
            if (isRange(x, y)) { // 경계 체크 추가
                visited[x][y].first = 0;
                visited[x][y].second = 0;
            }
        }
    }

    // 이동
    p.r += dx[d];
    p.c += dy[d];
    sx = p.r - 1;
    sy = p.c - 1;

    // 함정 데미지 적용
    if (orin != idx) {
        for (int x = sx; x < sx + p.h; x++) {
            for (int y = sy; y < sy + p.w; y++) {
                if (map[x][y] == 1) { 
                    p.k -= 1;
                }
            }
        }
    }

    if (p.k <= 0) {
        p.isdead = true;
        p.k = 0;
    }

    // 새로운 위치에 업데이트
    for (int x = sx; x < sx + p.h; x++) {
        for (int y = sy; y < sy + p.w; y++) {
            if (isRange(x, y)) { // 경계 체크 추가
                visited[x][y].first = p.num;
                visited[x][y].second = p.k;
            }
        }
    }

    peopleList[idx] = p;
}

bool isCanMove(int orin, Person p, int d) {
    int nx = p.r + dx[d] - 1;
    int ny = p.c + dy[d] - 1;

    for (int x = nx; x < nx + p.h; x++) {
        for (int y = ny; y < ny + p.w; y++) {
            // 범위 벗어나거나 벽이면 이동 불가
            if (!isRange(x, y)) {
                return false;
            } else if (map[x][y] == 2) {
                return false;
            }
            // 다른 기사가 있으면 연쇄 이동 처리
            else if (visited[x][y].first > 0 && visited[x][y].first != p.num) {
                int idx = checkIdx(visited[x][y].first);
                if (idx == -1) continue;
                if (isCanMove(orin, peopleList[idx], d)) {
                    movePerson(orin, idx, peopleList[idx], d);
                } else {
                    return false;
                }
            }
        }
    }
    return true;
}

void gameStart() {
    for (int i = 0; i < Q; i++) {
        int num = orders[i].first - 1;
        int dir = orders[i].second;

        if (!peopleList[num].isdead && isCanMove(num, peopleList[num], dir)) {
            movePerson(num, num, peopleList[num], dir);
        }
    }
}

int getDamage() {
    int ans = 0;
    for (int i = 0; i < N; i++) {
        if (peopleList[i].isdead) continue;
        ans += (peopleList[i].ok - peopleList[i].k);
    }
    return ans;
}

int main() {
    cin.tie(NULL);
    cout.tie(NULL);
    ios::sync_with_stdio(false);

    cin >> L >> N >> Q;

    init();
    gameStart();

    int ans = getDamage();
    cout << ans << "\n";
    return 0;
}