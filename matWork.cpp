// =============================================================
//  Report 2 : 동치 관계 판별 및 동치류 / 폐포 프로그램
//  집합 A = {1, 2, 3, 4, 5} 위의 관계행렬(5 x 5)을 입력받아
//  반사 / 대칭 / 추이 관계를 판별하고, 동치 관계이면 동치류를 출력.
//  동치 관계가 아니면 각 폐포(반사, 대칭, 추이)를 구해 다시 판별.
// =============================================================
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int N = 5;                         // 집합 A = {1, 2, 3, 4, 5} 의 원소 개수
typedef vector<vector<int>> Matrix;      // 관계행렬 (2차원 배열)

// -------------------------------------------------------------
//  출력 도우미
// -------------------------------------------------------------
// 큰 단계 제목
void printSection(const string& title) {
    cout << "\n==================================================\n";
    cout << "  " << title << "\n";
    cout << "==================================================\n\n";
}

// 작은 소제목
void printSubTitle(const string& title) {
    cout << "  [ " << title << " ]\n\n";
}

// (추가기능) 단계별로 화면을 확인할 수 있도록 Enter 를 누를 때까지 대기
void pauseScreen() {
    cout << "  ... 계속하려면 Enter 키를 누르세요 ...";
    string dummy;
    getline(cin, dummy);
}

// 관계행렬 출력
void printMatrix(const Matrix& M) {
    cout << "           1  2  3  4  5\n";
    cout << "        +-----------------+\n";
    for (int i = 0; i < N; i++) {
        cout << "      " << i + 1 << " | ";
        for (int j = 0; j < N; j++) cout << " " << M[i][j] << " ";
        cout << " |\n";
    }
    cout << "        +-----------------+\n";
}

// (추가기능) 관계를 순서쌍 집합으로 출력 (한 줄에 6개씩)
void printPairs(const Matrix& M) {
    cout << "    R = { ";
    int count = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (M[i][j]) {
                if (count > 0) {
                    cout << ", ";
                    if (count % 6 == 0) cout << "\n          ";
                }
                cout << "(" << i + 1 << "," << j + 1 << ")";
                count++;
            }
    cout << " }\n";
    cout << "    순서쌍 개수 : " << count << "개\n\n";
}

// (추가기능) 폐포 변환 전/후를 나란히 출력하고 새로 추가된 쌍을 * 로 표시
void printBeforeAfter(const Matrix& before, const Matrix& after) {
    cout << "            < 변환 전 >                  < 변환 후 >\n\n";
    cout << "           1  2  3  4  5               1  2  3  4  5\n";
    cout << "        +-----------------+         +-----------------+\n";
    for (int i = 0; i < N; i++) {
        cout << "      " << i + 1 << " | ";
        for (int j = 0; j < N; j++) cout << " " << before[i][j] << " ";
        cout << " |  ->   " << i + 1 << " | ";
        for (int j = 0; j < N; j++) {
            bool added = (before[i][j] == 0 && after[i][j] == 1);
            cout << (added ? "*" : " ") << after[i][j] << " ";
        }
        cout << " |\n";
    }
    cout << "        +-----------------+         +-----------------+\n";
    cout << "        ( * : 폐포 변환으로 새로 추가된 순서쌍 )\n\n";

    cout << "    추가된 순서쌍 : ";
    bool any = false;
    int cnt = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (before[i][j] == 0 && after[i][j] == 1) {
                if (cnt > 0 && cnt % 8 == 0) cout << "\n                    ";
                cnt++;
                cout << "(" << i + 1 << "," << j + 1 << ") ";
                any = true;
            }
    if (!any) cout << "없음";
    cout << "\n\n";
}

// =============================================================
//  1. 관계 행렬 입력 기능
// =============================================================
// 사용자로부터 5 x 5 행렬을 행 단위로 입력받아 2차원 배열에 저장한다.
// (추가기능) 0/1 이외의 값이나 개수가 틀리면 다시 입력받는다.
Matrix inputMatrix() {
    Matrix M(N, vector<int>(N, 0));
    cout << "\n  5 x 5 관계행렬을 행 단위로 입력하세요.\n";
    cout << "  (각 행에 0 또는 1 을 5개, 공백으로 구분.  예: 1 0 0 1 0)\n\n";
    for (int i = 0; i < N; i++) {
        while (true) {
            cout << "    " << i + 1 << "행 입력 : ";
            string line;
            if (!getline(cin, line)) exit(0);
            stringstream ss(line);
            vector<int> row;
            string tok;
            bool ok = true;
            while (ss >> tok) {
                if (tok == "0" || tok == "1") row.push_back(tok[0] - '0');
                else { ok = false; break; }
            }
            if (!ok) { cout << "      [오류] 0 또는 1만 입력할 수 있습니다. 다시 입력하세요.\n"; continue; }
            if ((int)row.size() != N) {
                cout << "      [오류] 정확히 " << N << "개의 값을 입력해야 합니다. 다시 입력하세요.\n"; continue;
            }
            M[i] = row;
            break;
        }
    }
    return M;
}

