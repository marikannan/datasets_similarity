
#define NAMESZ 		  20			/* first , last name size */
#define FILESZ 		  256			/* file name size */
#define COMPKEYSZ	  40			/* composite key size first name & last name */
#define RECORDSZ	  256			/* dataset record size */
#define MAX_REC		  100000 		/* maximum number of records in dataset 1 */
#define MAX_MATCHREC  	  5             /* maximum number of match records */


struct MATCHREC {
    int id;
    char firstname[NAMESZ+1];
    char lastname[NAMESZ+1];
    char compkey[COMPKEYSZ+1];
};

struct DATASET {
	int id;
	char firstname[NAMESZ+1];
	char lastname[NAMESZ+1];
	char compkey[COMPKEYSZ+1];
    int matchcnt;
    struct MATCHREC matchrec[MAX_MATCHREC];
};

extern struct DATASET   DS1[MAX_REC];
extern struct MATCHREC   DS2REC;
extern long long        DS1_TOTALREC;
extern char             recbuf[RECORDSZ+1];
extern char            sep[2];
extern int             jsonfmt;
#define JSONFMT     1
#define NOT_JSONFMT  0


/* Function : compare_ds1compkey - comparator function for qsort algorithm */
extern int compare_ds1compkey (struct DATASET *ds1rec1, struct DATASET *ds1rec2);
/* Function : bsearch_compare - comparator used by binary search algorithm */
extern int bsearch_compare(char *recbuf, struct DATASET *ds1rec);
/* Function: load_dataset1 - Load small dataset which can handle upto 1,00,000 records */
extern void load_dataset1(char *ds1_fname );
/* Function : join_datasets - Function which joins two datasets dataset1 & dataset2 */
extern void join_datasets (char *ds2_fname);
/* Function : print_similar - To print similar records b/w dataset1 and dataset 2 */
extern void print_similar(void);

