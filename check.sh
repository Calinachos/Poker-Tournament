#!/bin/bash

CONF_DIR="conf/"
INPUT_DIR="in/"
OUT_DIR="out/"
REF_DIR="ref/"
EXE_NAME="poker_tournament"
VALGRIND="valgrind --leak-check=full -q --log-file=a.out"
max_points=85
result_points=0
verify_vg=0
red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`
yellow=`tput setaf 3`

# ------------------------- USEFUL FUNCTIONS ------------------------- #

print_header()
{
	header="${1}"
	header_len=${#header}
	printf "\n"
	if [ $header_len -lt 71 ]; then
		padding=$(((53 - $header_len) / 2))
		for ((i = 0; i < $padding; i++)); do
			printf " "
		done
	fi
	printf "= %s =\n\n" "${header}"
}

test_do_fail()
{
	printf " ${red}failed${reset}  [0/%d]" "${1}"
	if test "x$EXIT_IF_FAIL" = "x1"; then
		exit 1
	fi
}

test_do_pass()
{
	printf " ${green}passed${reset}  [%d/%d]" "${1}" "${1}"
	((result_points+=${1}))
}

valgrind_ok()
{
	printf "  VALGRIND ..... ${green}OK${reset}\n"
	((verify_vg+=1))
}

valgrind_failed()
{
	printf "  VALGRIND ..... ${red}FAILED${reset}\n"
}

valgrind_failed_sf()
{
	printf "  VALGRIND ..... ${red}SEG FAULT${reset}\n"
}

valgrind_bonus_msg()
{
	if [ $verify_vg -eq ${1} ]; then
		printf "${yellow}VALGRIND BONUS${reset} ........ ${green}passed${reset}  [%d/%d]\n\n" ${2} ${2}
		((result_points+=${2}))
	else
		printf "${yellow}VALGRIND BONUS${reset} ........ ${red}failed${reset}  [0/%d]\n\n" ${2}
	fi
}

test_function()
{
	conf_file="$CONF_DIR${1}"
	input_file="$INPUT_DIR${2}"
	output_file="$OUT_DIR${3}"
	ref_file="$REF_DIR${4}"
	points_per_test="${5}"

	{ ./$EXE_NAME $conf_file $input_file $output_file; } 2> check.out
	if [ "$(cat check.out | grep -o "Segmentation fault")" == "Segmentation fault" ]; then
		printf "[test%02d.in] ..........." ${6}
		test_do_fail ${5}
		valgrind_failed_sf
	
	else
		diff -Z $output_file $ref_file > /dev/null
		if test $? -eq 0; then
			printf "[test%02d.in] ..........." ${6}
			test_do_pass $points_per_test
			((verify_vg+=1))

			$VALGRIND ./$EXE_NAME $conf_file $input_file $output_file > /dev/null
			if [[ -z $(cat a.out) ]]; then
				valgrind_ok
			else
				valgrind_failed
			fi
		else
			printf "[test%02d.in] ..........." ${6}
			test_do_fail ${5}
			valgrind_failed
		fi
	fi
}

bonus_function()
{
	echo ""
}

init_function()
{
	make build &> /dev/null

	if [ $? -ne 0 ]; then
		echo -e "Building failed!"
		exit 1
	fi

	if [ ! -e poker_tournament ]; then
		echo "No executable! Make sure you name it poker_tournament."
		exit 1
	fi
	mkdir $OUT_DIR &> /dev/null
	if [ ! -e in ]; then
		echo "No in directory!"
		exit 1
	fi
	if [ ! -e conf ]; then
		echo "No conf directory!"
		exit 1
	fi
	if [ ! -e ref ]; then
		echo "No ref directory!"
		exit 1
	fi
}

testing()
{
	verify_vg=0
	declare -a tests_values=(0 1 1 1 1 2 2 1 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 3 3 3 3 4 4 4)
	declare -a valgrind_test_values=(2 2 2 3 3 3 5)
	for ((i=$1; i<=$2;i++))
	do
		test_function "conf$i.txt" "test$i.in" "test$i.out" "test$i.ref" "${tests_values[$i]}" $i
	done
	valgrind_bonus_msg ${3} ${valgrind_test_values[${4}]} 
}

finish()
{
	printf "\nTOTAL ...................... [%d/%d]\n" $result_points $max_points
	if [ $result_points -eq 105 ]; then
		echo -e " \n      ${yellow}Congrats, you rock! :D${reset}"
	fi
	echo ""

	rm poker_tournament
}

clean_out_function()
{
	rm out/*.*
	rm a.out
	rmdir out
	rm check.out
	
}

# -------------------- TESTING ---------------------- #

init_function

print_header "TEMA 1 - POKER TOURNAMENT"

#Testing

testing 1 3 6 0
testing 4 6 6 1
testing 7 13 14 2
testing 14 16 6 3
testing 17 30 28 4
testing 31 33 6 5
testing 34 40 14 6

finish
clean_out_function
