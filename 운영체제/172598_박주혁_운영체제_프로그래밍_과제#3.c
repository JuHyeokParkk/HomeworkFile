// 172598 ������ �ü�� ���α׷��� ���� #3 (��Ƽ ������ ���α׷���)
// Linux ���α׷���

/* race condition�� �� �� �̻��� ���μ����� �����尡
�ϳ��� �����͸� ������ �� �����Ͱ� ����ȭ���� �ʴ� ��Ȳ�� ���Ѵ�.
�̸� �ذ��ϱ� ���ؼ��� ��ȣ ����(Mutyal Exclusion), Progress(����), Bounded waiting(���� ���)�� ���� ������ �������Ѿ� �Ѵ�.
�� ������� ���ؽ�(Mutex, Binary Semaphore), ��������(Semaphore), �����(Monitor)���� �ִ�.
�̹� �������� ���ؽ� ����� �̿��� �ش� �ڵ带 �ۼ��Ͽ���.
*/

#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

void* performThread1(void* data); // (1 ~ 4) �� �����忡�� �� ��
void* performThread2(void* data);
void* performThread3(void* data);
void* performThread4(void* data);
void find_k_prime(); // k�� ���μ� ã�� �Լ�
void find_coprime(int start, int end); // �ش� ���� ���� ���μ� ã�� �Լ�
void print_result(); // ��� ���

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

    printf("m, n, k ���� ������� �Է��ϼ��� : ");
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
    for (int i = 2; i * i <= k; i++)         // k�� ���μ� ���� ���ϱ�
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

    for (int i = start; i <= end; i++)  // �ش� ���� ���� ���� k�� ���μ��� ������������ Ȯ��
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

        pthread_mutex_lock(&mutex);         // �Ӱ� ���� ����

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
    printf("�������� LSD ���� �� (0~9)\n");

    for (int i = 0; i < 10; i++)
        printf("%d => %d��\n", i, LSD_freq[i]);

    printf("--------------------------------\n�������� MSD ���� �� (0~9)\n");

    for (int i = 0; i < 10; i++)
        printf("%d => %d��\n", i, MSD_freq[i]);
}