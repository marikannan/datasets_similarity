# Use Case 
Find name similarity between two datasets. 

Consider two datasets A and B each having ID, first name and last name. First dataset has 100 K rows and second has 10 million rows. Each of 100 K should scan through 10 million rows and find possible matches. Output dataset should provide a clear idea as how 100 K records in Dataset A matches with one or many records in Dataset B.

# Motivation
Based on the details of the datasets , it seems the first dataset is relatively smaller than the other in the ratio of 1:100 ( ie 100K vs 10 million records ). So I am considering the design pattern of loading the smaller dataset into memory and read the second dataset record one by one and find the match and perform the join. 

# Algorithm
``` bash 
// Algorithm:
//
//      load dataset1;
//      sort dataset1;                   // using qsort algorithm
//
//      loop
//          read record from dataset2
//          if record match in dataset1 //using binary search algorithm
//          then
//              add record to matchrec in dataset1
//          endif
//      end loop
//
//      display matchrecords
//
```
The efficiency in sorting  ( the first dataset  dataset1) is accomplished using  "qsort" algorithm. The main core logic of this program is how quickly , it can find the match in the first dataset. For this, I made use of “binary search algorithm” . Here is the complexity details of the algorithms used.

// Efficiency of sorting    : qsort    - O(n log n)

// Efficiency of searching : bsearch  - O(log n)

# Input/Output Format
The dataset record contains identifier, firstname & lastname seperated by “\t” tab space as below,
## Data set Input Format
Both dataset1(smalldataset.txt) & dataset2 (largedataset.txt)
``` bash
/Users/user/wkarea//data $ head smalldataset.txt 
12001 	 Aaron2001 	 Aaberg2001
22001 	 Aaron2001 	 Aaby2001
32001 	 Abbey2001 	 Aadland2001
42001 	 Abbie2001 	 Aagaard2001
52001 	 Abby2001 	 Aakre2001
62001 	 Abdul2001 	 Aaland2001
```
## Output Format:
``` bash 
{ dataset1 record1 } => {
                                          {dataset2 match record1},
                                          {dataset2 match record2},
                                           .....
                                          {dataset2 match recordN}
                                      }
{ dataset1 record2 } => {
                                          {dataset2 match record1},
                                          {dataset2 match record2},
                                           .....
                                          {dataset2 match recordN}
                                      }
```
# Generating larger dataset
For testing the performance/efficiency of the implementation, I prepared the  datasets with 100K records in dataset1 and 10 million records in dataset2 using the below script which take small dataset and add suffix 0...N so that the names will be unique except few cases where I am gonna insert match records to check the output of the application.
``` bash
~/wkarea//data $ cat gen_bulkdata.sh 
#!/bin/bash
cnt=0
>largedataset.txt
while [ $cnt -le 2000 ] 
do
  cat dataset.final | awk  \
       '{print $1"'"$cnt"'","\t",$2"'"$cnt"'","\t",$3"'"$cnt"'"}'  >> largedataset.txt 
  echo "Counting $cnt"
  let cnt=cnt+1
done
```

# Execution & Testing
For performance testing I mocked dataset in such a way that the records are unique in dataset1 ( 1,00,000 records ) and few match records in dataset2 ( which contains 10 million records in total)
``` bash
~/wkarea//data $ ls -rlth
total 665440
-rw-r--r--  1 user  staff   207B Mar 21 02:33 gen_bulkdata.sh
-rw-r--r--  1 user  staff   322M Mar 21 02:53 largedataset.txt
-rw-r--r--  1 user  staff   3.4M Mar 21 02:54 smalldataset.txt

~/wkarea//data $ wc -l smalldataset.txt largedataset.txt 
  100000 smalldataset.txt
 10000000 largedataset.txt
 10100000 total
```

# Output Matched Records
The entire datasets datset1 & dataset2 ( 100K vs 10 million records ) are scanned and completed in 2 mins 8 secs.

``` bash 
/Users/user/wkarea//stats $ cat output_withduration 
{ 49862200, Sunil, Kumar } =>  { 
	{ 234563, Sunil, Kumar },
	{ 986966, Sunil, Kumar },
	{ 49962002, Sunil, Kumar },
    } 
{ 49872200, RameshKumar, Bhatia } =>  { 
	{ 49902203, RameshKumar, Bhatia },
	{ 21234543, RameshKumar, Bhatia },
    } 
      128.55 real       127.91 user         0.52 sys
```


Ran in the background ( application name: similar ):
~/assignment $ nohup time ./similar > stats/output_withduration & 
[2]	2227

/Users/user/wkarea//stats $ jobs
[2] +  Running                 nohup time ./similar > stats/output_withduration &
[1] -  Running                 nohup top -l 1440 -s 5 -pid 2313 > top_statistics.txt &


CPU Usage: ( around 30% spikes in CPU utilization )



# Source Code:

The source code is organized in such a way that it can be used as an API in any other application copying similar.c and similar.h.

``` bash
/Users/user/wkarea/ $ tree
.
├── Makefile
├── data
│   ├── largedataset.txt
│   └── smalldataset.txt
├── inc
│   └── similar.h
├── similar
└── src
    ├── main.c
    └── similar.c

3 directories, 7 files
/Users/user/wkarea/ $ 
```
## API functions:
* load_dataset1   - Load the dataset 1 into memory.
Usage: void load_dataset1(char *filename)
* join_datasets    - Join dataset1 with dataset2.
Usage: void join_datasets (char *filename);
* print_similar    - Print the similar records in datasets against dataset1.
Usage: void print_similar(void)
