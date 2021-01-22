
while :
do
	./make_test
	for i in 1 2 3 4 5 6 7 8 9 10 11
	do
		mpirun -np $i ./a.out in.txt out.txt
		./checker
	done
done