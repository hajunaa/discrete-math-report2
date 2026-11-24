# 이산수학 Report 2 — 동치 관계 판별 프로그램

집합 A = {1, 2, 3, 4, 5} 위의 관계행렬(5 × 5)을 입력받아

1. 반사 · 대칭 · 추이 관계를 판별하고
2. 동치 관계이면 동치류를 출력하며
3. 동치 관계가 아니면 반사 · 대칭 · 추이 폐포를 구해 다시 판별하고 동치류를 출력하는 C++ 프로그램입니다.

- 학과 : 소프트웨어학과
- 학번 / 이름 : 20252315 / 망다흐바야르

## 실행 방법

```bash
g++ -std=c++11 -o matWork matWork.cpp
./matWork
```

## 구성 조건과 함수

| 구성 조건 | 함수 |
|---|---|
| 1. 관계 행렬 입력 | `inputMatrix()` |
| 2. 동치 관계 판별 | `isReflexive()`, `isSymmetric()`, `isTransitive()`, `checkEquivalence()` |
| 3. 동치류 출력 | `equivalenceClass()`, `printEquivalenceClasses()` |
| 4. 폐포 구현 | `reflexiveClosure()`, `symmetricClosure()`, `transitiveClosure()` (와샬 알고리즘), `closureProcess()` |

## 추가 기능

- 입력 오류 검사 (0/1 이외의 값, 개수 오류 시 재입력)
- 성질을 위반하는 순서쌍 출력
- 폐포 변환 전 / 후 비교 (새로 추가된 순서쌍 `*` 표시)
- 와샬 알고리즘 단계별 추가 경로 출력
- 불리언 곱으로 연결관계 R* = R ∪ R² ∪ … ∪ R⁵ 계산 및 와샬 결과와 비교
- 동치 폐포 (반사 → 대칭 → 추이) 와 집합 A 의 분할 출력
- 메뉴, 예제 행렬 4개, 무작위 행렬, 단계별 일시정지
