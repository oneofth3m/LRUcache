mkdir testdata
for ((i = 0; i < 20; i++))
do
echo $i > testdata/file$i.txt
for ((j = 0; j < 1024*400; j++))
do
echo " => is its file number. There could have been more numbers. There could have been less numbers. But for this file the number is the number at the beginning. You might argue against it. You might argue for it. But that is the truth." >> testdata/file$i.txt
done
done
