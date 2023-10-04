#! usr/bin/bash

# check number
checkNumber () {
	foo=$1
	for (( i=0; i<${#foo}; i++ )); do
  		char=${foo:$i:1}
  		if [[ ! "$char" == *@(0|1|2|3|4|5|6|7|8|9|-|+|.)* ]]
  		then
  			isRightNumber=0
  		fi
	done
}
hist () {
	# get 5 first line
	i=0
	while read  line
	do
		if [[ $i == 5 ]]
		then
			break;
		fi
		i=$((i+1))
		echo $line;
	done < hist.txt
	# delete last line
	length=$(wc -l < hist.txt)
	#https://www.baeldung.com/linux/bash-single-vs-double-brackets
	while [[ $length > 5 ]]
	do
		length=$(wc -l < hist.txt)
		cp hist.txt hist.txt.tmp
		sed '$ d' hist.txt.tmp > hist.txt
		rm -f hist.txt.tmp
	done
}
calc () {
	case "$mode" in
	"+")
		ans=$(echo $a $b | awk '{printf("%.2f \n",$1+$2)}')
	;;
	'-')
		ans=$(echo $a $b | awk '{printf("%.2f \n",$1-$2)}')
	;;
	'*')
		mode='\\*'
		ans=$(echo $a $b | awk '{printf("%.2f \n",$1*$2)}')
	;;
	'/')
		# MATH ERROR
		if (( $(echo "$b == 0" | bc -l) ));
		then
			echo MATH ERROR
			isRightNumber=0
		else
			ans=$(echo $a $b | awk '{printf("%.2f \n",$1/$2)}')
		fi
	;;
	*)
		echo SYNTAX ERROR
		isRightNumber=0
	;;
	esac
}
# read command

clear
while true
do	
	ans=$(cat ans_value.txt)
	#echo 'ans value': $ans
	echo -n ">> "
	read a mode b
	isRightNumber=1
	if [[ $a == "hist" ]];
	then
		hist
	elif [[ $a == "exit" ]];
	then
		break
	else
		# check use ans and not use ans in operation
		if [[ $a == "" || $b == "" ]]
		then 
			isRightNumber=0
		elif [[ $a == "ans" && ! $b == "ans" ]]
		then
			a=$ans
			checkNumber $b 
		elif [[ $b == "ans" && ! $a == "ans" ]]
		then
			b=$ans
			checkNumber $a
		elif [[ $b == "ans" && $a == "ans" ]]
		then
			b=$ans
			a=$ans
		else
			checkNumber $a
			checkNumber $b	
		fi
		
		if [[ $isRightNumber == '1' ]]
		then
			calc 
			if [[ $isRightNumber == '1' ]]
			then
				# PRINT THE RESULT
				echo $ans
				value=("$a $mode $b")
				echo $ans > ans_value.txt
				#https://superuser.com/questions/246837/how-do-i-add-text-to-thebeginning-of-a-file-in-bash/1352628#1352628
				echo $value | cat - hist.txt > temp && mv temp hist.txt
			fi	
		else
			echo SYNTAX ERROR
		fi
		
	fi
	
	read -p "Press any key to continue..."
	clear
done

