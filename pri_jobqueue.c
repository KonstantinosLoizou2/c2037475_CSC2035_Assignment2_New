/*
 * Replace the following string of 0s with your student number
 * 000000000
 */
#include <stdlib.h>
#include <stdbool.h>
#include "pri_jobqueue.h"
#include <errno.h>
#include <stdio.h>

/* 
 * TODO: you must implement this function that allocates a job queue and 
 * initialise it.
 * Hint:
 * - see job_new in job.c
 */
pri_jobqueue_t* pri_jobqueue_new() {
    pri_jobqueue_t* pjq= (pri_jobqueue_t*)malloc(sizeof(pri_jobqueue_t));
    if(pjq==NULL){//use pri delete method to free the memory
        errno = ENOMEM;
        return NULL;
    }
    pri_jobqueue_init(pjq);
    return pjq;
}

/* 
 * TODO: you must implement this function.
 */
void pri_jobqueue_init(pri_jobqueue_t* pjq) {
    pjq->buf_size = JOB_BUFFER_SIZE;
    pjq->size = 0;
    for(int i = 0; i< JOB_BUFFER_SIZE; i++){
        job_init((&(pjq->jobs[i])));
    }

}

/* 
 * TODO: you must implement this function.
 * Hint:
 *      - if a queue is not empty, and the highest priority job is not in the 
 *      last used slot on the queue, dequeueing a job will result in the 
 *      jobs on the queue having to be re-arranged
 *      - remember that the job returned by this function is a copy of the job
 *      that was on the queue
 */
job_t* pri_jobqueue_dequeue(pri_jobqueue_t* pjq, job_t* dst) {
    if (pjq==NULL||pri_jobqueue_is_empty(pjq)){
        return NULL;
    }
    int i;
    //job_t* dequeued_job = &pjq->jobs[pjq->size - 1];
    job_t* dequeued_job = pri_jobqueue_peek(pjq,dst);
    for(i=0; i<pjq->size;i++){
        if (job_is_equal(&pjq->jobs[i],dequeued_job)){
            break;
        }
    }
    if(dst!=NULL){
        job_copy(&pjq->jobs[i],dst);
        //job_set(&dst,pjq->jobs[i].pid,pjq->jobs[i].id,pjq->jobs[i].priority,pjq->jobs[i].label);
        job_init((&(pjq->jobs[i])));
        pjq->size--;
        //pjq->size = pjq->size-2;
        return dst;
   }else{
        dst= (job_t*) malloc(sizeof(job_t));
        if (dst == NULL) {
            return NULL;
        }
        job_copy(&pjq->jobs[i],dst);
        job_init((&(pjq->jobs[i])));
        //pjq->size = pjq->size-2;
        pjq->size--;
        return dst;
    }
}

/* 
 * TODO: you must implement this function.
 * Hints:
 * - if a queue is not full, and if you decide to store the jobs in 
 *      priority order on the queue, enqueuing a job will result in the jobs 
 *      on the queue having to be re-arranged. However, it is not essential to
 *      store jobs in priority order (it simplifies implementation of dequeue
 *      at the expense of extra work in enqueue). It is your choice how 
 *      you implement dequeue (and enqueue) to ensure that jobs are dequeued
 *      by highest priority job first (see pri_jobqueue.h)
 * - remember that the job passed to this function is copied to the 
 *      queue
 */
void pri_jobqueue_enqueue(pri_jobqueue_t* pjq, job_t* job) {
    if (pjq==NULL||job == NULL){
        return;
    }

   if (pri_jobqueue_is_full(pjq)){
        return;
    }
    if(job->priority<1){
        return;
    }
    //if (pjq->size == 0){
   //     job_set(&pjq->jobs[0],job->pid,job->id,job->priority,job->label);
    //    pjq->size++;
   // }
   // if (pjq->jobs[pjq->size].priority == 0){
      //  int m;
       // for(m=0;m<((pjq->size)+1);m++){
       //     pjq->jobs[m] = pjq->jobs[m + 1];
        //}

   // }
    int i;
    for(i=0; i<pjq->size;i++){
        if (pjq->jobs[i].priority<=job->priority){
            break;
        }
    }
    for (int j = pjq->size; j > i; j--) {
        pjq->jobs[j] = pjq->jobs[j - 1];
    }
    job_set(&pjq->jobs[i],job->pid,job->id,job->priority,job->label);
    pjq->size++;
}
   
/* 
 * TODO: you must implement this function.
 */
bool pri_jobqueue_is_empty(pri_jobqueue_t* pjq) {
    if (pjq == NULL||pjq->size == 0){
        return true;
    }else{
        return false;
    }
}

/* 
 * TODO: you must implement this function.
 */
bool pri_jobqueue_is_full(pri_jobqueue_t* pjq) {
    if(pjq == NULL){
        return true;
    }
    return pjq->size >= pjq->buf_size;
}

/* 
 * TODO: you must implement this function.
 * Hints:
 *      - remember that the job returned by this function is a copy of the 
 *      highest priority job on the queue.
 *      - both pri_jobqueue_peek and pri_jobqueue_dequeue require copying of 
 *      the highest priority job on the queue
 */
job_t* pri_jobqueue_peek(pri_jobqueue_t* pjq, job_t* dst) {
    if(!pri_jobqueue_is_empty(pjq)){
        job_t* highest_job = &pjq->jobs[pjq->size-1];
        if(dst!=NULL){
            *dst = *highest_job;
            return dst;
        }else{
            dst= (job_t*) malloc(sizeof(job_t));
            if (dst == NULL) {
                return NULL;
            }
            *dst = *highest_job;
            return dst;
        }
    }
    return NULL;
    }

/* 
 * TODO: you must implement this function.
 */
int pri_jobqueue_size(pri_jobqueue_t* pjq) {
    if (pjq == NULL) {
        return 0;
    }else{
        return pjq->size;
    }

}

/* 
 * TODO: you must implement this function.
 */
int pri_jobqueue_space(pri_jobqueue_t* pjq) {
    if (pjq == NULL) {
        return 0;
    }else{
        return (pjq->buf_size) -(pjq->size); // or loop and count priority 0
    }
}

/* 
 * TODO: you must implement this function.
 *  Hint:
 *      - see pri_jobqeue_new
 */
void pri_jobqueue_delete(pri_jobqueue_t* pjq) {
    if (pjq ==NULL){
        free(pjq);
    }
}
