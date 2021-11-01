#!/bin/bash

#######################################################################
# The following program implements a command line interface to manage
# Linux services.
# In particular, the program is able to:
# - start services (start <service_name)
# - stop services (stop <service_name>)
# - check the status of a service (status <service_name)
# In order to satisfy the SSH requirement, it is required that the server supports SSH
# 	on its own (e.g. by using OpenSSH)
# The following program should run on any Bash Shell, 
# 	so it is easy to access using terminals like PuTTY
# Since services are loaded from an external file, I have supposed
# that a service can be managed only if it belongs to the list of
# services in the file. 
# The user should be able to observe the status of a service 
# in real-time: for this reason, I have implemented the "status" command
# as an endless loop that keeps providing information about the service
# status. During testing, the best latency of the command was equal to 5ms,
# so it matches the requirements.
# However, the challenge requires the program to work over low-bandwidth.
# I have estimated that, if we have a best latency of 5ms and we have the 
# longest possible string to transmit, i.e. something like
# 	deactivating (1000 ms)            [22 chars + \n]
# the max requested bandwidth should be equal to
#	(1000/5)*22*8 = 36.8 Kb/s
# which should not be too demanding
#
# Code developed for Team DIANA Recruitment
# Leonardo Palmucci, October 2021
#######################################################################

servicefile="./services.txt"

verify_command () {
	line=$1		# user command line
	nargs=$2 	# number of expected args
	services=$3	# services list
	if [ ! ${#line[@]} -eq $nargs ]; then
		echo "ERROR: Wrong number of args"
		return 1
	fi
	if [ $nargs -gt 1 ]; then
		arg=${line[1]}
		if [ ! ${services[$arg]} ]; then
			echo "ERROR: Service is not in the list of available services"
			return 2
		fi
	fi
	return 0
}

# Associative array containing all the services, loaded from file
declare -A services
# Associative array that represents the service status as described in the specifications
declare -A conversion_table=( [active]=running [inactive]=stopped [activating]=starting [deactivating]=stopping [failed]=failed )
declare -i i=0

end_flag=true

while read line; do
	services+=(["$line"]=$i)
	i=$i+1
done <$servicefile

echo "Welcome to ServiceTool!"
echo "---------------------------------------------------------------------------------"
echo "To use this tool, use the following commands:"
echo "1) start <service_name>: start a service"
echo "2) stop <service_name>: stop a service"
echo "3) status <service_name>: display service status"
echo "---------------------------------------------------------------------------------"
echo "---------------------------------------------------------------------------------"
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
echo "---------------------------------------------------------------------------------"

while true; do
## The program goes into a loop to better check if the tool
## 	satisfies the real-time constraint 
## i.e. the user can use the status command multiple times
## 	in a much shorter time

# read user command
read -a line
case ${line[0]} in
	start) 	#start command
		verify_command $line 2 $services
		if (( ! $? )); then
			systemctl start ${line[1]}
		fi
		;;
	stop) 	#stop command
		verify_command $line 2 $services
		if (( ! $? )); then
			systemctl stop ${line[1]}
		fi
		;;
	status) #status command
		verify_command $line 2 $services
		if (( ! $? )); then
			# must be forced to be interrupted 
			while true; do
				# capture first time instant
				(( begin_timestamp = $(date +%s%3N) ))
				# check process status			
				status=$(systemctl is-active ${line[1]}) 
				# capture second time instant
				(( end_timestamp = $(date +%s%3N) ))
				# compute latency
				(( latency = $end_timestamp - $begin_timestamp ))
				# print status and latency
				printf "%s (%d ms)\n" ${conversion_table[$status]} $latency
			done;
		fi
		;;
	*) 	#default behaviour: command not recognized
		echo "ERROR: Command not recognized. Retry."
		;;
esac

	# loop condition control
	$end_flag || break
done


exit 0
