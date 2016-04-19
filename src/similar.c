#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "similar.h"


/* Function : gen_compkey - generate compositekey */
static void gen_compkey(char *compkey, const struct DATASET *ds1) {
	memset(compkey,0x00,COMPKEYSZ);
	snprintf(compkey,COMPKEYSZ,"%s|%s",ds1->firstname,ds1->lastname);
}

/* Function : init_fields - initialize the fileds  */
static void init_fields(struct DATASET *ds1) {
    int idx = 0;
    
    ds1->id = 0;
    memset(ds1->firstname,0x00,NAMESZ+1);
    memset(ds1->lastname,0x00,NAMESZ+1);
    memset(ds1->compkey,0x00,COMPKEYSZ+1);
    ds1->matchcnt = 0;
    
    for ( idx = 0 ; idx < MAX_MATCHREC; idx++ ) {
        ds1->matchrec[idx].id = 0;
        memset(ds1->matchrec[idx].firstname,0x00,NAMESZ+1);
        memset(ds1->matchrec[idx].lastname,0x00,NAMESZ+1);
        memset(ds1->matchrec[idx].compkey,0x00,COMPKEYSZ+1);
    }
}

/* Function : get_fields - seperate the fields from the tab '\t' seperated record */
static void get_fields(char *recbuf, struct DATASET *ds1) {
	sscanf(recbuf,"%d%s%s",&ds1->id,ds1->firstname,ds1->lastname);
    memset(ds1->compkey,0x00,COMPKEYSZ);
    snprintf(ds1->compkey,COMPKEYSZ,"%s|%s",ds1->firstname,ds1->lastname);
}

/* Function : get_fields - seperate the fields from the tab '\t' seperated record */
static void get_fields2(char *recbuf, struct MATCHREC *matchrec ) {
    matchrec->id = 0;
    memset(matchrec->firstname,0x00,NAMESZ+1);
    memset(matchrec->lastname,0x00,NAMESZ+1);
	sscanf(recbuf,"%d%s%s",&matchrec->id,matchrec->firstname,matchrec->lastname);
    memset(matchrec->compkey,0x00,COMPKEYSZ);
	snprintf(matchrec->compkey,COMPKEYSZ,"%s|%s",matchrec->firstname,matchrec->lastname);
    
}

/* Function : print_dsrec - to print a data set record */
static void print_dsrec(const struct DATASET ds,char sep[]) {
    if ( jsonfmt == JSONFMT )
        printf("{ %d, %s, %s } => ",ds.id,ds.firstname,ds.lastname);
    else
        printf("%d%s%s%s%s \n",ds.id,sep,ds.firstname,sep,ds.lastname);
}

/* Function : print_matchrec - to print a data set 2 record preceding with single tab */
static void print_matchrec(const struct MATCHREC matchrec ,char sep[]) {
    if ( jsonfmt == JSONFMT )
        printf("\t{ %d, %s, %s }",matchrec.id,matchrec.firstname,matchrec.lastname);
    else
        printf("\t%d%s%s%s%s \n",matchrec.id,sep,matchrec.firstname,sep,matchrec.lastname);
}

/* Function : print_ds - to print the entire data set */
static void print_ds(void) {
	int idx = 0;

	for ( idx = 0; idx < DS1_TOTALREC; idx++ )
		print_dsrec(DS1[idx],sep);
    
	printf("Total number of records : %lld\n",DS1_TOTALREC);
	
}

/* Function : add_matchrec - To add match record */
static void add_matchrec (struct DATASET *ds1rec, struct MATCHREC  *matchrec ) {
    if ( ds1rec->matchcnt+1 <= MAX_MATCHREC ) {
        ds1rec->matchrec[ds1rec->matchcnt].id = matchrec->id;
        memcpy(ds1rec->matchrec[ds1rec->matchcnt].firstname,matchrec->firstname,NAMESZ);
        memcpy(ds1rec->matchrec[ds1rec->matchcnt].lastname,matchrec->lastname,NAMESZ);
        memcpy(ds1rec->matchrec[ds1rec->matchcnt].compkey,matchrec->compkey,COMPKEYSZ);
        ds1rec->matchcnt += 1;
    }
}

