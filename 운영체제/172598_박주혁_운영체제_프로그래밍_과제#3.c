// 172598 박주혁 운영체제 프로그래밍 과제 #3 (멀티 스레드 프로그래밍)
// Linux 프로그래밍

/* race condition은 두 개 이상의 프로세스나 스레드가
하나의 데이터를 공유할 때 데이터가 동기화되지 않는 상황을 말한다.
이를 해결하기 위해서는 상호 배제(Mutyal Exclusion), Progress(진행), Bounded waiting(유한 대기)와 같은 조건을 만족시켜야 한다.
그 방법으로 뮤텍스(Mutex, Binary Semaphore), 세마포어(Semaphore), 모니터(Monitor)등이 있다.
이번 과제에서 뮤텍스 기법을 이용해 해당 코드를 작성하였다.
*/

#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

void* performThread1(void* data); // (1 ~ 4) 각 스레드에서 할 일
void* performThread2(void* data);
void* performThread3(void* data);
void* performThread4(void* data);
void find_k_prime(); // k의 소인수 찾는 함수
void find_coprime(int start, int end); // 해당 범위 내의 서로소 찾는 함수
void print_result(); // 결과 출력

int m, n, k;
int LSD_freq[10] = { 0, }, MSD_freq[10] = { 0, }, k_prime[20] = { 0, };
int quarter_range;
int k_prime_num = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char* argv[]) {

    int status;
    pthread_t threads[4];
    char threadNames[4][128] = {
      {"thread1"},
      {"thread2"},
      {"thread3"},
      {"thread4"}
    };

    printf("m, n, k 값을 순서대로 입력하세요 : ");
    scanf("%d %d %d", &m, &n, &k);

    find_k_prime();
    
    quarter_range = (n - m) / 4;

    pthread_create(&threads[0], NULL, performThread1, (void*)threadNames[0]);
    pthread_create(&threads[1], NULL, performThread2, (void*)threadNames[1]);
    pthread_create(&threads[2], NULL, performThread3, (void*)threadNames[2]);
    pthread_create(&threads[3], NULL, performThread4, (void*)threadNames[3]);

    pthread_join(threads[0], (void**)&status);
    pthread_join(threads[1], (void**)&status);
    pthread_join(threads[2], (void**)&status);
    pthread_join(threads[3], (void**)&status);

    print_result();

    return 0;
}

void* performThread1(void* data) 
{
    find_coprime(m, m + quarter_range);
}

void* performThread2(void* data) 
{
    find_coprime(m + quarter_range + 1, m + (quarter_range * 2));
}

void* performThread3(void* data) 
{
    find_coprime(m + (quarter_range * 2) + 1, m + (quarter_range * 3));
}

void* performThread4(void* data) 
{
    find_coprime((m + quarter_range * 3) + 1, n);
}

void find_k_prime()
{
    for (int i = 2; i * i <= k; i++)         // k의 소인수 집합 구하기
    {
        if (k % i == 0)
        {
            k_prime[k_prime_num++] = i;
            while (k % i == 0)
                k /= i;
        }

    }

    if (k != 1)
        k_prime[k_prime_num++] = k;
    
}
void find_coprime(int start, int end)
{
    start = (start == 1) ? 2 : start;

    for (int i = start; i <= end; i++)  // 해당 범위 내의 수가 k의 소인수로 나누어지는지 확인
    {
        bool coprime_flag = true;

        for (int j = 0; j < k_prime_num; j++)
        {

            if (i % k_prime[j] == 0 && k_prime[j] <= i)
            {
                coprime_flag = false;
                break;
            }
        }

        pthread_mutex_lock(&mutex);         // 임계 구역 설정

        int current = 10;
        if (coprime_flag)
        {
            LSD_freq[i % 10]++;

            while (i >= current)
                current *= 10;

            MSD_freq[i / (current / 10)]++;

        }

        pthread_mutex_unlock(&mutex);
    }

    
}

void print_result()
{
    printf("십진수의 LSD 숫자 빈도 (0~9)\n");

    for (int i = 0; i < 10; i++)
        printf("%d => %d번\n", i, LSD_freq[i]);

    printf("--------------------------------\n십진수의 MSD 숫자 빈도 (0~9)\n");

    for (int i = 0; i < 10; i++)
        printf("%d => %d번\n", i, MSD_freq[i]);
}