#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <math.h>

#define READ 0
#define WRITE 1
#define MAX 1000

int compare(const void* a, const void* b);
double getStandardDeviation(int n, int sum, int squaredSum);

int main()
{
	int parentToSon[2], sonToParentMean[2], sonToParentMiddle[2];
	int value[MAX] = { 0, };
	int sum = 0, squaredSum = 0, middleValue = 0;
	double mean = 0, standardDeviation = 0;
	int n = 0, tmp = 0;
	pid_t pid;

	if (pipe(parentToSon) < 0)
	{
		printf("pipe error\n");
		exit(1);
	}

	if (pipe(sonToParentMean) < 0)
	{
		printf("pipe error\n");
		exit(1);
	}

	if (pipe(sonToParentMiddle) < 0)
	{
		printf("pipe error\n");
		exit(1);
	}

	if ((pid = fork()) < 0)
	{
		printf("fork error\n");
		exit(1);
	}



	if (pid > 0)
	{

		printf("수 입력(음수 입력 시 입력 종료) : ");
		while (1)
		{
			scanf("%d", &tmp);
			write(parentToSon[WRITE], &tmp, sizeof(tmp));
			if (tmp < 0)
				break;
			sum += tmp;
			squaredSum += tmp * tmp;

			n++;
		}

		write(parentToSon[WRITE], &n, sizeof(n));
		standardDeviation = getStandardDeviation(n, sum, squaredSum);
		standardDeviation = pow(standardDeviation, 0.5);
		wait(NULL);

		read(sonToParentMean[READ], &mean, sizeof(mean));
		read(sonToParentMiddle[READ], &middleValue, sizeof(middleValue));

		printf("평균값(a) : %f\n", mean);
		printf("중앙값(m) : %d\n", middleValue);
		printf("표준편차(d) : %f\n", standardDeviation);

		double getK = 0;
		getK = mean - middleValue > 0 ? (mean - middleValue) : -(mean - middleValue);
		getK = getK / standardDeviation;

		int k = (getK == (int)getK) ? (int)getK : (int)getK + 1;

		printf("최소의 자연수 k : %d\n", k);

		return 0;
	}

	else
	{
		int i = 0;
		while (1)
		{

			read(parentToSon[READ], &tmp, sizeof(tmp));
			if (tmp < 0)
				break;
			value[i] = tmp;
			sum += tmp;
			i++;
		}

		read(parentToSon[READ], &n, sizeof(n));
		qsort(value, sizeof(value) / sizeof(int), sizeof(int), compare);

		mean = (double)sum / n;
		middleValue = value[999 - (n / 2)];

		write(sonToParentMean[WRITE], &mean, sizeof(mean));
		write(sonToParentMiddle[WRITE], &middleValue, sizeof(middleValue));


	}
}

int compare(const void* a, const void* b)
{
	int num1 = *(int*)a;
	int num2 = *(int*)b;

	if (num1 < num2)
		return -1;

	if (num1 > num2)
		return 1;

	return 0;
}

double getStandardDeviation(int n, int sum, int squaredSum)
{
	return (squaredSum / n) - ((sum / n) * (sum / n));
}

