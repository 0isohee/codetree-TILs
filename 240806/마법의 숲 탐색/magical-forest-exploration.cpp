#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int R, C, K;
int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};
vector<vector<int>> map;
vector<vector<bool>> isExit;

void fillBlock(int idx, int x, int y, int dir) {
    map[x][y] = idx;
    for (int d = 0; d < 4; d++) {
        map[x + dx[d]][y + dy[d]] = idx;
    }
    isExit[x + dx[dir]][y + dy[dir]] = true;
}

bool isRange(int x, int y) {
    //중앙값 가능한지 확인
    if (x <= 0 || x >= R + 2 || y <= 0 || y >= C - 1 || map[x][y] != 0) return false;

    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (nx < 0 || nx >= R + 3 || ny < 0 || ny >= C) return false;
        if (map[nx][ny] != 0) return false;
    }
    return true;
}

bool canMove(int x, int y) {
    if (x <= 0 || x >= R + 3 || y <= 0 || y >= C || map[x][y] == 0) return false;
    return true;
}

int moveBlock(int v, int x, int y) {
    int maxR = x - 2;
    queue<pair<int, int>> q;
    vector<vector<bool>> visited(R + 3, vector<bool>(C, false));
    q.push({x, y});
    visited[x][y] = true;

    while (!q.empty()) {
        pair<int, int> now = q.front();
        q.pop();

        for (int d = 0; d < 4; d++) {
            int nx = now.first + dx[d];
            int ny = now.second + dy[d];

            //현재 정령 블럭이거나, 출입구면
            if (canMove(nx, ny) && !visited[nx][ny] && ((map[nx][ny] == map[now.first][now.second]) || (map[nx][ny] != 0 && isExit[now.first][now.second]))) {
                visited[nx][ny] = true;
                q.push({nx, ny});
                maxR = max(maxR, nx - 2);
            }
        }
    }
    return maxR;
}

void clearMap() {
    for (int i = 0; i < R + 3; i++) {
        for (int j = 0; j < C; j++) {
            map[i][j] = 0;
            isExit[i][j] = false;
        }
    }
}

int main() {
    cin.tie(NULL);
    cout.tie(NULL);
    ios::sync_with_stdio(false);

    cin >> R >> C >> K;
    map.resize(R + 3, vector<int>(C));
    isExit.resize(R + 3, vector<bool>(C, false));

    int ans = 0;

    for (int i = 0; i < K; i++) {
        int c, d;
        cin >> c >> d;
        c = c - 1;
        int r = 0;

        while (r <= R + 2) {
            if (isRange(r + 1, c)) {
                r++;
            }
            //서쪽 회전
            else if (isRange(r, c - 1) && isRange(r + 1, c - 1)) {
                c -= 1;
                r += 1;
                d = (d + 3) % 4;
            }
            //동쪽 회전
            else if (isRange(r, c + 1) && isRange(r + 1, c + 1)) {
                c += 1;
                r += 1;
                d = (d + 1) % 4;
            }
            // 세 방향 다 안됨 ..
            else {
                break;
            }
        }
        if (r <= 2) {
            clearMap();
        } else {
            fillBlock(i + 1, r, c, d);
            ans += moveBlock(i + 1, r, c);
        }
    }
    cout << ans << "\n";
    return 0;
}