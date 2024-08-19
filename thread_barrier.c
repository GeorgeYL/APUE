#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#include <sys/time.h>
#include <stdio.h>

#define NTHR 8       /*Number of threads */
#define NUMNUM 80000000L     /*Number of numbers to sort */
#define TNUM  (NUMNUM/NTHR) /*Number to sort per thread */

long nums[NUMNUM];
long snums[NUMNUM];

pthread_barrier_t b;

/*extern int heapsort(void *, size_t, size_t,
                                        int (*)(const void *, const void *));

*/

void alg_heapRebuild(long *arr, long root, long size)
{
        long child=2*root+1;
        if(child <= size-1)
        {
                long rightChild = child+1;
                if(rightChild <= size-1)
                        if(arr[child] < arr[rightChild])
                                child = rightChild;
                if(arr[root] < arr[child])
                {
                        long temp=arr[child];
                        arr[child]=arr[root];
                        arr[root]=temp;
                        alg_heapRebuild(arr,child,size);
                }
        }
}

void heapSort(long* arr, long size)
{
    long i, heapLen;
    heapLen     = size;
    printf("start from:%ld,size:%ld\n", arr[0],size);
        for( i = heapLen/2 ; i >= 0; i--)
        {
                alg_heapRebuild(arr,i,heapLen);
        }
        long last=heapLen - 1;
        for( i=1; i <= heapLen;i++,last--)
        {
                long temp=arr[0];
                arr[0]=arr[last];
                arr[last]=temp;
                alg_heapRebuild(arr,0,last);
        }
}


int complong(const void *arg1, const void *arg2)
{
        long l1 = *(long *)arg1;
        long l2 = *(long *)arg2;

        if(l1==l2)
                return 0;
        else if(l1 < l2)
                return -1;
        else
                return 1;
}

//
void * thr_fn(void *arg)
{
        long idx = (long)arg;
        printf("thread:%ld,start:%ld\n",pthread_self(), idx);
        //heapsort(&nums[idx], TNUM, sizeof(long), complong);
        heapSort((long*)&nums[idx],(long)TNUM);

        pthread_barrier_wait(&b);

        /*
        *
        */

        return 0;
}

void merge()
{
        long idx[NTHR];
        long i, minidx, sidx, num;

        for(i=0; i< NTHR; i++)
                idx[i] = i* TNUM;

        for(sidx = 0; sidx < NUMNUM; sidx ++)
        {
                num = LONG_MAX;
                for(i=0; i< NTHR; i++)
                {
                        if((idx[i]<(i+1)*TNUM) && (nums[idx[i]] < num))
                        {
                                num = nums[idx[i]];
                                minidx = i;
                        }
                }
                snums[sidx] = nums[idx[minidx]];
                idx[minidx]++;
        }
}

int main()
{
        unsigned       long i;
        struct timeval start, end;
        long long      startusec, endusec;
        double             elapsed;
        int                     err;
        pthread_t               tid;

        // create the initial set of numbers to sort;

        srandom(1);
        for(i = 0 ;i< NUMNUM; i++)
                nums[i] = random();

    // create 8 threads to sort the numbers
        gettimeofday(&start, NULL);
        pthread_barrier_init(&b, NULL, NTHR+1);
        for(i=0; i< NTHR; i++) {
                err = pthread_create(&tid, NULL, thr_fn, (void*)(i*TNUM));
                if(err != 0)
                        printf("Error: can't craete thread");
        }
        pthread_barrier_wait(&b);
        merge();
        gettimeofday(&end, NULL);

        startusec = start.tv_sec * 1000000 + start.tv_usec;
        endusec = end.tv_sec *1000000 + end.tv_usec;
        elapsed = (double)(endusec - startusec)/1000000.0;
        /*for(i=0; i < NUMNUM; i++){
                if(i%10 == 0) printf("\n");
                printf("%ld,", snums[i]);}
        */
        printf("\nsort took %.4f seconds[Num:%ld]\n", elapsed,NUMNUM);

        return 0;
}
