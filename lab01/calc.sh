#! usr/bin/bash

# check number
checkNumber () {
	foo=$1
	re='^[+-]?[0-9]+([.][0-9]+)?$'
	if ! [[ $1 =~ $re ]];
	then
		isRightNumber=0
	fi
	
#	for (( i=0; i<${#foo}; i++ )); do
#  		char=${foo:$i:1}
#  		if [[ ! "$char" == *@(0|1|2|3|4|5|6|7|8|9|-|+|.)* ]]
#  		then
#  			isRightNumber=0
#  		fi
#	done
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
	
	while [[ $length > 5 ]]
	do
		length=$(wc -l < hist.txt)
		cp hist.txt hist.txt.tmp
		sed '$ d' hist.txt.tmp > hist.txt
		rm -f hist.txt.tmp
	done
}
checkAns(){
	reans='^[+-]?ans$'
	reANS='^[+-]?ANS$'
	aIsAns=0
	bIsAns=0
	defaultA=$a
	defaultB=$b
	if [[ $a =~ $reans || $a =~ $reANS ]]; then
		firstChar=${a::1}
		a=$ans
		aIsAns=1
		if [[ $firstChar == '-' ]]; then
			a=$(echo "$a*-1" | bc -l)
		fi
	fi
	if [[ $b =~ $reans || $b =~ $reANS ]]; then
		firstChar=${b::1}
		b=$ans
		bIsAns=1
		if [[ $firstChar == '-' ]]; then
			b=$(echo "$b*-1" | bc -l)
		fi
	fi
	if [[ $aIsAns == '0' ]]; then
		checkNumber $a			
	fi 
	if [[ $bIsAns == '0' ]];then
		checkNumber $b
	fi	
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
	'%')
		# MATH ERROR
		if (( $(echo "$b == 0" | bc -l) ));
		then
			echo MATH ERROR
			isRightNumber=0
		else
			ans=$(echo $a $b | awk '{printf("%.2f \n",$1%$2)}')
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
	if [[ $a == "hist" || $a == "HIST" ]];
	then
		hist
	elif [[ $a == "exit" || $a == "EXIT" ]];
	then
		break
	elif [[ $a == "" || $b == "" ]];
	then
		 echo SYNTAX ERROR
	else
		# check use ans and not use ans in operation
		checkAns
		
		if [[ $isRightNumber == '1' ]]
		then
			calc 
			if [[ $isRightNumber == '1' ]]
			then
				# PRINT THE RESULT
				echo $ans
				if [[ $aIsAns == '1' && $bIsAns == '0' ]];
				then
					value=("$defaultA $mode $b = $ans")	
				elif [[ $bIsAns == '1' && $aIsAns == '0' ]];
				then
					value=("$a $mode $defaultB = $ans")
				elif [[ $bIsAns == '1' && $aIsAns == '1' ]]
				then
					value=("$defaultA $mode $defaultB = $ans")
				else
					value=("$a $mode $b = $ans")
				fi
				echo $ans > ans_value.txt
				echo $value | cat - hist.txt > temp && mv temp hist.txt
			fi	
		else
			echo SYNTAX ERROR
		fi
		
	fi
		
	read -p "Press any key to continue..."
	clear
done

