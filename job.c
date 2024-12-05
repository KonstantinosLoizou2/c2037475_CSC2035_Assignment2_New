/*
 * Replace the following string of 0s with your student number
 * 220374752
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "job.h"
#include <errno.h>
/* 
 * DO NOT EDIT the job_new function.
 */
job_t* job_new(pid_t pid, unsigned int id, unsigned int priority, 
    const char* label) {
    return job_set((job_t*) malloc(sizeof(job_t)), pid, id, priority, label);
}

/* 
 * TODO: you must implement this function
 */
job_t* job_copy(job_t* src, job_t* dst) {
    if (src == NULL){
        errno = EINVAL;
        return NULL;
    }
    if (src == dst)
        return dst;
    if (strlen(src->label) != (MAX_NAME_SIZE-1)){
        errno =EINVAL;
        return NULL;
    }
    if (dst == NULL) {
        dst = (job_t *) malloc(sizeof(job_t));//enonem
        dst->pid = src->pid;
        dst->id = src->id;
        dst->priority = src->priority;
        strncpy(dst->label, src->label, MAX_NAME_SIZE - 1);
        return dst;
    }

    dst->pid = src->pid;
    dst->id = src->id;
    dst->priority = src->priority;
    strncpy(dst->label, src->label, MAX_NAME_SIZE - 1);
    return dst;
}

/* 
 * TODO: you must implement this function
 */
void job_init(job_t* job) {
    if (job != NULL){
        job->pid = 0;
        job->id = 0;
        job->priority = 0;
        strncpy(job->label,PAD_STRING, MAX_NAME_SIZE-1);
        job->label[MAX_NAME_SIZE-1] = '\0';
    }else{
        return;
    }
}

/* 
 * TODO: you must implement this function
 */
bool job_is_equal(job_t* j1, job_t* j2) {
    if (j1 == NULL && j2 == NULL){
        return true;
    }
    if (j1 == NULL || j2 == NULL){
        return false;
    }
    if (j1->pid == j2->pid && j1->id == j2->id &&
        j1->priority == j2->priority && strcmp(j1->label,j2->label)  == 0){
        return true;
    }
    return false;
}

/*
 * TODO: you must implement this function.
 * Hint:
 * - read the information in job.h about padding and truncation of job
 *      labels
 */
job_t* job_set(job_t* job, pid_t pid, unsigned int id, unsigned int priority,
    const char* label) {
    if (job == NULL) {
        errno = EINVAL;
        return NULL;
    }
    job->pid = pid;
    job->id = id;
    job->priority = priority;
    if (label == NULL || strlen(label) == 0 ){
        strncpy(job->label,PAD_STRING,MAX_NAME_SIZE-1);
        job->label[MAX_NAME_SIZE-1] = '\0';
    }else{
        strncpy(job->label,label,MAX_NAME_SIZE-1);
        job->label[MAX_NAME_SIZE-1] = '\0';
    }
    if (strlen(job->label)< MAX_NAME_SIZE){
        memset((job->label)+ strlen(job->label),'*',(MAX_NAME_SIZE-1)-strlen(job->label));
        job->label[MAX_NAME_SIZE-1] = '\0';
    }
    return job;

}

/*
 * TODO: you must implement this function.
 * Hint:
 * - see malloc and calloc system library functions for dynamic allocation, 
 *      and the documentation in job.h for when to do dynamic allocation
 */
char* job_to_str(job_t* job, char* str) {
    if (job == NULL || strlen(job->label) != (MAX_NAME_SIZE - 1)) {
        errno = EINVAL;
        return NULL;
    }
    if (str == NULL) {
        str = (char *) malloc(JOB_STR_SIZE);
        if (str == NULL) {
            errno = ENOMEM;
            return NULL;
        }
    }
    snprintf(str, JOB_STR_SIZE, JOB_STR_FMT, (int) job->pid, job->id, job->priority, job->label);
    return str;

}

/*
 * TODO: you must implement this function.
 * Hint:
 * - see the hint for job_to_str
 */
job_t* str_to_job(char* str, job_t* job) {

    if (str==NULL||strlen(str)!=(JOB_STR_SIZE - 1)) { //probably this should be after job check
        errno = EINVAL;
        return NULL;
    }

    if (job==NULL){
        job = (job_t*)malloc(sizeof(job_t));
        if(job==NULL){
            free(job);
            errno = ENOMEM;
            return NULL;
        }
    }

    int read_string = sscanf(str,JOB_STR_FMT, &job->pid,&job->id,&job->priority,job->label);
    if (read_string != 4) { //maybe delete it?
        errno = EINVAL;
        return NULL;
    }
    if(strlen(job->label) !=(MAX_NAME_SIZE-1)){
        errno = EINVAL;
        return NULL;
    }
    return job;
}

/* 
 * TODO: you must implement this function
 * Hint:
 * - look at the allocation of a job in job_new
 */
void job_delete(job_t* job) {
    if (job != NULL){
        free(job);
    }
}