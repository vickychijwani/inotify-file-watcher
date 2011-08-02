#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/inotify.h>

#include "bash_colors.h"

#define EVENT_SIZE sizeof(struct inotify_event)
#define BUF_LEN 1024 * (EVENT_SIZE + 16)

char **files;
int *watchers, watched_file_count;

char *watcher_to_file(int watcher);
size_t trim(char *out, size_t len, const char *str);

int main() {
  int fd, length, i = 0, status;
  char *file;
  char buf[1000], buf2[1000];
  char buffer[BUF_LEN];
  FILE *fp;

  fd = inotify_init();

  files = malloc(100 * sizeof(char *));
  fp = fopen("watcher.files", "r");
  if(fp == NULL) {
    printf("%sThe list of watchable files is absent.%s", RED, DEFAULT);
    exit(1);
  }
  while(fscanf(fp, "%s", buf) != EOF) {
    files[i] = malloc(1000 * sizeof(char));
    strcpy(files[i], buf);
    i++;
  }
  watched_file_count = i;
  fclose(fp);

  watchers = malloc(watched_file_count * sizeof(int));

  for(i=0; i<watched_file_count; i++) {
    file = files[i];
    watchers[i] = inotify_add_watch(fd, file, IN_CLOSE_WRITE);
    if(watchers[i] > 0 && watchers[i] < 100) {
      printf("%sWatching %s.%s\n", GREEN, file, DEFAULT);
    }
    else {
      printf("%sError watching %s.%s\n", RED, file, DEFAULT);
    }
  }

  while(1) {
    i = 0;
    length = read(fd, buffer, BUF_LEN);

    if(length < 0) {
      printf("%sError in reading inotify event.%s\n", RED, DEFAULT);
    }

    while(i < length) {
      struct inotify_event *event = (struct inotify_event *) &buffer[i];
      char *filename = watcher_to_file(event->wd);
      printf("\n%sFile %s modified.%s\n", BOLD_BLUE, filename, DEFAULT);

      fp = fopen("watcher.commands", "r");
      if(fp == NULL) {
	printf("%sThe list of post-file-modification commands is absent.%s\n", RED, DEFAULT);
	exit(1);
      }
      while(fgets(buf, sizeof(buf), fp) != NULL) {    // fgets() returns NULL on EOF or error
	sprintf(buf2, buf, filename);
	trim(buf, sizeof(buf), buf2);	
	if(strcmp(buf, "") != 0) {
	  printf("%sRunning '%s'...%s\n", CYAN, buf, DEFAULT);
	  status = system(buf);
	  if(status == 0)
	    printf("%sdone.%s\n", GREEN, DEFAULT);
	  else {
	    printf("%sfailed.%s\n", RED, DEFAULT);
	  }
	}
      }
      fclose(fp);

      i += EVENT_SIZE + event->len;
    }
  }

  close(fd);

  return 0;
}

char *watcher_to_file(int watcher) {
  int i;
  for(i=0; i<watched_file_count; i++) {
    if(watchers[i] == watcher) {
      return files[i];
    }
  }
  return "";
}

size_t trim(char *out, size_t len, const char *str) {
  if(len == 0)
    return 0;

  const char *end;
  size_t out_size;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0) {  // All spaces?
    *out = 0;
    return 1;
  }

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;
  end++;

  // Set output size to minimum of trimmed string length and buffer size minus 1
  out_size = (end - str) < len-1 ? (end - str) : len-1;

  // Copy trimmed string and add null terminator
  memcpy(out, str, out_size);
  out[out_size] = 0;

  return out_size;
}
