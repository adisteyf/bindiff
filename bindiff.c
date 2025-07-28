#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "defs.h"




/*
uint8_t bytecode[] = {
	next, 0,
	delt, 1,
	next, 0,
	addn, 1, 4,
	quit
};

uint8_t oldfile[] = { / * 1, 3, 4, 4, 5 * /
	1, 2, 3, 4, 5,
};
*/
//uint8_t * oldfileptr = &oldfile[0];
uint8_t * bytecode=0;
uint8_t * oldfile =0;

uint8_t * oldfileptr=0;


uint8_t bytefile[32] = { 0 };
int32_t bytefile_len =   0  ;
int32_t oldfile_len  =   0  ;

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
		bytecode = malloc(size);

		uint8_t * ptr = bytecode;
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
	free(bytecode);
	free(oldfile);
}

uint8_t *
load_cmd (uint8_t * cmd)
{
	uint8_t inst = *cmd;
	cmd++;

	switch (inst) {
case next: {
	uint8_t i = *cmd; i++;
	while (i--) {
		bytefile[bytefile_len++] = *oldfileptr;
		oldfileptr++;
	}

	cmd++;
	break;
}
case repl:
	break;
case addn: {
		bytefile[bytefile_len++] = *oldfileptr;
		oldfileptr++;
		uint8_t i = *cmd;
		while (i--) {
			cmd++;
			bytefile[bytefile_len++] = *cmd;
		}

		cmd++;
		break;
}
case delt: {
	uint8_t i = *cmd;
	while (i--) {
		oldfileptr++;
	}

	cmd++;
	break;
}
case quit: {
			int32_t i = 
				oldfile_len - (oldfileptr - oldfile);
			while (i--) {
				bytefile[bytefile_len++] = *oldfileptr;
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
	int32_t tmp = bytefile_len;
	while (tmp--) {
		printf("%c",bytefile[bytefile_len-tmp-1]);
	}
}


int32_t
main (int argc, char ** argv)
{
	if (argc < 3) { return 2; }
	load_files(argv[1], argv[2]);

	uint8_t * cmd = &bytecode[0];
	for (;;) {
		cmd = load_cmd(cmd);
	}

	return 0;
}
