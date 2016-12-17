#include "challenge8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matasano.h"

#define FILENAME "challenge8.txt"
#define BLOCK_SIZE 16

void try_challenge8()
{
    FILE * f = fopen(FILENAME,"r");
    char * line = NULL;
    ssize_t l;
	int bestMatchCount = 0;
	int bestLineNo = -1;
	int lineno = 0;
	char * bestLine = NULL;
	size_t n = 0;

	if(f == NULL){
        fprintf(stderr,"Could not open %s for reading\n",FILENAME);
        return;
    }

    while((l = getline(&line,&n,f)) > 0){
        struct bigint bi;
		int matchcount = 0;

        if(line[l-1] == '\n'){
            l--;
            line[l] = '\0'; 
        }
        hex2val(line,&bi);
		int nb = bi.n/BLOCK_SIZE;
		int i,j,k;

		for(i=0; i<nb; i++){
			const unsigned char * b1 = &bi.bytes[i*BLOCK_SIZE];
			for(j=i+1; j<nb; j++){
				const unsigned char * b2 = &bi.bytes[j*BLOCK_SIZE];
				int match = 1;
				for(k=0; match && k<BLOCK_SIZE; k++){
					if(b1[k] != b2[k]){
						match = 0;
					}
				}
				matchcount += match;
			}
		}
		printf("Line %d, Matchcount: %d\n",
			   lineno,matchcount);
		if(matchcount > bestMatchCount){
			bestMatchCount = matchcount;
			bestLine = realloc(bestLine,l+1);
			bestLineNo = lineno;
			memcpy(bestLine,line,l+1);
			printf("New best. Line: %s\n",bestLine);
		}
		lineno++;
	}
	if(bestMatchCount > 0){
		printf("Best Line: %d, Match Count: %d\n%s\n",bestLineNo,bestMatchCount,bestLine);
	}
}

