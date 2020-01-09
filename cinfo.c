#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include "config.h"

int page_size, page_mask;

#ifndef HAVE_MINCORE
int mincore(void *start, size_t len, unsigned char *vec)
{
	return syscall(218,start,len,vec);
}
#endif


void setpagevalues()
{
	page_size=getpagesize();
	page_mask=(~(page_size-1));
}


void usage(void)
{
	fprintf(stderr,"syntax:\n\tcinfo [options] [filenames]\n");
	fprintf(stderr,"options:\n");
	fprintf(stderr,"\t-b --bar\toutput a screen-wide bar of cache information\n");
	fprintf(stderr,"\t-d --dump\toutput vector describing cache state of all pages\n");
	fprintf(stderr,"\t-e --evict\tevict file from page cache and show remaining pages\n");
	fprintf(stderr,"\t-f --file\tfile of files or - to read from stdin\n");
	fprintf(stderr,"\t-h --help\tprint this helpful message\n");
	fprintf(stderr,"\t-l --leave\tleave the last page cached\n");
	fprintf(stderr,"\t-s --stats\toutput only statistics\n");
	fprintf(stderr,"\t-t --totals\toutput totals of all files\n");

}

off_t filesize(int fd)
{
	struct stat buf;
	if(!fstat(fd,&buf))
		return buf.st_size;
	return -1;
}

void error(char *format, ...)
{
	va_list ap;
	va_start(ap,format);
	vfprintf(stderr, format, ap);
	va_end(ap);
}

void die(char *format, ...)
{
	fflush(stdout);
	error(format);
	exit(1);
}

int notregular(char *fname)
{
	struct stat buf;
	if (stat(fname, &buf) == -1)
		return -1;
	else
		return (!S_ISREG(buf.st_mode));
}


int do_bar;
int do_evict;
int do_leave;
int do_stats;
int do_dump;
int do_totals;
long long total_pages;
long long total_cached;

void dototals()
{
	printf("\nTotals: %llu pages, %llu cached (%.2f%%)\n",
		   total_pages,
		   total_cached,
		   total_cached*100.0/total_pages);
}

void dofile(char *fname)
{
	unsigned long  p; 
	int i;
	size_t num_pages;
	int fd;
	int skip_evict = 0;

	unsigned char *map;
	off_t fsize, uncache_size;

	if(notregular(fname))
		return;

	printf("%s: ",fname);
	fflush(stdout);

	if((fd=open(fname,O_RDONLY,0))<0)
	{
		error("%s\n",strerror(errno));
		return;
	}

	fsize=filesize(fd);
	uncache_size = fsize;

	if (fsize == 0) {
		error("File size 0 cannot have any cached pages\n");
		close(fd);
		return;
	}

	if (do_leave) {
		if (fsize % page_size) {
			uncache_size = fsize - (fsize % page_size);
		} else{
			uncache_size = fsize - page_size; 
		}

		if (uncache_size < page_size)
			skip_evict = 1;
	}

	if (do_evict && !skip_evict) {
		if((errno=posix_fadvise(fd, 0, uncache_size, POSIX_FADV_DONTNEED))) {
			error("posix_fadvise returned %s\n", strerror(errno));
			close(fd);
			return;
		}
	}

	p=(unsigned long)mmap(0, fsize, PROT_READ, MAP_SHARED, fd,0);
	
	if(p==-1) {
		error("mmap returned error: %s\n",strerror(errno));
		close(fd);
		return;
	}
	
	if(p%page_size) 
		die("mmap returned non-aligned pointer, don't know how to handle, exiting\n");
	
	num_pages=(fsize+page_size-1)/page_size;
	total_pages+=num_pages;
	
	if(!(map=malloc(num_pages))) 
		die("unable to allocate memory: %s",strerror(errno));

	
	if(mincore((void *)((p+~page_mask)&page_mask),num_pages*page_size, map)) 
		die("kernel returned: %s\n",strerror(errno));

	if(do_totals)
		for(i=0;i<num_pages;i++)
			if(map[i]&1)
				total_cached++;


	if(do_dump) {
		for(i=0;i<num_pages;i++) {
			printf("%c",(map[i]&1) ? 'X' : '_');
		}
		
		printf("\n");
	}
	else if(do_stats) {
		int num_incore=0;
		for(i=0;i<num_pages;i++) {
			if(map[i]&1)
				num_incore++;
		}
		
		printf("%zu pages, %u pages cached (%.2f%%)\n",
			   num_pages,
			   num_incore,
			   num_pages ? (num_incore*100.00)/num_pages : 0);
	}
	else if(do_bar)
	{
		int width=80-strlen(fname)-4;
		double leap;
		int num_incore=0;
		int last_pos=-1;
		int leap_pages=0;

		if(num_pages<width)
			leap=1;
		else
			leap=1.0*num_pages/width;

		for(i=0;i<num_pages;i++) {
			if(map[i]&1)
				num_incore++;

			leap_pages++;

			if((int)(i/leap)>last_pos) {
				int proc=3.0*num_incore/leap_pages;
				char c='?';

				last_pos=i/leap;

				switch(proc) {
				case 0: c='.';break;
				case 1: c='_';break;
				case 2: c='-';break;
				case 3: c='X';break;
				}
				
				printf("%c",c);
				num_incore=leap_pages=0;
			}

			
		}
		printf("\n");
		
	}
	munmap((void *)p, num_pages*page_size);
	free(map);
	close(fd);

}	

int main(int argc, char **argv)
{
	char c;
	char *file_list = NULL;

	do_stats=1; // default

	if(argc==1) {
		usage();
		exit(1);
	}

	while (1)
	{
		int option_index = 0;
		static struct option long_options[] =
			{
				{"bar"    ,0,0,'b'},
				{"dump"   ,0,0,'d'},
				{"evict"  ,0,0,'e'},
				{"file"   ,1,0,'f'},
				{"help"   ,0,0,'h'},
				{"leave"  ,0,0,'l'},
				{"stats"  ,0,0,'s'},
				{"totals" ,0,0,'t'},
				{0,0,0,0}
			};
		
		c = getopt_long (argc, argv, "bdef:shlt",
				 long_options, &option_index);
		if (c == -1)
			break;
		
		
		switch (c)
		{
		case 'b':
			do_bar=1;do_stats=0;
			break;
		case 'd':
			do_dump=1;do_stats=0;
			break;
		case 'e':
			do_evict=1;
			break;
		case 'f':
			if (!(file_list = strdup(optarg))) {
				die("Unable to allocate file_list %s\n", strerror(errno));
			}
			break;
		case 'h':
			usage();
			exit(0);
			break;
		case 'l':
			do_leave=1;
			break;
		case 's':
			do_stats=1;
			break;
		case 't':
			do_totals=1;
			break;
			
		default:
			printf ("?? getopt returned character code 0%o ??\n", c);
			break;
		}
	}

	setpagevalues();

	if ( optind == argc && !file_list) {
		usage();
		exit(0);
	}

	if (file_list) {
		FILE *f;
		char fname[PATH_MAX];

		if (strcmp(file_list, "-")) { 
			if (!(f = fopen(file_list, "r"))) {
				die("Failed to open file %s\n", strerror(errno));
			}
		} else {
			f = stdin;
		}

		free(file_list);

		while (fgets(fname, PATH_MAX, f)) {
			char *newline; 

			if ((newline = strchr(fname, '\n')))
				newline[0] = 0;

			dofile(fname);
		}

		fclose(f);

	} else {
		while (optind < argc)
			dofile(argv[optind++]);
	}

	if (do_totals)
		dototals();

	return 1;
}
