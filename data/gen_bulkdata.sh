cnt=2001
>smalldataset.txt
while [ $cnt -le 2200 ] 
do
  cat dataset.final | awk  '{print $1"'"$cnt"'","\t",$2"'"$cnt"'","\t",$3"'"$cnt"'"}'  >> smalldataset.txt 
  echo "Counting $cnt"
  let cnt=cnt+1
done
