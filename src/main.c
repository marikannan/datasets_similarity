//
//  main.c
//  
//
//  Created by Marikannan Muthiah on 20/03/16.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "similar.h"

struct          DATASET DS1[MAX_REC];
long long       DS1_TOTALREC;
struct          MATCHREC  DS2REC;
char            recbuf[RECORDSZ+1];
char            sep[2];
int             jsonfmt;

// Algorithm:
//
//      load dataset1;
//      sort dataset1;                      // using qsort
//
//      loop
//          read record from dataset2
//          if record match in dataset1     // match is found based on binary search
//          then
//              add record to matchrec in dataset1
//          endif
//      end loop
//
//      display matchrecords
//
// Algorithms Used : For better efficiency sorting is performed by "qsort" algorithm and searching the match in dataset1 is accoplished using "binary search".
// Effiency of sorting   : qsort    - O(n log n)
// Effiency of searching : bsearch  - O(log n)


int main(void)
{
	char ds1_fname[FILESZ+1],ds2_fname[FILESZ+1];	/* filename of dataset1 & 2 */
    int  idx = 0, idx2 = 0;
    sep[0]=',';                                     /* defaule seperator used for display */
    jsonfmt = JSONFMT;
    
	sprintf ( ds1_fname, "./data/smalldataset.txt");
	sprintf ( ds2_fname, "./data/largedataset.txt");
    
    
	/* Load the small dataset1 into memory and will be used for join later against dataset 2 */
    load_dataset1(ds1_fname);
    
    /* sort the dataset 1 */
	qsort(DS1,DS1_TOTALREC,sizeof(struct DATASET),compare_ds1compkey);
    
    /* find the match for each record in dataset1 in dataset2 */
    join_datasets(ds2_fname);
    
    /* diplay the join results */
    print_similar();
    
	return 0;
}