// (추가기능) 예제 행렬
Matrix sampleMatrix(int k) {
    if (k == 1) // 동치 관계 : {1,2}, {3,4,5}
        return { {1,1,0,0,0},
                 {1,1,0,0,0},
                 {0,0,1,1,1},
                 {0,0,1,1,1},
                 {0,0,1,1,1} };
    if (k == 2) // 반사 X, 대칭 X, 추이 X
        return { {0,1,0,0,0},
                 {0,1,1,0,0},
                 {0,0,1,0,0},
                 {0,0,0,0,1},
                 {0,0,0,0,1} };
    if (k == 3) // 반사 O, 대칭 O, 추이 X
        return { {1,1,0,0,0},
                 {1,1,1,0,0},
                 {0,1,1,0,0},
                 {0,0,0,1,0},
                 {0,0,0,0,1} };
    // k == 4 : 수업 예제 R = {(1,1),(1,4),(2,2),(3,4),(3,5),(4,1),(5,2),(5,5)}
    return { {1,0,0,1,0},
             {0,1,0,0,0},
             {0,0,0,1,1},
             {1,0,0,0,0},
             {0,1,0,0,1} };
}

// (추가기능) 무작위 행렬 생성
Matrix randomMatrix() {
    Matrix M(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            M[i][j] = (rand() % 100 < 30) ? 1 : 0;   // 30% 확률로 1
    return M;
}

// =============================================================
//  2. 동치 관계 판별 기능
// =============================================================
// 반사 관계 : 모든 i 에 대해 M[i][i] = 1
bool isReflexive(const Matrix& M, bool verbose = false) {
    bool result = true;
    for (int i = 0; i < N; i++)
        if (M[i][i] != 1) {
            if (verbose) cout << "          - (" << i + 1 << "," << i + 1 << ") 가 없음\n";
            result = false;
        }
    return result;
}

// 대칭 관계 : 모든 i, j 에 대해 M[i][j] = M[j][i]
bool isSymmetric(const Matrix& M, bool verbose = false) {
    bool result = true;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (M[i][j] == 1 && M[j][i] != 1) {
                if (verbose) cout << "          - (" << i + 1 << "," << j + 1 << ") 는 있지만 ("
                                  << j + 1 << "," << i + 1 << ") 가 없음\n";
                result = false;
            }
    return result;
}

// 추이 관계 : M[i][j] = 1 이고 M[j][k] = 1 이면 M[i][k] = 1
bool isTransitive(const Matrix& M, bool verbose = false) {
    bool result = true;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (M[i][j])
                for (int k = 0; k < N; k++)
                    if (M[j][k] && !M[i][k]) {
                        if (verbose) cout << "          - (" << i + 1 << "," << j + 1 << "), ("
                                          << j + 1 << "," << k + 1 << ") 는 있지만 ("
                                          << i + 1 << "," << k + 1 << ") 가 없음\n";
                        result = false;
                    }
    return result;
}

// 관계의 성질을 판별하고 결과 메시지를 출력, 동치 관계 여부 반환
bool checkEquivalence(const Matrix& M) {
    bool r = isReflexive(M);
    bool s = isSymmetric(M);
    bool t = isTransitive(M);

    cout << "    1) 반사 관계 : " << (r ? "O  (만족)" : "X  (불만족)") << "\n";
    if (!r) isReflexive(M, true);       // (추가기능) 위반 원인 출력
    cout << "\n";
    cout << "    2) 대칭 관계 : " << (s ? "O  (만족)" : "X  (불만족)") << "\n";
    if (!s) isSymmetric(M, true);
    cout << "\n";
    cout << "    3) 추이 관계 : " << (t ? "O  (만족)" : "X  (불만족)") << "\n";
    if (!t) isTransitive(M, true);

    bool eq = r && s && t;
    cout << "\n    --------------------------------------------\n";
    if (eq) cout << "    => 결과 : 이 관계는 동치 관계입니다.\n";
    else    cout << "    => 결과 : 이 관계는 동치 관계가 아닙니다.\n";
    cout << "    --------------------------------------------\n\n";
    return eq;
}

// =============================================================
//  3. 동치 관계일 경우 동치류 출력 기능
// =============================================================
// 원소 a 의 동치류 [a] = { b | (a, b) ∈ R }
vector<int> equivalenceClass(const Matrix& M, int a) {
    vector<int> cls;
    for (int b = 0; b < N; b++)
        if (M[a][b]) cls.push_back(b + 1);
    return cls;
}

void printClass(const vector<int>& cls) {
    cout << "{ ";
    for (size_t i = 0; i < cls.size(); i++) cout << cls[i] << (i + 1 < cls.size() ? ", " : " ");
    cout << "}";
}

void printEquivalenceClasses(const Matrix& M) {
    printSubTitle("동치류");
    for (int a = 0; a < N; a++) {
        cout << "    [" << a + 1 << "] = ";
        printClass(equivalenceClass(M, a));
        cout << "\n";
    }

    cout << "\n";
    // (추가기능) 서로 다른 동치류로 이루어진 A 의 분할(partition) 출력
    printSubTitle("집합 A 의 분할");
    cout << "    A = ";
    vector<bool> used(N, false);
    bool first = true;
    int parts = 0;
    for (int a = 0; a < N; a++) {
        if (used[a]) continue;
        vector<int> cls = equivalenceClass(M, a);
        if (!first) cout << " U ";
        printClass(cls);
        for (int v : cls) used[v - 1] = true;
        first = false;
        parts++;
    }
    cout << "\n    (서로 다른 동치류 " << parts << "개)\n\n";
}

// =============================================================
//  4. 폐포 구현 기능
// =============================================================
// 반사 폐포 : 대각 원소를 모두 1 로
Matrix reflexiveClosure(const Matrix& M) {
    Matrix C = M;
    for (int i = 0; i < N; i++) C[i][i] = 1;
    return C;
}

// 대칭 폐포 : R ∪ R^(-1)
Matrix symmetricClosure(const Matrix& M) {
    Matrix C = M;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (M[i][j]) C[j][i] = 1;
    return C;
}

// 추이 폐포 : 와샬(Warshall) 알고리즘
// (추가기능) verbose = true 이면 단계 k 별로 추가되는 경로 i -> k -> j 를 출력
Matrix transitiveClosure(const Matrix& M, bool verbose = false) {
    Matrix C = M;
    if (verbose) printSubTitle("와샬 알고리즘 단계별 추가 경로");
    bool any = false;
    for (int k = 0; k < N; k++)
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                if (C[i][j] == 0 && C[i][k] == 1 && C[k][j] == 1) {
                    if (verbose) cout << "    단계 k=" << k + 1 << " :  " << i + 1 << " -> "
                                      << k + 1 << " -> " << j + 1 << "  경로가 추가됨\n";
                    C[i][j] = 1;
                    any = true;
                }
    if (verbose && !any) cout << "    추가된 경로 없음\n";
    if (verbose) cout << "\n";
    return C;
}

// -------------------------------------------------------------
//  (추가기능) 불리언 곱을 이용한 연결관계 R* 구하기
//  R* = R ∪ R^2 ∪ R^3 ∪ R^4 ∪ R^5   (n = 5 이므로 R^5 까지)
//  R^k : 정확히 k 단계로 갈 수 있는 순서쌍들의 관계
// -------------------------------------------------------------
// 불리언 곱 (A ⊙ B)[i][j] = OR_k ( A[i][k] AND B[k][j] )
Matrix booleanProduct(const Matrix& A, const Matrix& B) {
    Matrix C(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                if (A[i][k] && B[k][j]) { C[i][j] = 1; break; }
    return C;
}

// 불리언 합 (A ∨ B)[i][j] = A[i][j] OR B[i][j]
Matrix booleanJoin(const Matrix& A, const Matrix& B) {
    Matrix C(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            C[i][j] = (A[i][j] || B[i][j]) ? 1 : 0;
    return C;
}

// R, R^2, ..., R^5 를 계산해 나란히 출력하고 합집합 R* 를 반환
Matrix connectivityByPowers(const Matrix& M) {
    vector<Matrix> P(N);              // P[k] = R^(k+1)
    P[0] = M;
    for (int k = 1; k < N; k++) P[k] = booleanProduct(P[k - 1], M);   // R^(k+1) = R^k ⊙ R

    Matrix star = P[0];
    for (int k = 1; k < N; k++) star = booleanJoin(star, P[k]);

    cout << "    R* = R ∪ R^2 ∪ R^3 ∪ R^4 ∪ R^5   ( R^k = R^(k-1) ⊙ R , 불리언 곱 )\n\n";
    cout << "         R          R^2         R^3         R^4         R^5    |     R*\n";
    for (int i = 0; i < N; i++) {
        cout << "    ";
        for (int k = 0; k < N; k++) {
            for (int j = 0; j < N; j++) cout << P[k][i][j] << " ";
            cout << "  ";
        }
        cout << "|  ";
        for (int j = 0; j < N; j++) cout << star[i][j] << " ";
        cout << "\n";
    }
    cout << "\n";
    return star;
}

// 폐포 변환 전/후 출력 -> 다시 판별 -> 동치 관계이면 동치류 출력
void applyClosure(const string& title, const Matrix& before, const Matrix& after) {
    printSection(title);
    printSubTitle("폐포 변환 전 / 후");
    printBeforeAfter(before, after);

    printSubTitle("변환 후 관계");
    printPairs(after);

    printSubTitle("변환 후 동치 관계 재판별");
    if (checkEquivalence(after)) printEquivalenceClasses(after);
    pauseScreen();
}

void closureProcess(const Matrix& M) {
    bool r = isReflexive(M), s = isSymmetric(M), t = isTransitive(M);

    // 4-1. 성질을 만족하지 않는 경우 각각의 폐포로 변환
    if (!r) applyClosure("4-1. 반사 폐포 (Reflexive Closure)", M, reflexiveClosure(M));
    if (!s) applyClosure("4-2. 대칭 폐포 (Symmetric Closure)", M, symmetricClosure(M));
    if (!t) {
        printSection("4-3. 추이 폐포 (Transitive Closure, 와샬 알고리즘)");
        Matrix T = transitiveClosure(M, true);

        // (추가기능) 불리언 곱으로 구한 연결관계 R* 와 와샬 결과 비교
        printSubTitle("불리언 곱으로 연결관계 R* 구하기");
        Matrix S = connectivityByPowers(M);
        cout << "    => 와샬 알고리즘 결과와 "
             << (S == T ? "일치합니다. (두 방법 모두 같은 추이 폐포)" : "다릅니다!") << "\n\n";
        pauseScreen();

        printSubTitle("폐포 변환 전 / 후");
        printBeforeAfter(M, T);
        printSubTitle("변환 후 관계");
        printPairs(T);
        printSubTitle("변환 후 동치 관계 재판별");
        if (checkEquivalence(T)) printEquivalenceClasses(T);
        pauseScreen();
    }

    // 4-4. (추가기능) 반사 -> 대칭 -> 추이 순서로 모두 적용한 동치 폐포
    //      이 순서로 적용하면 항상 동치 관계가 된다.
    Matrix E = transitiveClosure(symmetricClosure(reflexiveClosure(M)));
    applyClosure("4-4. 동치 폐포 (반사 -> 대칭 -> 추이 순서로 모두 적용)", M, E);
}

// =============================================================
//  main : 전체 프로세스
// =============================================================
int main() {
    srand((unsigned)time(nullptr));

    while (true) {
        cout << "\n\n##################################################\n\n";
        cout << "        동치 관계 판별 프로그램\n";
        cout << "        집합 A = {1, 2, 3, 4, 5}\n\n";
        cout << "##################################################\n\n";
        cout << "    1. 관계행렬 직접 입력\n";
        cout << "    2. 예제 1  (동치 관계)\n";
        cout << "    3. 예제 2  (반사 X, 대칭 X, 추이 X)\n";
        cout << "    4. 예제 3  (반사 O, 대칭 O, 추이 X)\n";
        cout << "    5. 예제 4  (수업 예제 R)\n";
        cout << "    6. 무작위 행렬 생성\n";
        cout << "    0. 종료\n\n";
        cout << "  메뉴 선택 >> ";

        string choice;
        if (!getline(cin, choice)) break;
        if (choice == "0") { cout << "\n  프로그램을 종료합니다.\n\n"; break; }

        Matrix M;
        if      (choice == "1") M = inputMatrix();
        else if (choice == "2") M = sampleMatrix(1);
        else if (choice == "3") M = sampleMatrix(2);
        else if (choice == "4") M = sampleMatrix(3);
        else if (choice == "5") M = sampleMatrix(4);
        else if (choice == "6") M = randomMatrix();
        else { cout << "\n  [오류] 0 ~ 6 중에서 선택하세요.\n"; continue; }

        // 1. 입력된 관계 출력
        printSection("1. 입력된 관계행렬");
        printMatrix(M);
        cout << "\n";
        printPairs(M);

        // 2. 동치 관계 판별
        printSection("2. 동치 관계 판별");
        bool eq = checkEquivalence(M);

        // 3. 동치 관계이면 동치류 출력, 4. 아니면 폐포 변환
        if (eq) {
            printSection("3. 동치류 출력");
            printEquivalenceClasses(M);
            pauseScreen();
        } else {
            pauseScreen();
            closureProcess(M);
        }
    }
    return 0;
}
