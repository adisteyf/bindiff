#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "defs.h"




uint8_t * oldfile   =0;
uint8_t * bytefile  =0;
uint8_t * newfile   =0;
uint8_t * oldfileptr=0;

uint64_t newfile_len=0;
uint64_t oldfile_len=0;
uint64_t newfile_cap=0;

void
load_files (char * oldname, char * patchname)
{
	{
		FILE * oldf = fopen(oldname, "r");
		if (!oldf) { exit(1); }

		/* alloc */
		fseek(oldf, 0, SEEK_END);
		oldfile_len = ftell(oldf);
		fseek(oldf, 0, SEEK_SET);
		oldfile = malloc(oldfile_len);

		uint8_t * ptr = oldfile;
		int c = getc(oldf);
		while (c != EOF) {
			*ptr = (uint8_t)c;
			ptr++;
			c = getc(oldf);
		}

		oldfileptr = oldfile;
		fclose(oldf);
	}

	{
		FILE * patchf = fopen(patchname, "r");
		if (!patchf) {
			free(oldfile);
			exit(1); /* TODO: perror */
		}

		/* alloc */
		fseek(patchf, 0, SEEK_END);
		long size = ftell(patchf);
		fseek(patchf, 0, SEEK_SET);
		bytefile = malloc(size);

		uint8_t * ptr = bytefile;
		int c = getc(patchf);
		while (c != EOF) {
			*ptr = (uint8_t)c;
			ptr++;
			c= getc(patchf);
		}

		fclose(patchf);
	}
}

void
free_mem (void)
{
	free(bytefile);
	free(newfile);
	free(oldfile);
}

uint8_t *
load_cmd (uint8_t * cmd)
{
	uint8_t inst = *cmd;
	cmd++;

	switch (inst) {
case next: {
	uint32_t i = *((int*)cmd);
	i++;
	cmd += 3;

	while (i--) {
		newfile[newfile_len++] = *oldfileptr;
		oldfileptr++;
	}

	cmd++;
	break;
}
case repl: {
	uint32_t i = *((int*)cmd);
	cmd += 3;
	while (i--) {
		cmd++;
		newfile[newfile_len++] = *cmd;
		oldfileptr++;
	}

	break;
}
case addn: {
		newfile[newfile_len++] = *oldfileptr;
		oldfileptr++;
		uint8_t i = *((int*)cmd); cmd += 3;
		while (i--) {
			cmd++;
			newfile[newfile_len++] = *cmd;
		}

		cmd++;
		break;
}
case delt: {
	uint32_t i = *((int*)cmd); cmd += 3;
	while (i--) {
		oldfileptr++;
	}

	cmd++;
	break;
}
case quit: {
			int32_t i = oldfile_len - (oldfileptr - oldfile);
			while (i--) {
				newfile[newfile_len++] = *oldfileptr;
				oldfileptr++;
			}

			print_file();
			free_mem();
			exit(0);
			break;
		}
	}

	return cmd;
}

void
print_file (void)
{
	int32_t tmp = newfile_len;
	while (tmp--) {
		printf("%c",newfile[newfile_len-tmp-1]);
	}
}


int32_t
main (int argc, char ** argv)
{
	if (argc < 3) { return 2; }
	load_files(argv[1], argv[2]);
	uint8_t * cmd = &bytefile[0];

	uint8_t cap[8] = { 0 };
	int32_t i=8;
	while (i--) {
		cap[7-i] = *cmd;
		cmd++;
	}

	memcpy(&newfile_cap,cap,sizeof(uint64_t));
	newfile = malloc(newfile_cap);
	for (;;) {
		cmd = load_cmd(cmd);
	}

	return 0;
}