/* Function : compare_ds1compkey - comparator function for qsort algorithm */
int compare_ds1compkey (struct DATASET *ds1rec1, struct DATASET *ds1rec2) {
	char compkey1[COMPKEYSZ];
	char compkey2[COMPKEYSZ];

	memset(compkey1,0x00,COMPKEYSZ);
	memset(compkey2,0x00,COMPKEYSZ);

	gen_compkey(compkey1,ds1rec1);
	gen_compkey(compkey2,ds1rec2);

	return(memcmp(compkey1,compkey2,COMPKEYSZ));
}

/* Function : bsearch_compare - comparator function for binary search algorithm  */
int bsearch_compare(char *recbuf, struct DATASET *ds1rec) {
    char compkey1[COMPKEYSZ];
	char compkey2[COMPKEYSZ];
    struct MATCHREC matchrec ;
    int rc = 0;
    int idx = 0;
    
	memset(compkey2,0x00,COMPKEYSZ);
    get_fields2(recbuf,&matchrec);
	gen_compkey(compkey2,ds1rec);
    
    rc = memcmp(matchrec.compkey,compkey2,COMPKEYSZ);
    if ( 0 == rc )
        add_matchrec(ds1rec,&matchrec);
    
	return(rc);
}

/* Function: load_dataset1 - Load small dataset which can handle upto 1,00,000 records */
void load_dataset1(char *ds1_fname ) {
    FILE *fpds1;
	int  idx = 0;
    
	fpds1 = fopen(ds1_fname, "r" );
	if ( fpds1 == NULL ) {
		printf ("Error : Not able to open data set 1 : %s. \nExitting ...\n",ds1_fname);
		exit (-1);
	}
    memset(recbuf,0x00,RECORDSZ);               /* clear record buffer */
    memset(recbuf,0x00,256);               /* clear record buffer */
    
	idx = 0;
	while ( fgets (recbuf,256,fpds1) != NULL ) {
        init_fields(&DS1[idx]);
		get_fields(recbuf,&DS1[idx]);
		// print_dsrec(DS1[idx],"|");		/* '|' pipe is a seperator while displaying */
		idx++;
	}
	DS1_TOTALREC = idx ; 	/* total number of records loaded into memory. */
}

/* Function : join_datasets - Function which joins two datasets dataset1 & dataset2 */
void join_datasets (char *ds2_fname) {
    int  idx = 0;
    FILE *fpds2; 				/* file pointer to data set1 & data set 2 */
    
    fpds2 = fopen(ds2_fname, "r" );
	if ( fpds2 == NULL ) {
		printf ("Error : Not able to open data set 2 : %s. \nExitting ...\n",ds2_fname);
		exit (-1);
	}
    memset(recbuf,0x00,RECORDSZ);               /* clear record buffer */
    
	idx = 0;
	while ( fgets (recbuf,256,fpds2) != NULL ) {
		get_fields2(recbuf,&DS2REC);
        bsearch(recbuf,DS1,DS1_TOTALREC,sizeof(struct DATASET),&bsearch_compare);
		idx++;
	}
}

/* Function : print_similar - To print similar records b/w dataset1 and dataset 2 */
void print_similar(void) {
    int  idx = 0, idx2 = 0;
    
    /* diplay the join results */
    idx=0;
    for ( idx =0; idx < DS1_TOTALREC ; idx++ ) {
        if ( DS1[idx].matchcnt ) {
            print_dsrec(DS1[idx],sep);
            if ( jsonfmt == JSONFMT )
                printf(" { \n");
            
            for ( idx2 = 0 ; idx2 < MAX_MATCHREC && DS1[idx].matchrec[idx2].id != 0; idx2++ ) {
                print_matchrec(DS1[idx].matchrec[idx2],sep);
                jsonfmt == JSONFMT ? printf(",\n") : printf("\n") ;
            }
            if ( jsonfmt == JSONFMT )
                printf("    } \n");
        }
    }
}

