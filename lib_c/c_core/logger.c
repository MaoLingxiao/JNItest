#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/file.h>

#include "logger.h"

//log大小8k
#define log_buffer_size 8192

//打开日志 返回文件指针
FILE *log_open(const char *filename) {
    FILE *log_fp = fopen(filename, "a");
    if (log_fp == NULL) {
        fprintf(stderr, "can not open log file %s", filename);
    }
    return log_fp;
}

//关闭日志
void log_close(FILE *log_fp) {
    if (log_fp != NULL) {
        fclose(log_fp);
    }
}

//写日志
void log_write(int level, const char *source_filename, const int line, const FILE *log_fp, const char *fmt, ...) {
	  //等级
    char LOG_LEVEL_NOTE[][10] = {"FATAL", "WARNING", "INFO", "DEBUG"};
    //空引用
    if (log_fp == NULL) {
        fprintf(stderr, "can not open log file");
        return;
    }
    static char buffer[log_buffer_size];
    static char log_buffer[log_buffer_size];
    //日期
    static char datetime[100];
    //行号
    static char line_str[20];
    //现在时间
    static time_t now;
    now = time(NULL);
		//格式化输出时间
    strftime(datetime, 99, "%Y-%m-%d %H:%M:%S", localtime(&now));
    //格式化行号
    snprintf(line_str, 19, "%d", line);
    //格式化日志信息
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(log_buffer, log_buffer_size, fmt, ap);
    va_end(ap);

    int count = snprintf(buffer, log_buffer_size,
                         "%s [%s] [%s:%d]%s\n",
                         LOG_LEVEL_NOTE[level], datetime, source_filename, line, log_buffer);
    fprintf(stdout, "%s", buffer);
    int log_fd = fileno((FILE *) log_fp);
    if (flock(log_fd, LOCK_EX) == 0) {
        if (write(log_fd, buffer, (size_t) count) < 0) {
            fprintf(stderr, "write error");
            return;
        }
        flock(log_fd, LOCK_UN);
    }
    else {
        fprintf(stderr, "flock error");
        return;
    }
}