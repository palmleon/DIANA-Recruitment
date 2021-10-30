#!/bin/bash

##########################################
# Code developed for Team DIANA Recruitment
# Leonardo Palmucci, October 2021
##########################################

## In order to satisfay the SSH requirement, it is required that the server supports SSH
## 	on its own (e.g. by using OpenSSH)
## The following program should run on any Bash Shell, 
## 	so it is easy to access using terminals like PuTTY

servicefile="./services.txt"

verify_command () {
	line=$1		# user command line
	nargs=$2 	# number of expected args
	services=$3	# services list
	echo ${#line[@]}
	if [ ${#line[@]} -lt $nargs ]; then
		echo "ERROR: Too few args"
		return 1
	fi
	if [ ${#line[@]} -gt $nargs ]; then
		echo "ERROR: Too many args"
		return 2
	fi
	arg=${line[1]}
	if [ $nargs -gt 1 ]; then
		if [ ! ${services[$arg]} ]; then
			echo "ERROR: Service is not in the list of available services"
			return 3
		fi
	fi
	return 0
}

# Associative array containing all the services, loaded from file
declare -A services
declare -i i=0

end_flag=true

while read line; do
	services+=(["$line"]=$i)
	i=$i+1
done <$servicefile

echo "Welcome to ServiceTool!"
echo "-----------------------------------------------"
echo "To use this tool, use the following commands:"
echo "1) start <service_name>: start a service"
echo "2) stop <service_name>: stop a service"
echo "3) status <service_name>: display service status" 
echo "4) end: terminate the program"
echo "-----------------------------------------------"
echo "-----------------------------------------------"
echo "Available services:"

declare -i count=1

for service in ${!services[@]}; do
	if [ $count != 5 ]; then
		printf "%s\t" "$service"
		count=$count+1
	else 
		printf "%s\n" "$service"
		count=1
	fi
done
echo
echo "------------------------------------------------"

while true; do
## The program goes into a loop to better check if the tool
## 	satisfies the real-time constraint 
## i.e. the user can use the status command multiple times
## 	in a much shorter time

# read user command
read -a line
case ${line[0]} in
	start) 	#start command
		echo START_COMMAND
		verify_command $line 2 $services
		if (( ! $? )); then
			systemctl start ${line[1]}
		fi
		;;
	stop) 	#stop command
		echo STOP_COMMAND
		verify_command $line 2 $services
		if (( ! $? )); then
			systemctl stop ${line[1]}
		fi
		;;
	status) #status command
		echo STATUS_COMMAND
		verify_command $line 2 $services
		if (( ! $? )); then
			echo
			# TODO status command core!!!
		fi
		;;
	end)	#end command
		echo END_COMMAND
		verify_command $line 1 $services
		if (( ! $? )); then
			end_flag=false
		fi
		;;
	*) 
		echo "ERROR: Command not recognized. Retry."
		;;
esac

	# loop condition control
	$end_flag || break
done


exit 0
