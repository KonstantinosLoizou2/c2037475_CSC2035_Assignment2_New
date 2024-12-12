/*
 * Replace the following string of 0s with your student number
 * 220374752
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include "joblog.h"
#include <string.h>

/* 
 * DO NOT EDIT the new_log_name function. It is a private helper 
 * function provided for you to create a log name from a process 
 * descriptor for use when reading, writing and deleting a log file.
 * 
 * You must work out what the function does in order to use it properly
 * and to clean up after use.
 */
static char* new_log_name(proc_t* proc) {
    static char* joblog_name_fmt = "%s/%.31s%07d.txt";
                                // string format for the name of a log file
                                // declared static to have only one instance

    if (!proc)
        return NULL;

    char* log_name;
            
    asprintf(&log_name, joblog_name_fmt, JOBLOG_PATH, proc->type_label,
        proc->id);

    return log_name;
}

/* 
 * DO NOT EDIT the joblog_init function that sets up the log directory 
 * if it does not already exist.
 */
int joblog_init(proc_t* proc) {
    if (!proc) {
        errno = EINVAL;
        return -1;
    }
        
    int r = 0;
    if (proc->is_init) {
        struct stat sb;
    
        if (stat(JOBLOG_PATH, &sb) != 0) {
            errno = 0;
            r = mkdir(JOBLOG_PATH, 0777);
        }  else if (!S_ISDIR(sb.st_mode)) {
            unlink(JOBLOG_PATH);
            errno = 0;
            r = mkdir(JOBLOG_PATH, 0777);
        }
    }

    joblog_delete(proc);    // in case log exists for proc
    
    return r;
}

/* 
 * TODO: you must implement this function.
 * Hints:
 * - you have to go to the beginning of the line represented
 *      by entry_num to read the required entry
 * - see job.h for a function to create a job from its string representation
 */
job_t* joblog_read(proc_t* proc, int entry_num, job_t* job) {
    int saved_errno = errno;
    if (!proc||entry_num<0){
        errno = saved_errno;
        return NULL;
    }
    char * file_name = new_log_name(proc);
    if (file_name == NULL){
        errno = saved_errno;
        return NULL;
    }
    FILE* log_file;
    int line_num = 0;
    log_file = fopen(file_name, "r");
    if(!log_file){
        errno = saved_errno;
        return NULL;
    }
    char buffer[JOB_STR_SIZE];
    while (fgets(buffer, sizeof(buffer), log_file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strlen(buffer) == 0) {
            continue;
        }
        if (line_num == entry_num){
            if (job == NULL){
                job = (job_t*)malloc(sizeof(job_t));
                if (job ==NULL){
                    errno = ENOMEM;
                    fclose(log_file);
                    return NULL;
                }
            }
            str_to_job(buffer,job);
            fclose(log_file);
            return job;
        }
        line_num++;
    }
    fclose(log_file);
    return NULL;
}

/* 
 * TODO: you must implement this function.
 * Hints:
 * - remember new entries are appended to a log file
 * - see the hint for joblog_read
 */
void joblog_write(proc_t* proc, job_t* job) {
    if (proc == NULL||job == NULL){
        return ;
    }
    char * file_name = new_log_name(proc);
    if (file_name == NULL){
        return ;
    }
    FILE* log_file;
    log_file = fopen(file_name, "a");
    if(!log_file){
        return;
    }
    char proc_id_buffer[64];
    snprintf(proc_id_buffer, sizeof(proc_id_buffer), "%07d", (int)job->pid);
    char job_id_buffer[64];
    snprintf(job_id_buffer, sizeof(job_id_buffer), "%05d", (int)job->id);

    if (strlen(job->label)< MAX_NAME_SIZE){
        memset((job->label)+ strlen(job->label),'*',(MAX_NAME_SIZE-1)-strlen(job->label));
        job->label[MAX_NAME_SIZE-1] = '\0';
    }

    char entry[JOB_STR_SIZE];
    snprintf(entry, JOB_STR_SIZE, JOB_STR_FMT, (int)job->pid, job->id, job->priority, job->label);

    if (fprintf(log_file, "%s\n", entry) < 0) {
        fclose(log_file);
        return;
    }

    fclose(log_file);
}

/* 
 * TODO: you must implement this function.
 */
void joblog_delete(proc_t* proc) {
    int saved_errno= errno;
    if(!proc){
        errno= saved_errno;
        return;
    }

    char * log_name = new_log_name(proc);

    if(!log_name){
        errno=saved_errno;
        return;
    }
    if (unlink(log_name) == 0){
        free(log_name);
        errno = saved_errno;
        return;
    }
    free(log_name);
    errno = saved_errno;
}
